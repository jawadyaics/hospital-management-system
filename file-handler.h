#pragma once

#include "storage.h"
#include "patient.h"
#include "doctor.h"
#include "admin.h"
//#include "appointment.h"
//#include "bill.h"
//#include "prescription.h"

//#include "FileNotFountException.h"


class FileHandler {
    public:
    //Loades Objects in Memory
    void loadPatients(Storage<Patient>& patientDB){};
    void loadDoctors(Storage<Doctor>& doctorsDB){};
    void loadAdmins(Storage<Admin>& adminDB){};
    //void loadAppointments(Storage<Appointment>& appointmentDB){};


    //Appends new doctors or patients
    void appendPatient(const Patient& p){};
    void appendDoctor(const Doctor& d){};

    //Updaters
    void updateDoctor(const Patient& updatePatient){};
    void updateDoctor(const Doctor& updateDoctor){};


    //Deleters
    void deletePatient(int patientID){};
    void deleteDoctor(int doctorID){};
};

