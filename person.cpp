#include "person.h"


int Person::StringLength(const char* str){
    int i = 0; 
    while (str[i] != '\0'){
        i++;
    }
    return i;
}

void Person::StringCopy(char* destination , const char* source){
    int i = 0;
    for( ; source[i] != '\0'; i++){
        destination[i] = source[i];
    }
    destination[i] = '\0';
}

// Constructors

Person::Person(){
    ID = 0;
    Name[1] = '\0';
    Password[1] = '\0';
}

Person::Person(int id , const char* name , const char* password){
    ID = id;
    StringCopy(Name , name);
    StringCopy(Password , password);
}

// Getters

int Person::getID() const{
    return ID;
}

const char* Person::getName() const {
    return Name;
}

const char* Person::getPassword() const {
    return Password;
}


// Setters 

void Person::setID(int id){
    ID = id;
}

void Person::setName(const char* name){
    StringCopy(Name , name);
}


void Person::setPassword(const char* password){
    StringCopy(Password , password);
}

