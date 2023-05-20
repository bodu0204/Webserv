#include "config.hpp"
#include "../utils/utils.hpp"

config::config(){}//not call

config::config(std::string src):is_faile(true){
	src = utils::trim_sp(src);
	if (!src.length()) {this->is_faile = false; return ;}

}
