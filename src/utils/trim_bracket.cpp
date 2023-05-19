#include <string>

static inline ssize_t strfind(const char *str, char c){
	size_t i = 0;
	for (; str[i] && str[i] != c; i++);
	if (str[i] && str[i] == c)
		return (i);
	return (-1);
}

static bool _bracket(const char *src, const char *brac, char ignor, size_t &len){
	bool flat = true;
	if (src[0] == brac[0]){
		len++;
		flat = false;
	}
	while (1){
		ssize_t b = strfind(brac, src[len]);
		while (src[len] && src[len] != brac[1] && b < 0){
			if(src[len] == ignor){
				if (src[len + 1])
					len++;
				else
					return (false);
			}
			len++;
			b = strfind(brac, src[len]);
		}
		if (src[len] == brac[1]){
			len++;
			return (true);
		}else if (!src[len] || b % 2)
			return (false);
		if(flat && !b)
			return (true);
		bool r = _bracket(src, brac + b, ignor, len);
		if (!r)
			return (false);
	}
	return (false);
}

std::string in_bracket(std::string &str){
	size_t len = 0;
	bool r = _bracket(str.c_str(), "{}''", '\\', len);
	if (r){
		std::string ret;
		if (str[0] == '{')
			ret = str.substr(1,len - 2);
		else
			ret = str.substr(0, len);
		str = str.substr(len, str.length() - len);
		return(ret);
	}
	return(std::string(""));
}

//#include<iostream>
//int main (int argc, char *argv[]){
//	if (argc != 2){
//		std::cerr << "arg error" << std::endl;
//		return(1);
//	}
//	std::string input = argv[1];
//	std::cout << "input>" << input << "$"<< std::endl;
//	std::cout << "in_bracket>" << in_bracket(input) << "$"<< std::endl;
//	std::cout << "input>" << input << "$"<< std::endl;
//	return (0);
//}
/* 
blyu@xiaowendeiMac utils % ./a.out "{{}}aaaaa"
input>{{}}aaaaa$
in_bracket>{}$
input>aaaaa$
blyu@xiaowendeiMac utils % ./a.out "{a{\'}aa}aa" 
input>{a{\'}aa}aa$
in_bracket>a{\'}aa$
input>aa$
blyu@xiaowendeiMac utils % ./a.out "{{'}'}aaaaa"
input>{{'}'}aaaaa$
in_bracket>$
input>{{'}'}aaaaa$
blyu@xiaowendeiMac utils % ./a.out "{a{\'}a}a}aa"
input>{a{\'}a}a}aa$
in_bracket>a{\'}a$
input>a}aa$
 */