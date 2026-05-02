#include "admin.h"
#include <iostream>


Admin::Admin(){
        ID = 0;
        Name[0] = '\0';
        Password[0] = '\0';
    }

    Admin::Admin(int id , const char* name , const char* password): Person(id , name , password){};
    Admin::~Admin(){}

    void Admin::showMenu(){}
    void Admin::printDetails(){
        std::cout<<"Admin Details"<<std::endl;
        std::cout<<"ID: "<<getID()<<std::endl;
        std::cout<<"Name: "<<getName()<<std::endl;
        std::cout<<"Role: System Administrator"<<std::endl;
    }
