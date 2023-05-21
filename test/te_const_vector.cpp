#include <vector>
int main(int argc, char *argv[]){
	std::vector<int> v;
	for (size_t i = 1; i < argc; i++)
	{
		v.push_back(atoi(argv[i]));
	}
	const std::vector<int>& vc = v;
	for (size_t i = 0; i < argc; i++)
	{
		vc[i] = 42;
	}
	return (0);
}