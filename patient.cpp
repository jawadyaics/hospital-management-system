#include "patient.h"

    Patient::Patient(){
        Age = 0;
        Gender[1] = '\0';
        Contact[1] = '\0';
        Balance = 0;
    }


    Patient::Patient(int id ,const char* name ,const char* password ,int age ,const char* gender ,const char* contact, double balance)
    : Person(id , name , password), Age(age), Balance(balance)
    {
        StringCopy(Gender, gender);
        StringCopy(Contact, contact);
    }

    Patient::~Patient(){
        
    }
    // getter

    int Patient::getAge() const {
        return Age;
    }

    const char* Patient::getGender() const{
        return Gender;
    }

    const char* Patient::getContact() const{
        return Contact;
    }

    double Patient::getBalance() const {
        return Balance;
    }


    // Setters 

    void Patient::setAge(int age) {
        Age = age;
    }

    void Patient::setGender(const char* gender){
        StringCopy(Gender , gender);
    }

    void Patient::setContact(const char* contact){
        StringCopy(Contact, contact);
    }

    void Patient::setBalance(double balance){
        Balance = balance;
    }


    void Patient::addFunds(double amount){
        if (amount > 0) Balance+=amount;
    }

    void Patient::deductFunds(double amount){
        Balance -= amount;
    }

    void Patient::showMenu(){

    }

    void Patient::printDetails() {

    }