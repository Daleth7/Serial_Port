#include "Serial_Port_Linux.h"

#include <sstream>

    // Linux specific headers
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

namespace RS_232{
    // Read-only
    Serial_Port_Windows::size_type Serial_Port_Windows::available() const {
        size_type toreturn;
        ioctrl(m_fd, FIONREAD, &toreturn);
        return toreturn;
    }


//Settings modifiers
    const error_type& Serial_Port_Linux::check_status(){
            // Temporary solution until a way to check port status is found
        return m_error;
    }

//C-style I/O
    bool Serial_Port_Linux::open(
        count_type new_port_number,
        baud_rate new_baud_rate,
        size_type new_read_rate
    ){
        vol_str_type tty_suffix;

        if(new_port_number < 16){
            std::stringstream ss;
            ss << new_port_number;
            tty_suffix = "S" + ss.str();
        }else if(new_port_number < 22){
            std::stringstream ss;
            ss << new_port_number-16;
            tty_suffix = "USB" + ss.str();
        }else if(new_port_number < 24){
            std::stringstream ss;
            ss << new_port_number-22;
            tty_suffix = "AMA" + ss.str();
        }else {
            std::stringstream ss;
            ss << new_port_number-24;
            tty_suffix = "ACM" + ss.str();
        }

        return this->open(tty_suffix, new_baud_rate, new_read_rate);
    }

    bool Serial_Port_Linux::open(
        const str_type& tty_suffix,
        baud_rate new_baud_rate,
        size_type new_read_rate
    ){
        if(m_connected) return true;

        m_connected = false;
        m_baud_rate = new_baud_rate;
        m_read_rate = new_read_rate;

            // Attempt to establish connection
        vol_str_type port_path = "/dev/tty" + tty_suffix;
        if((m_fd = open(port_path.c_str(), O_RDWR | O_NDELAY | O_NOCITY)) < 0){
            m_error = error_type(error_type::error_unavailable,
                "Port at " + port_path + " unavailable.");
        }

            // Save port settings for restoration in case of failure
        tcgetattr(m_fd, &m_old_settings);

            // Set some control bits
        m_port_settings.c_cflag =
              CRTSCTS   // Output hardware flow control
            | CS8       // ~ 8n1 (8bit byte, no parity, 1 stopbit
            | CLOCAL    // Local connection
            | CREAD     // Enable receiving characters
            ;

            // Set baudrate
        switch(m_baud_rate){
            case br_110:
                cfsetispeed(&m_port_settings, B110);
                cfsetospeed(&m_port_settings, B110);
                break;
            case br_300:
                cfsetispeed(&m_port_settings, B300);
                cfsetospeed(&m_port_settings, B300);
                break;
            case br_600:
                cfsetispeed(&m_port_settings, B600);
                cfsetospeed(&m_port_settings, B600);
                break;
            case br_1200:
                cfsetispeed(&m_port_settings, B1200);
                cfsetospeed(&m_port_settings, B1200);
                break;
            case br_2400:
                cfsetispeed(&m_port_settings, B2400);
                cfsetospeed(&m_port_settings, B2400);
                break;
            case br_4800:
                cfsetispeed(&m_port_settings, B4800);
                cfsetospeed(&m_port_settings, B4800);
                break;
            case br_9600:
                cfsetispeed(&m_port_settings, B9600);
                cfsetospeed(&m_port_settings, B9600);
                break;
            case br_14400:
                cfsetispeed(&m_port_settings, B14400);
                cfsetospeed(&m_port_settings, B14400);
                break;
            case br_19200:
                cfsetispeed(&m_port_settings, B19200);
                cfsetospeed(&m_port_settings, B19200);
                break;
            case br_28800:
                cfsetispeed(&m_port_settings, B28800);
                cfsetospeed(&m_port_settings, B28000);
                break;
            case br_38400:
                cfsetispeed(&m_port_settings, B38400);
                cfsetospeed(&m_port_settings, B38400);
                break;
            case br_56000:
                cfsetispeed(&m_port_settings, B56000);
                cfsetospeed(&m_port_settings, B56000);
                break;
            case br_57600:
                cfsetispeed(&m_port_settings, B57600);
                cfsetospeed(&m_port_settings, B57600);
                break;
            case br_115200:
                cfsetispeed(&m_port_settings, B15200);
                cfsetospeed(&m_port_settings, B15200);
                break;
            default:    // Terminate connection
                cfsetispeed(&m_port_settings, B0);
                cfsetospeed(&m_port_settings, B0);
                m_error = error_type(error_type::open, "Unsupported baud rate.");
                return false;
        }

            // Set input processing flags
        m_port_settings.c_iflag =
              IGNPAR    // Dump bytes with parity errors
            | ICRNL     // If '/r' is received, map to '/n'
            ;

            // Set output pre-processing flags
        m_port_settings.c_oflag = 0;    // No pre-processing

            // Set either canonical or non-canonical input
        m_port_settings.c_lflag = ICANON;

            // Flush the line and activate port settings
        tcflush(m_fd, TCIFLUSH);
        tcsetattr(m_fd, TCSANOW, &m_port_settings);

            // Finally connected.
        m_connected = true;
        return true;
    }

