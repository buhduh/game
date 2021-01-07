#include <iostream>

using namespace std;

namespace foo {
	class Foo {
		public:
		void myFunc();
	};
};

void foo::Foo::myFunc() {
	cout << "namespaced foo::Foo::myFunc()" << endl;
}

int main(void) {
	foo::Foo f = foo::Foo();
	f.myFunc();
}
