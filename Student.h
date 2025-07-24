# include <iostream>

#ifndef ASSIGN2_STUDENT_H
#define ASSIGN2_STUDENT_H

class Student {
    public:
        Student(std::string name, std::string number, std::string email);
        Student();
        ~Student();

        void printStudent();

    private:
        std::string name;
        std::string number;
        std::string email;

};

#endif // ASSIGN2_STUDENT_H