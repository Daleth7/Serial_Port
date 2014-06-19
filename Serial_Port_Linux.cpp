#include "Serial_Port_Linux.h"

#include <sstream>

#include <fcntl.h>

namespace RS_232{

//Settings modifiers
    const error_type& Serial_Port_Linux::check_status(){}

//C-style I/O
    bool Serial_Port_Linux::open(
        count_type new_port_number,
        baud_rate new_baud_rate,
        size_type new_read_rate
    ){
        if(m_port == new_port_number && m_connected)
            return true;

        m_connected = false;
        m_baud_rate = new_baud_rate;
        m_read_rate = new_read_rate;

        vol_str_type port_path;

        if(new_port_number < 16){
            std::stringstream ss;
            ss << new_port_number;
            port_path = "/dev/ttyS" + ss.str();
        }else if(new_port_number < 22){
            std::stringstream ss;
            ss << new_port_number-22;
            port_path = "/dev/ttyUSB" + ss.str();
        }else if(new_port_number < 24){
            std::stringstream ss;
            ss << new_port_number-24;
            port_path = "/dev/ttyAMA" + ss.str();
        }else if(new_port_number < 26){
            std::stringstream ss;
            ss << new_port_number-26;
            port_path = "/dev/ttyACM" + ss.str();
        }else if(new_port_number < 28){
            std::stringstream ss;
            ss << new_port_number-28;
            port_path = "/dev/rfcomm" + ss.str();
        }else{
            std::stringstream ss;
            ss << new_port_number-30;
            port_path = "/dev/ircomm" + ss.str();
        }

        if(-1 == open(port_path.c_str(), O_RDWR | O_NDELAY | O_NOCITY)){
            m_error = error_type(error_type::code::unavailable,
                "Port at " + port_path + " unavailable.");
        }

        m_port = new_port_number;
        return true;
    }

    bool Serial_Port_Linux::close(){}

    bool change(
        count_type,
        baud_rate,
        size_type = 0
    ){}

    bool Serial_Port_Linux::write(byte_type){}

    bool Serial_Port_Linux::read(byte_type&){}

    bool Serial_Port_Linux::write(
        const byte_type*,
        size_type = 0,
        size_type* = nullptr //Optional request to get number of
                            // bytes written
    ){}

    bool Serial_Port_Linux::read(
        byte_type*,
        size_type = 0,
        size_type* = nullptr //Optional request to get number of
                            // bytes read
    ){}

    Serial_Port& Serial_Port_Linux::getline(
        byte_type* buf,
        size_type num_to_read,
        byte_type delim = '\0'
    ){}
    Serial_Port& Serial_Port_Linux::ignore(
        size_type num_to_ignore,
        byte_type delim = '\0'
    ){}

//Stream I/O
    Serial_Port_Linux& Serial_Port_Linux::operator<<(byte_type){}

    Serial_Port_Linux& Serial_Port_Linux::operator>>(byte_type&){}

    Serial_Port_Linux& Serial_Port_Linux::operator<<(str_type){}

    Serial_Port_Linux& Serial_Port_Linux::operator>>(str_type&){}

//Other modifiers
//Returns if flushing is successful or not
    bool Serial_Port_Linux::flush(
        bool output = true,
        bool force_abort = false    //Terminates read or write
                                    //  operations even if they
                                    //  have not been completed.
    ){}

    bool Serial_Port_Linux::flush_input(bool force_abort = false){}

    bool Serial_Port_Linux::flush_output(bool force_abort = false){}

//Constructors and destructor
    Serial_Port_Linux::Serial_Port_Linux(
        count_type port_number,
        baud_rate new_br_rate,
        size_type new_read_rate
    )

}