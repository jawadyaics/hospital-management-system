#pragma once
#include "person.h"
#include <iostream>

class Doctor : public Person {
    private:
    char Specialization[50];
    char Contact[20];
    double Fee;

    public:

    Doctor(){};
    Doctor(int id , const char* name , const char* password , const char* specialization , const char* contact, double fee){};
    ~Doctor(){};

    //Getters
    const char* getSpecialization() const {};
    const char* getContact() const {};
    double getFee() const {};

    //Setters

    void setSpecialization(const char* specializatoin){};
    void setContact(const char* contact){};
    void setFee(double fee){};

    //Overloading == Operator

    bool operator==(const Doctor& other)const{};
    
    //Overloading << Operaor
    friend std::ostream& operator<<(std::ostream& , const Doctor&){};


    //virtual Function Implementation

    void showMenu(){};
    void printDetails() const{};
};