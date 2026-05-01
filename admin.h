#pragma once
#include "person.h"

class Admin : public Person {
    public:

    Admin(){};

    Admin(int id , const char* name , const char* password){};
    ~Admin(){};

    void showMenu(){};
    void printDetails(){};

}