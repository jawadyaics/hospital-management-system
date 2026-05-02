#pragma once
#include <iostream>

class Prescription {
    private:
    int prescriptionID;
    int appointmentID;
    char medicines[255];
    char notes[255];

    void stringCopy(char* destination , const char* source);

    public:
    Prescription();
    Prescription(int pID , int aID , const char* m , const char* n);


    //Getters
    int getPrescriptionId() const;
    int getAppointmentId() const;
    const char* getMedicines() const;
    const char* getNotes() const;

    // Setters
    void setMedicines(const char* medicine);
    void setNotes(const char* notes);

    friend std::ostream& operator<<(std::ostream& , const Prescription&);
};