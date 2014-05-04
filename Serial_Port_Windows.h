#ifndef WINDOWS_SERIAL_PORT_COMMUNICATIONS_CLASS_H___
#define WINDOWS_SERIAL_PORT_COMMUNICATIONS_CLASS_H___

#include "Serial_Port_Interface.h"

#include <windows.h>

namespace RS_232{
    class Serial_Port_Windows : public Serial_Port{
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
                    enum class code{
                        read, write, open, close, unavailable,
                        overflow, hardware,
                        unknown, os_specific,
                        none
                    };

                    code get_code()const;

                    virtual const vol_str_type& what()const;

                    error_type();
                    error_type(code c, const vol_str_type& s);
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

        using settings_type = DCB;
        using timeout_type  = COMMTIMEOUTS;
        using handle_type   = HANDLE;

        //Read-only
/*  Inherited
            bool is_connected()const;
            count_type number()const;
            const error_type& error()const;
            baud_rate baud()const;
            size_type read_rate()const;
*/

        //Settings modifiers
/*  Inherited
            void set_baud_rate(baud_rate new_rate);
            void set_read_rate(size_type bytes_to_read);
            void clear_error();
*/
            virtual const error_type& check_status() final override;

        //C-style I/O
            virtual bool open(count_type, baud_rate, size_type = 0) final override;
            virtual bool close() final override;
            virtual bool change(count_type, baud_rate, size_type = 0) final override;
            virtual bool write(byte_type) final override;
            virtual bool read(byte_type&) final override;
            virtual bool write(
                const byte_type*,
                size_type = 0,
                size_type* = nullptr //Optional request to get number of
                                    // bytes written
            ) final override;
            virtual bool read(
                byte_type*,
                size_type = 0,
                size_type* = nullptr //Optional request to get number of
                                    // bytes read
            ) final override;
            virtual Serial_Port& getline(
                byte_type* buf,
                size_type num_to_read,
                byte_type delim = '\0'
            ) final override;
            virtual Serial_Port& ignore(
                size_type num_to_ignore,
                byte_type delim = '\0'
            ) final override;

        //Stream I/O
            virtual Serial_Port_Windows& operator<<(byte_type) final override;
            virtual Serial_Port_Windows& operator>>(byte_type&) final override;
            virtual Serial_Port_Windows& operator<<(str_type) final override;
            virtual Serial_Port_Windows& operator>>(str_type&) final override;

        //Other modifiers
            //Returns if flushing is successful or not
            virtual bool flush(
                bool output = true,
                bool force_abort = false    //Terminates read or write
                                            //  operations even if they
                                            //  have not been completed.
            ) final override;
            virtual bool flush_input(bool force_abort = false) final override;
            virtual bool flush_output(bool force_abort = false) final override;

        //Constructors and destructor
            Serial_Port_Windows(count_type, baud_rate, size_type = 0);
            Serial_Port_Windows(const Serial_Port_Windows&)             = default;
            Serial_Port_Windows(Serial_Port_Windows&&)                  = default;
            Serial_Port_Windows& operator=(const Serial_Port_Windows&)  = default;
            Serial_Port_Windows& operator=(Serial_Port_Windows&&)       = default;
            virtual ~Serial_Port_Windows();

        private:
/*
    //Inherited
            bool m_connected;
            count_type m_port;
            size_type m_read_rate;
            error_type m_error;
*/
        //Port settings
        settings_type   m_port_settings;
        timeout_type    m_comm_timeout;
        handle_type     m_handle;
    };
}

#endif