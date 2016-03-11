#include "Serial_Port_Interface.h"

RS_232::Serial_Port& getline(
    RS_232::Serial_Port& src,
    RS_232::Serial_Port::vol_str_type& dest,
    RS_232::Serial_Port::byte_type delim
){
    RS_232::Serial_Port::byte_type hold('\0');
    do{
        src.read(hold);
    }while(hold != delim && src.good());
    return src;
}