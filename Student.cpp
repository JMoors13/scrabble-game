#include "Student.h"

Student::Student(std::string name, std::string number, std::string email) {
    this->name = name;
    this->number = number;
    this->email = email;
}

Student::Student() {

}

Student::~Student() {
    
}

void Student::printStudent() {
    std::cout << "Name: " << this->name << std::endl;
    std::cout << "Student ID: " << this->number << std::endl;
    std::cout << "Email: " << this ->email << std::endl;
}