    bool Serial_Port_Linux::close(){
            // Terminate connection by setting 0 baud rate
        cfsetispeed(&m_old_settings, B0);
        cfsetospeed(&m_old_settings, B0);
            // Restore original port settings and apply 0 baud rate
        tcsetattr(m_fd, TCSANOW, &m_old_settings);

        m_connected = false;
    }

    bool Serial_Port_Linux::change(
        count_type port_number,
        baud_rate new_baud_rate,
        size_type new_read_rate
    ){return this->close() && this->open(port_number,new_baud_rate,new_read_rate);}

    bool Serial_Port_Linux::change(
        const str_type& tty_suffix,
        baud_rate new_baud_rate,
        size_type new_read_rate
    ){return this->close() && this->open(tty_suffix,new_baud_rate,new_read_rate);}

    bool Serial_Port_Linux::write(byte_type ch)
        {this->write(&ch, 1);}

    bool Serial_Port_Linux::read(byte_type& ch_dest)
        {this->read(&ch_dest, 1);}

    bool Serial_Port_Linux::write(
        const byte_type* buf,
        size_type number_to_write,
        size_type* actually_written
    ){
        if(!m_connected){
            m_error = error_type(error_type::error_unavailable,
                "Attempt to write to unconnected port.");
            return false;
        }

        size_type writ = write(m_fd, buf, number_to_write);
        if(actually_written != NULL)
            *actually_written = writ;
        if(writ < 0){
            int err_code = errno;   // Save error_code in case it changes due to
                                    //  following error_code
            vol_str_type err_msg;

            switch(err_code){
                case EAGAIN:
                case EWOULDBLOCK:
                    err_msg =   "File has been marked non-blocking, but"
                                " writing would block.";
                    break;
                case EBADF:
                    err_msg =   "File descriptor is invalid or is"
                                " not open for writing.";
                    break;
                case EDESTADDRREQ:
                    err_msg =   "Datagram socket has no set peer address."
                                " Use connect().";
                    break;
                case EQUOT:
                    err_msg =   "Quota of disk blocks on containing file"
                                " system exhausted.";
                    break;
                case EFAULT:
                    err_msg =   "buf's address space is inaccessible.";
                    break;
                case EFBIG:
                    err_msg =   "File size limit reached.";
                    break;
                case EINTR:
                    err_msg =   "Captured signal interrupted write.";
                    break;
                case EINVAL:
                    err_msg =   "File object is not suited for writing.";
                    break;
                case EIO:
                    err_msg =   "Low level I/O error detected.";
                    break;
                case ENOSPC:
                    err_msg =   "No room for new data.";
                    break;
                case EPIPE:
                    err_msg =   "SIGPIPE signal detected.";
                    break;
                default:
                    err_msg = "Unknown write error.";
                    break;
            }

            m_error = error_type(error_type::error_write, err_msg);
            return false;
        }
        return true;
    }

