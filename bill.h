#pragma once
#include <iostream>

class Bill {
    private:
    int billID;
    int appointmentID;
    int patientID;
    double amount;
    char status[20];

    void stringCopy(char* destination , const char* source);
    public:
    Bill();
    Bill(int bID , int aID , int pID , double a , const char* s);


        // Getters
    int getBillID() const; 
    int getAppointmentID() const;
    int getPatientID() const;
    double getAmount() const;
    const char* getStatus() const;

        // Setters
    void setStatus(const char* status);
    void setAmount(double amount);


    friend std::ostream& operator<<(std::ostream&, const Bill&);


};