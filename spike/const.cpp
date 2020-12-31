#include <iostream>
#include <vector>

class Foo {
	public:
	const int& getFoo() const {return m_foo;};
	//int& getFoo() const {return m_foo;};
	const std::vector<int>& getBar() const {return m_bar;};
	private:
	int m_foo = 42;
	std::vector<int> m_bar = std::vector<int>(42);
};

int main(void) {
	//int foo = 42;
	//int baz = 345;
	//int *const bar = &foo;
	/*bar = 345;*/
	//const_cast
	//const int *bar = &foo;
	//bar = &baz;
	Foo foo = Foo();
	auto huh = foo.getFoo();
	huh += 1;
	//foo.getFoo() += 1;
	//std::cout << "huh? " << huh << std::endl;
	//std::cout << "huh? " << foo.getFoo() << std::endl;
	auto bar = foo.getBar();
	//bar = std::vector<int>(34534);
	bar.push_back(23);
	int i = 0;
	int &j = i;
	j = 3;
	std::cout << "i: " << i << std::endl;
}
