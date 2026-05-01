#pragma once

class Person {
    protected:
    int ID;
    char Name[50];
    char Password[5];

    int StringLength(const char* str){};
    void StringCopy(char* destination , const char* source){};


    public:
    Person();
    Person(int id , char *name , char* password){};
    virtual ~Person();


    // Getters Declaration

    int getID() const {};
    const char* getName() const {};
    const char* getPassword() const {};

    // Setters 

    void setID(int id) {};
    void setName(const char* name) {};
    void setPassword(const char* password) {};


    // Virtual Functions

    virtual void showMenu() = 0;

    virtual void printDetails() = 0;

};