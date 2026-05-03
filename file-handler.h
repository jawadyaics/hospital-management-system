#pragma once

#include "storage.h"
#include "patient.h"
#include "doctor.h"
#include "admin.h"
#include "appointment.h"
#include "bill.h"
#include "prescription.h"
#include "file-not-found-exception.h"


class FileHandler {
    public:
    //Loades Objects in Memory
    void loadPatients(Storage<Patient>& patientDB);
    void loadDoctors(Storage<Doctor>& doctorsDB);
    void loadAdmins(Storage<Admin>& adminDB);
    void loadAppointments(Storage<Appointment>& appointmentDB);
    void loadPrescriptions(Storage<Prescription>& presccriptionDB);
    void loadBills(Storage<Bill>& billDB);


    //Appends new doctors or patients
    void appendPatient(const Patient& p);
    void appendDoctor(const Doctor& d);
    void appendAdmin(const Admin& adm);
    void appendAppointment(const Appointment& apt);
    void appendPrescription(const Prescription& pres);
    void appendBill(const Bill& bil);


    //Updaters
    void updateAppointment(const Patient& updatePatient);
    void updateAppointment(const Doctor& updateDoctor);
    void updateAdmin(const Admin& updateAdmin);
    void updateAppointment(const Appointment& updateAppointment);
    void updatePrescription(const Prescription& updatePrescription);
    void updateBill(const Bill& updateBill);


    //Deleters
    void deletePatient(int patientID);
    void deleteDoctor(int doctorID);
    void deleteAdmin(int adminID);
    void deleteAppointment(int appointmentID);
    void deletePrescription(int prescriptionID);
    void deleteBill(int BillID);
};

