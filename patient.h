#pragma once
#include "person.h"


class Patient : public Person {
    private:
    int Age;
    char Gender[20];
    char Contact[20];
    double Balance;

    public:
    Patient();
    Patient(int id ,const char* name ,const char* password ,int age ,const char* gender ,const char* contact, double balance);

    ~Patient();

    // getter

    int getAge() const;

    const char* getGender() const;

    const char* getContact() const;

    double getBalance() const ;


    // Setters 

    void setAge(int age) ;

    void setGender(const char* gender);

    void setContact(const char* contact);

    void setBalance(double balance);


    void addFunds(double amount);

    void deductFunds(double abount);

    // Virtual Functions

    void showMenu();
    void printDetails();
};