#include "Serial_Port.h"

#ifdef __LINUX__
    #include "Serial_Port_Windows.h"
#else
    #include "Serial_Port_Linux.h"
#endif

namespace RS_232{
    Serial_Port* open_serial_port(
        Serial_Port::count_type pn,
        Serial_Port::baud_rate new_br,
        Serial_Port::size_type new_rr
    ){
#ifndef __LINUX__
        return new Serial_Port_Windows(pn, new_br, new_rr);
#else
        return new Serial_Port_Linux(pn, new_br, new_rr);
#endif
    }

#ifdef __LINUX__
    Serial_Port* open_serial_port(
        const Serial_Port::str_type& tty_suffix,
        Serial_Port::baud_rate new_br,
        Serial_Port::size_type new_rr
    ){
        return new Serial_Port_Linux(tty_suffix, new_br, new_rr);
    }
#endif
}