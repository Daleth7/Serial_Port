#ifndef SERIAL_PORT_COMMUNICATIONS_CLASS__H__
#define SERIAL_PORT_COMMUNICATIONS_CLASS__H__

#include "Serial_Port_Interface.h"

namespace RS_232{
    Serial_Port* open_serial_port(
        Serial_Port::count_type port_number = -1,
        Serial_Port::baud_rate new_baud_rate = Serial_Port::br_110,
        Serial_Port::size_type new_read_rate = 0
    );
}

#endif