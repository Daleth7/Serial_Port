#ifndef SERIAL_PORT_COMMUNICATIONS_CLASS__H__
#define SERIAL_PORT_COMMUNICATIONS_CLASS__H__

#include "Serial_Port_Interface.h"

namespace RS_232{
    Serial_Port* open_serial_port(
        Serial_Port::count_type port_number = -1,
        baud_rate new_baud_rate = br_110,
        Serial_Port::size_type new_read_rate = 0
    );
#ifdef __LINUX__
    Serial_Port* open_serial_port(
        const Serial_Port::str_type& tty_suffix,
        baud_rate new_baud_rate = br_110,
        Serial_Port::size_type new_read_rate = 0
    );
#endif
}

#endif