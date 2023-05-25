class base
{
private:
	virtual void _hello() = 0;
public:
	void hello();
};

class some :public base
{
private:
	void _hello();
};

void base::hello(){
	this->_hello();
}

#include <iostream>
void some::_hello(){
	std::cout<< "hello world"<<std::endl;
}

int main(){
	some s;
	base *b = &s;
	b->hello();
}