#include "doctor.h"


    Doctor::Doctor(){
        Specialization[0] = '\0';
        Contact[0] = '\0';
        Fee = 0;

    }


    Doctor::Doctor(int id , const char* name , const char* password , const char* specialization , const char* contact, double fee):
    Person(id, name , password) , Fee(fee)
    {
        StringCopy(Specialization , specialization);
        StringCopy(Contact , contact);
    }


    Doctor::~Doctor(){

    }

    //Getters
    const char* Doctor::getSpecialization() const {return Specialization;}
    const char* Doctor::getContact() const {return Contact;}
    double Doctor::getFee() const {return Fee;}

    //Setters

    void Doctor::setSpecialization(const char* specializatoin){StringCopy(Specialization , specializatoin);}
    void Doctor::setContact(const char* contact){StringCopy(Contact , contact);}
    void Doctor::setFee(double fee){Fee = fee;}

    //Overloading == Operator

    bool Doctor::operator==(const Doctor& other)const{
        if(ID == other.ID) return true;
        return false;
    }
    
    //Overloading << Operaor
    std::ostream& operator<<(std::ostream& out , const Doctor& other){
        out<<"Doctor's Details"<<std::endl;
        out<<"ID: "<<other.getID()<<std::endl;
        out<<"Name: "<<other.getName()<<std::endl;
        out<<"Specialization: "<<other.getSpecialization()<<std::endl;
        out<<"Fee: Rs "<<other.getFee()<<std::endl;

        return out;
    }


    //virtual Function Implementation

    void Doctor::showMenu(){
        //Implementation with SFML
    }
    void Doctor::printDetails(){
        std::cout << *this;
    }