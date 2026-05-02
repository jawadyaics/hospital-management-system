#pragma once
#include <iostream>


class Appointment {
    private:
    int appointmentID;
    int patientID;
    int doctorID;
    char date[20];
    char time[10];
    char status[20];


    void stringCopy(char* destination , const char* source);
    bool stringCompare(const char* str1 , const char* str2);


    public:

    Appointment();
    Appointment(int aptID, int pID, int dID, const char* dt , const char* tm , const char * st);


    // Getters
    int getAppointmentID() const;
    int getPatientID() const;
    int getDoctorID() const;
    const char* getDate() const;
    const char* getTime() const;
    const char* getStatus() const;


    //Setters
    void setDate(const char* date);
    void setTime(const char* time);
    void setStatus(const char* status);


    //Operator Overloads
    bool operator== (const Appointment& other);

    friend std::ostream& operator<<(std::ostream& , const Appointment&);
};