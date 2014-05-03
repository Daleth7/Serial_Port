#include "Serial_Port_Interface.h"

Serial_Port& getline(
    Serial_Port& src,
    Serial_Port::vol_str_type& dest,
    Serial_Port::byte_type delim
){
    byte_type hold('\0');
    do{
        src.read(hold);
    }while(hold != delim && src.good());
    return src;
}