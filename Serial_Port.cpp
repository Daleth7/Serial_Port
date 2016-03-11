#include "Serial_Port.h"

#ifdef __linux__
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
#ifdef __WINDOWS__
        return new Serial_Port_Windows(pn, new_br, new_rr);
#elif defined(__linux__)
        return new Serial_Port_Linux(pn, new_br, new_rr);
#else 
        return NULL;
#endif
    }

#ifdef __linux__
    Serial_Port* open_serial_port(
        const Serial_Port::str_type& tty_suffix,
        Serial_Port::baud_rate new_br,
        Serial_Port::size_type new_rr
    ){
        return new Serial_Port_Linux(tty_suffix, new_br, new_rr);
    }
#endif
}