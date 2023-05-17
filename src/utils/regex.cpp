#include <string>
#include "utils.hpp"
using namespace utils;

static bool _regex_std(const char *src, size_t buf_len, const char *regex, size_t rs, size_t *len){

    while (regex[rs])
    {
        /* code */
    }
    
    if(!memcmp(regex + rs, "(?R)", strlen("(?R)")))
    {
        size_t l = 0;
        if (rs)
            rs = 0;
    }
}


static bool _regex_0(const char *src, const char *regex, size_t *strat, size_t *len){
    
    *strat = 0;
    *len = 0;
    if(regex[0] != '^')
    {
        for (size_t i = 0; src[i]; i++)
        {
            if (_regex_std(src + i, 0, regex, 0, len))
                *strat = i;
        }
        return (!!(*len));
    }
    else
        return(_regex_std(src, 0, regex + 1, 0, len));
}

bool is_regex(const std::string, const char *){
    size_t  strat;
    size_t  len;
}

bool find_regex(const std::string, const char *){}
