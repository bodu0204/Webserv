#include <string>
#define IGNOR_C '\\'
#define BRACKET_f "\'\"{"
#define BRACKET_b "\'\"}"
#define BRACKET "{}"
#define IGNOR_B "\'\""

static bool _r_bracket(const char *src, int n, size_t &len){
	for (; src[len]; len++)
	{
		if (BRACKET_f[n] != BRACKET_b[n])
		{
			size_t i = 0;
			for (; BRACKET_f[i] && src[len] != BRACKET_f[i]; i++);
		}
	}
	
}

static bool _bracket(const char *src, size_t &start, size_t &len){
	start = 0;
	len = 0;
	const char *ig_b = (const char *)memchr(IGNOR_B, src[start], strlen(IGNOR_B));
	const char *brac = (const char *)memchr(BRACKET, src[start], strlen(BRACKET));
	while (src[start] && !brac)
	{
		if (ig_b)
		{
			start++;
			for (; src[start] && src[start] != *ig_b; start++)
				if(src[start] == IGNOR_C && src[start + 1])
					start++;
			if (!src[start])
			{
				start = 0;
				return (false);
			}
			start++;
			ig_b = NULL;
		}
		while (src[start] && !ig_b && !brac)
		{
			if(src[start] == IGNOR_C && src[start + 1])
				start++;
			start++;
			ig_b = (const char *)memchr(IGNOR_B, src[start], strlen(IGNOR_B));
			brac = (const char *)memchr(BRACKET, src[start], strlen(BRACKET));
		}
	}
	if (!brac)
	{
		start = 0;
		if (ig_b)
			return (false);
		return (true);
	}
	size_t i = 0;
	for (; BRACKET[i] && BRACKET[i] != *brac; i++);
	if (i % 2)
	{
		start = 0;
		return (false);
	}
	return (_r_bracket(src + (++start), *brac, len));
}

std::string in_bracket(std::string){

}
