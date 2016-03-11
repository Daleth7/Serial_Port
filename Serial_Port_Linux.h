#ifndef Linux_SERIAL_PORT_COMMUNICATIONS_CLASS_H___
#define Linux_SERIAL_PORT_COMMUNICATIONS_CLASS_H___

#include "Serial_Port_Interface.h"

#include <termios.h>

/*
    For documentation on the Linux API for serial comminucation, refer to:
    http://man7.org/linux/man-pages/man3/termios.3.html
    http://pubs.opengroup.org/onlinepubs/007908775/xsh/fcntl.h.html

    Port number member currently unsupported
*/

namespace RS_232{
    class Serial_Port_Linux : public Serial_Port{
        public:
        //Types and aliases
/*  Inherited
            using count_type;
            using byte_type;
            using size_type;
            using str_type;
            using vol_str_type;

            class error_type{
                public:
                    enum class error_code{
                        read, write, open, close, unavailable,
                        overflow, hardware,
                        unknown, os_specific,
                        none
                    };

                    error_code get_code()const;

                    virtual const vol_str_type& what()const;

                    error_type();
                    error_type(error_code c, const vol_str_type& s);
            };

        //Use standard baud rates of a weakly-typed enumeration
        //  to allow casting to primitive types
            enum baud_rate : std::int_least32_t{
                br_110 = 110,
                br_300, = 300,
                br_600 = 600,
                br_1200 = 1200,
                br_2400 = 2400,
                br_4800 = 4800,
                br_9600 = 9600,
                br_14400 = 14400,
                br_19200 = 19200,
                br_28800 = 28800,
                br_38400 = 38400,
                br_56000 = 56000,
                br_57600 = 57600,
                br_115200 = 115200
            };
*/

        typedef termios settings_type;
        typedef int file_descriptor_type;

        //Read-only
/*  Inherited
            bool is_connected()const;
            count_type number()const;
            const error_type& error()const;
            baud_rate baud()const;
            size_type read_rate()const;
*/
            virtual size_type available() const;

        //Settings modifiers
/*  Inherited
            void set_baud_rate(baud_rate new_rate);
            void set_read_rate(size_type bytes_to_read);
            void clear_error();
*/
            virtual const error_type& check_status();

        //C-style I/O
            virtual bool open(
                count_type,
                baud_rate,
                size_type = 0
            );
            virtual bool open(
                const str_type& tty_suffix, // Example: "USB0"
                baud_rate,
                size_type = 0
            );
            virtual bool close();
            virtual bool change(
                count_type,
                baud_rate,
                size_type = 0
            );
            virtual bool change(
                const str_type& tty_suffix, // Example: "USB0"
                baud_rate,
                size_type = 0
            );
            virtual bool write(byte_type);
            virtual bool read(byte_type&);
            virtual bool write(
                const byte_type*,
                size_type = 0,
                size_type* = NULL //Optional request to get number of
                                    // bytes written
            );
            virtual bool read(
                byte_type*,
                size_type = 0,
                size_type* = NULL //Optional request to get number of
                                    // bytes read
            );
            virtual Serial_Port& getline(
                byte_type* buf,
                size_type num_to_read,
                byte_type delim = '\0'
            );
            virtual Serial_Port& ignore(
                size_type num_to_ignore,
                byte_type delim = '\0'
            );

        //Stream I/O
            virtual Serial_Port_Linux& operator<<(byte_type);
            virtual Serial_Port_Linux& operator>>(byte_type&);
            virtual Serial_Port_Linux& operator<<(str_type);
            virtual Serial_Port_Linux& operator>>(str_type&);

        //Other modifiers
            //Returns if flushing is successful or not
            virtual bool flush(
                bool output = true,
                bool force_abort = false    //Terminates read or write
                                            //  operations even if they
                                            //  have not been completed.
            );
            virtual bool flush_input(bool force_abort = false);
            virtual bool flush_output(bool force_abort = false);

        //Constructors and destructor
            Serial_Port_Linux(count_type, baud_rate = br_9600, size_type = 0);
            Serial_Port_Linux(const Serial_Port_Linux& e)
                : Serial_Port(e.m_port, e.m_baud_rate, e.m_read_rate)
                , m_port_settings(e.m_port_settings)
                , m_old_settings(e.m_old_settings)
                , m_fd(e.m_fd)
            {}
            Serial_Port_Linux& operator=(const Serial_Port_Linux& sp2){
                this->m_connected = sp2.m_connected;
                this->m_port = sp2.m_port;
                this->m_baud_rate = sp2.m_baud_rate;
                this->m_read_rate = sp2.m_read_rate;
                this->m_error = sp2.m_error;
                this->m_port_settings = sp2.m_port_settings;
                this->m_old_settings = sp2.m_old_settings;
                this->m_fd = sp2.m_fd;
                return *this;
            }
            virtual ~Serial_Port_Linux();

        private:
/*
    //Inherited
            bool m_connected;
            count_type m_port;
            size_type m_baud_rate;
            size_type m_read_rate;
            error_type m_error;
*/
        //Port settings
        settings_type           m_port_settings, m_old_settings;
        file_descriptor_type    m_fd;
    };
}

#endif