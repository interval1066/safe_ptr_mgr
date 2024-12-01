#include <iostream>
#include <string>
#include <tuple>
#include "ptrmgr.h"

using namespace managed;

// Define an aggregate struct
struct Person {
    std::string name;
    int age;
    double height;

    // Custom output for demonstration
    void print() const {
        std::cout << "Person{name: " << name << ", age: " << age << ", height: " << height << "}\n";
    }
};

int main() {
    MixedPointerManager manager;

    // Add individual types
    manager.add<int>(42);
    manager.add<std::string>("Hello, World!");
    manager.add<double>(3.14159);

    // Add structs of aggregate types
    //manager.add<Person>("John Doe", 30, 5.9);
	manager.add<Person>(Person{"John Doe", 30, 5.9});
    //manager.add<Person>("Jane Smith", 28, 5.7);

    // Access and use individual objects
    try {
        auto* intObj = manager.get<int>(0);
        auto* strObj = manager.get<std::string>(1);
        auto* doubleObj = manager.get<double>(2);

        std::cout << "Integer: " << *intObj << "\n";
        std::cout << "String: " << *strObj << "\n";
        std::cout << "Double: " << *doubleObj << "\n";

        // Access and use structs
        auto* person1 = manager.get<Person>(3);
        //auto* person2 = manager.get<Person>(4);

        std::cout << "First Person: ";
        person1->print();
        //std::cout << "Second Person: ";
        //person2->print();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }

    return 0;
}

