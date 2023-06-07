#include "utils.hpp"
#include <vector>
#include <stack>

static bool set_areas(std::vector<utils::meta::area>&, utils::meta::area);
size_t _regex(const char *, const utils::meta::area &, size_t);
bool _regex_char(char, const utils::meta::area &);

size_t _regex_pipe(const char *target, const utils::meta::area &search, size_t more){
	std::vector<utils::meta::area> areas;
	if (set_areas(areas, search))
		return (0);
	size_t ret = 0;
	for (std::vector<utils::meta::area>::iterator i = areas.begin(); i != areas.end(); i++)
	{
		size_t l = _regex_std(target, *i, more);
		if (l && (!ret || l < ret))
			ret = l;
	}
	return(ret);
}

size_t _regex_std(const char *target, const utils::meta::area &search, size_t more/* 意味を決める */){
	size_t ret = 0;
	size_t sl = 0;
	utils::meta::area a;
	char c;

	for (;target[ret] && sl < search.len; ret++, sl += a.len)
	{
		a.start = search.start + sl;
		a.len = 0;
		if (search.start[sl] == '\\')
			a.len = 2;
		else if (search.start[sl] == '['){
			if(!utils::meta::_bracket(search.start + sl, "[]", '\\',(a.len)) || a.len < 3)
				return (0);
		}else if (search.start[sl] == '('){
			if(!utils::meta::_bracket(search.start + sl, "()[]", '\\',(a.len)) || a.len < 3)
				return (0);
		}
		else
			a.len = 1;
		c = search.start[sl + a.len];
		if (search.start[sl] == '(' || memchr("?*+", c, 3))
			break;
		if (!_regex_char(target[ret], a))
			return (0);
	}
	utils::meta::area b;
	b.start = search.start + sl + a.len;
	b.len = search.len - sl - a.len;
	if (search.start[sl] == '('){
		std::stack<size_t>s;
		std::vector<size_t>r;
		size_t l;
		switch (c)
		{
		case '*':
		case '?':
			if (!b.len){
				if (more <= ret)
					r.push_back(ret);
			}else{
				l = _regex_std(target + ret, b, more > ret ? more - ret : 0);
				if (l)
					r.push_back(ret + l);
			}
		case '+':
		default :
			a.len -= 2;
			a.start++;
			s.push(0);
			size_t lb = 0;
			do{
				s.top() = _regex_pipe(target + ret + lb, a, s.top() + 1);
				if (s.top()){
					if (!b.len){
						if (more <= ret + lb + s.top())
							r.push_back(ret + lb + s.top());
					}else{
						l = _regex_std(target + ret + lb + s.top(), b, more > ret + lb + s.top() ? more - ret - lb - s.top() : 0);
						if (l)
							r.push_back(ret + lb + s.top() + l);
					}
				}
				if (s.top() && memchr("*+", c, 2)){
					lb += s.top();
					s.push(0);
				}
				else if (!s.top()){
					s.pop();
					if (!s.size())
						break;
					lb -= s.top();
				}
			} while (true);
			break;
		}
		if (!r.size())
			return (0);
		ret = *r.begin();
		for (std::vector<size_t>::iterator i = r.begin(); i != r.end(); i++){
			if (*i < ret)
				ret = *i;
		}
		return (ret);
	}else if (memchr("?*+", c, 3)){
		std::vector<size_t>r;
		size_t l = 0;
		switch (c)
		{
		case '?':
		case '*':
			if (!b.len){
				if (more <= ret)
					r.push_back(ret);
			}else{
				l = _regex_std(target + ret, b, more > ret ? more - ret : 0);
				if (l)
					r.push_back(ret + l);
			}
		case '+':
			size_t x = _regex_char(target[ret], a);
			if (!x)
				r.push_back(0);
			if (!b.len){
				if (more <= ret + x)
					r.push_back(ret + x);
			}else{
				l = _regex_std(target + ret + x, b, more > ret + x ? more - ret - x : 0);
				if (l)
					r.push_back(ret + x + l);
			}
			if (c == '?')
				break;
			while (_regex_char(target[ret + x], a)){
				x++;
				if (!b.len){
					if (more <= ret + x)
						r.push_back(ret + x);
				}else{
					l = _regex_std(target + ret + x, b, more > ret + x ? more - ret - x : 0);
					if (l)
						r.push_back(ret + x + l);
				}
			}
			break;
		}
		if (!r.size())
			return (0);
		ret = *r.begin();
		for (std::vector<size_t>::iterator i = r.begin(); i != r.end(); i++){
			if (*i < ret)
				ret = *i;
		}
		return (ret);
	}else if (sl >= search.len)
		return (ret >= more ? ret : 0);
	else
		return (0);
}
