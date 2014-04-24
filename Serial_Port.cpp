#include "Serial_Port.h"

#include "Serial_Port_Windows.h"

namespace RS_232{
    Serial_Port* open_serial_port(
        Serial_Port::count_type pn,
        Serial_Port::baud_rate new_br,
        Serial_Port::size_type new_rr
    ){
#ifndef __LINUX__
        return new Serial_Port_Windows(pn, new_br, new_rr);
#else
        return nullptr;
#endif
    }
}