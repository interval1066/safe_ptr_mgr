#include <cstdlib>
#include <iostream>
#include <string>
#include "ptrmgr.h"

// Example usage:
class MyClass {
public:
    MyClass(int val) : value(val) {
        std::cout << "MyClass constructor, value: " << value << std::endl;
    }
    ~MyClass() {
        std::cout << "MyClass destructor, value: " << value << std::endl;
    }
    void display() const {
        std::cout << "Value: " << value << std::endl;
    }

private:
    int value;
};

class MyOtherClass {
public:
    MyOtherClass(std::string val) : value(val) {
        std::cout << "MyOtherClass constructor, value: " << value << std::endl;
    }
    ~MyOtherClass() {
        std::cout << "MyOtherClass destructor, value: " << value << std::endl;
    }
    void display() const {
        std::cout << "Value: " << value << std::endl;
    }

private:
    std::string value;
};

int main() {
    SafePointerManager<MyClass> manager;

    // Create new MyClass objects and add them to the manager
    manager.add(std::make_unique<MyClass>(10));
    manager.add(std::make_unique<MyClass>(20));
    manager.add(std::make_unique<MyOtherClass>("Goodthorp"));

    // Access and use the managed objects
    manager.get(0)->display();  // Output: Value: 10
    manager.get(1)->display();  // Output: Value: 20
	manager.get(2)->display(); 
    
    // Remove an object
    manager.remove(0);

    // Reset the manager (all objects are cleaned up)
    manager.reset();

    return EXIT_SUCCESS;
}

