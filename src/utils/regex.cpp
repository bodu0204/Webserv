#include "utils.hpp"
#include <vector>

static bool set_areas(std::vector<utils::meta::area>&, utils::meta::area);
size_t _regex(const char *, const utils::meta::area &, size_t);

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

size_t _regex_std(const char *target, const utils::meta::area &search, size_t more){
	size_t ret = 0;
	size_t sl = 0;
	utils::meta::area a;

	for (;target[ret] && sl < search.len; ret++, sl += a.len)
	{
		a.start = search.start + sl;
		a.len = 0;
		if (search.start[sl] == '\\')
			a.len = 2;
		else if (search.start[sl] == '['){
			if(!utils::meta::_bracket(search.start + sl, "[]", '\\',(a.len)) || a.len < 3)
				return (0);
		}
		else if (search.start[sl] == '('){
			if(!utils::meta::_bracket(search.start + sl, "()[]", '\\',(a.len)) || a.len < 3)
				return (0);
		}
		else
			a.len = 1;
		if (search.start[sl] == '(' || memchr("?*+", search.start[sl + a.len], 3))
			break;
		if (!_regex_char(target[ret], a))
			return (0);
	}
	if (search.start[sl] == '('){
		
	}
	else if (memchr("?*+", search.start[sl + a.len], 3))
	if (sl >= search.len)
			return (ret >= more ? ret : 0);
	else if (!target[ret])
			return (0);
}
