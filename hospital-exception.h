#pragma once

class HospitalException {
    protected:
    char message[250];
    void stringCopy(char* destination , const char* source);

    public:
    HospitalException();
    HospitalException(const char* m);
    virtual ~HospitalException();


    virtual const char* what() const;
};