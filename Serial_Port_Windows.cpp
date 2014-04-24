#include "Serial_Port_Windows.h"

#include <sstream>
#include <algorithm>

namespace RS_232{
    //Settings modifiers
    const Serial_Port_Windows::error_type& Serial_Port_Windows::check_status(){
        DWORD error_code(0);
        ClearCommError(m_handle, &error_code, nullptr);

        switch(error_code){
        //Error messages are from the msdn website
            case 0:
                m_error = error_type();
                break;
            case CE_BREAK:
                m_error = error_type("The hardware detected a break condition.");
                break;
            case CE_FRAME:
                m_error = error_type("The hardware detected a framing error.");
                break;
            case CE_OVERRUN:
                m_error = error_type("A character-buffer overrun has occurred. "
                    "The next character is lost.");
                break;
            case CE_RXOVER:
                m_error = error_type("An input buffer overflow has occurred. "
                    "There is either no room in the input buffer, "
                    "or a character was received after the "
                    "end-of-file (EOF) character.");
                break;
            case CE_RXPARITY:
                m_error = error_type("The hardware detected a parity error.");
                break;
            default:
                m_error = error_type("Unknown error.");
                break;
        }
        return m_error;
    }

    //C-style I/O
    bool Serial_Port_Windows::open(
        count_type port_number,
        baud_rate new_baud_rate,
        size_type new_read_rate
    ){
        if(port_number == m_port)
            return true;

        m_connected = false;
        m_port = port_number;
        m_baud_rate = new_baud_rate;
        m_read_rate = new_read_rate;

        std::stringstream ss;
        ss << port_number;

        vol_str_type port_name("\\\\.\\COM" + ss.str());

        if(
            (m_handle = CreateFileA(
                port_name.c_str(),
                GENERIC_READ | GENERIC_WRITE,
                0, //no share
                nullptr, //no security
                OPEN_EXISTING,
                0, //no threads
                nullptr //no template
            ))  == INVALID_HANDLE_VALUE
        ){
            m_error = error_type("Port #" + ss.str() + " unavailable");
            return false;
        }

        if (!GetCommState(m_handle, &m_port_settings)){
            m_error = error_type("Failed to retrieve port settings.");
            return false;
        }else{
            m_port_settings.BaudRate = m_baud_rate;
            m_port_settings.ByteSize = 8;
            m_port_settings.StopBits = ONESTOPBIT;
            m_port_settings.Parity   = NOPARITY;

             if(!SetCommState(m_handle, &m_port_settings)){
                m_error = error_type("Failed to retrieve port settings.");
                return false;
             }
        }

        return m_connected = true;
    }

    bool Serial_Port_Windows::close()
        {return !( m_connected && (m_connected = !CloseHandle(m_handle)) );}

    bool Serial_Port_Windows::change(
        count_type port_number,
        baud_rate new_baud_rate,
        size_type new_read_rate
    ){return this->close() && this->open(port_number,new_baud_rate,new_read_rate);}

    bool Serial_Port_Windows::write(byte_type b)
        {return this->write(&b, 1);}

    bool Serial_Port_Windows::read(byte_type& b)
        {return this->read(&b, 1);}

    bool Serial_Port_Windows::write(
        const byte_type* src,
        size_type src_size,
        size_type* actually_written
    ){
        DWORD writ(0);
        if(!m_connected){
            m_error = error_type("Attempt to write to unconnected port.");
            return false;
        }else if(!WriteFile(
            m_handle,
            src,
            (src_size == 0 ? m_read_rate : src_size),
            &writ,
            nullptr
        )){
            m_error = error_type("Failure sending data to port.");
            return false;
        }
        if(actually_written != nullptr)
            *actually_written = writ;
        return true;
    }

    bool Serial_Port_Windows::read(
        byte_type* dest,
        size_type dest_size,
        size_type* actually_read
    ){
        DWORD num_read(0);
        if(!m_connected){
            m_error = error_type("Attempt to read from unconnected port.");
            return false;
        }

        COMSTAT status;
        ClearCommError(m_handle, nullptr, &status);

        //Check if there is something to read
        if(status.cbInQue>0){
            if(
                !ReadFile(
                    m_handle,
                    dest,
                    std::min(
                        static_cast<size_type>(status.cbInQue),
                        (dest_size == 0 ? m_read_rate : dest_size)
                    ),
                    &num_read,
                    nullptr
                )
            ){
                m_error = error_type("Failure retrieving data from port.");
                return false;
            }
            if(actually_read != nullptr)
                *actually_read = num_read;
            return true;
        }

        if(actually_read != nullptr)
            *actually_read = 0;
        m_error = error_type("Nothing to read from port.");
        return false;
    }

    //Stream I/O
    Serial_Port_Windows& Serial_Port_Windows::operator<<(byte_type b)
        {return this->write(b), *this;}

    Serial_Port_Windows& Serial_Port_Windows::operator>>(byte_type& b)
        {return this->read(b), *this;}

    Serial_Port_Windows& Serial_Port_Windows::operator<<(str_type s)
        {return this->write(s.c_str(), s.size()), *this;}

    Serial_Port_Windows& Serial_Port_Windows::operator>>(str_type& s){
        byte_type* buf = new byte_type[m_read_rate+1];
        size_type i(m_read_rate+1);
        while(i-- > 0)
            buf[i] = 0;
        this->read(buf, m_read_rate);
        s = buf;
        delete[] buf;
        return *this;
    }

    Serial_Port_Windows::Serial_Port_Windows(
        count_type port_number,
        baud_rate new_baud_rate,
        size_type new_read_rate
    )
        : Serial_Port()
    {this->open(port_number, new_baud_rate, new_read_rate);}

    Serial_Port_Windows::~Serial_Port_Windows()
        {this->close();}
}