class some
{
private:
	int number;
	some();
public:
	some(int);
	~some();
	int get_number() const;
	static const some var;
};
const some some::var;

some::some():number(42){}

some::some(int i):number(i){}

int some::get_number() const{return (this->number);}

some::~some()
{}

#include <iostream>

int main (){
	some i(99);

	std::cout<< i.get_number() << std::endl;
	std::cout<< some::var.get_number() << std::endl;
}