    bool Serial_Port_Linux::read(
        const byte_type* buf,
        size_type number_to_read,
        size_type* actually_read
    ){
        if(!m_connected){
            m_error = error_type(error_type::error_unavailable,
                "Attempt to read from unconnected port.");
            return false;
        }

        size_type red = read(m_fd, buf, number_to_read);
        if(actually_read != NULL)
            *actually_read = red;
        if(red == 0){
            m_error = error_type(error_type::error_read,
                "Nothing to read from port.");
            return false;
        }
        if(red < 0){
            int err_code = errno;   // Save error_code in case it changes due to
                                    //  following error_code
            vol_str_type err_msg;

            switch(err_code){
                case EAGAIN:
                case EWOULDBLOCK:
                    err_msg =   "File has been marked non-blocking, but"
                                " reading would block.";
                    break;
                case EBADF:
                    err_msg =   "File descriptor is invalid or is"
                                " not open for reading.";
                    break;
                case EFAULT:
                    err_msg =   "buf's address space is inaccessible.";
                    break;
                case EINTR:
                    err_msg =   "Captured signal interrupted write.";
                    break;
                case EINVAL:
                    err_msg =   "File object is not suited for writing.";
                    break;
                case EIO:
                    err_msg =   "Low level I/O error detected.";
                    break;
                case EISDIR:
                    err_msg =   "Attempted to write to a directory, not a file.";
                    break;
                default:
                    err_msg = "Unknown write error.";
                    break;
            }

            m_error = error_type(error_type::error_write, err_msg);
            return false;
        }
        return true;
    }

    Serial_Port& Serial_Port_Linux::getline(
        byte_type* buf,
        size_type num_to_read,
        byte_type delim
    ){
        size_type i(0);
        do{
            this->read(buf[i++]);
        }while(buf[i] != delim && this->good() && i < num_to_read);
        return *this;
    }

    Serial_Port& Serial_Port_Linux::ignore(
        size_type num_to_ignore,
        byte_type delim
    ){
        byte_type hold('\0');
        size_type i(0);
        do{
            this->read(hold);
        }while(hold != delim && this->good() && i < num_to_read);
        return *this;
    }

//Stream I/O
    Serial_Port_Linux& Serial_Port_Linux::operator<<(byte_type b)
        {return this->write(b), *this;}

    Serial_Port_Linux& Serial_Port_Linux::operator>>(byte_type& b)
        {return this->read(b), *this;}

    Serial_Port_Linux& Serial_Port_Linux::operator<<(str_type s)
        {return this->write(s.c_str(), s.size()), *this;}

    Serial_Port_Linux& Serial_Port_Linux::operator>>(str_type& s){
        byte_type* buf = new byte_type[m_read_rate+1];
        size_type i(m_read_rate+1);
        while(i-- > 0)
            buf[i] = 0;
        this->read(buf, m_read_rate);
        s = buf;
        delete[] buf;
        return *this;
    }

//Other modifiers
//Returns if flushing is successful or not
    bool Serial_Port_Linux::flush(bool output, bool force_abort)
        {return output ? flush_output(force_abort) : flush_input(force_abort);}

    bool Serial_Port_Linux::flush_input(bool){
        return tcflush(m_fd, TCIFLUSH) >= 0;
    }

    bool Serial_Port_Linux::flush_output(bool force_abort){
        if(force_abort){
            return tcflush(m_fd, TCOFLUSH) >= 0;
        } else {
            return tcdrain(m_fd) >= 0;
        }
    }

//Constructors and destructor
    Serial_Port_Linux::Serial_Port_Linux(
        count_type port_number,
        baud_rate new_br_rate,
        size_type new_read_rate
    )
        : Serial_Port(port_number, new_br_rate, new_read_rate)
    {this->open(port_number, new_baud_rate, new_read_rate);}

    Serial_Port_Linux::~Serial_Port_Linux(){this->close();}

}