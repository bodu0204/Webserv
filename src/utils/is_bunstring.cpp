#include "utils.hpp"
bool utils::is_numstring(const char * str){
    for (size_t i = 0; str[i]; i++)
    {
        if (!isnumber(str[i]))
            return (false);
    }
    return (true);
}