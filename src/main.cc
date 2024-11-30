#include <cstdlib>
#include <iostream>
#include <string>
#include "ptrmgr.h"

// Example structs for mixed types
struct A {
    int value;
    A(int v) : value(v) {}
};

struct B {
    double value;
    B(double v) : value(v) {}
};

struct C {
    std::string value;
    C(std::string v) : value(v) {}
};

int main() {
    MixedPointerManager manager;
    
    // Add different types of pointers to the manager
	manager.addPointer(new A(10));
	manager.addPointer(new B(3.14));
	manager.addPointer(new C("Now is the time."));

	// Access them by type
	A* aPtr = manager.getPointer<A>(0);
	if (aPtr) {
		std::cout << "A pointer value: " << aPtr->value << std::endl;
	}

	B* bPtr = manager.getPointer<B>(1);
	if (bPtr) {
		std::cout << "B pointer value: " << bPtr->value << std::endl;
	}

	C* cPtr = manager.getPointer<C>(2);
	if (cPtr) {
		std::cout << "C pointer value: " << cPtr->value << std::endl;
	}

	// Print all types stored in the pool
	manager.printTypes();

	return EXIT_SUCCESS;
}
