#include "file-handler.h"
#include <fstream>
#include <ctime>

void FileHandler::logSecurityEvent(const char* role, int id, const char* result) {
    std::ofstream file("security_log.txt", std::ios::app);
    if (file.is_open()) {
        time_t now = time(0);
        char* dt = ctime(&now);
        int i = 0;
        while(dt[i] != '\n' && dt[i] != '\0') i++;
        dt[i] = '\0';
        file << "[" << dt << "] Role: " << role << ", ID: " << id << ", Result: " << result << "\n";
        file.close();
    }
}

// Loaders Implementation

void FileHandler::loadPatients(Storage<Patient>& patientDB){
    std::ifstream file("patients.txt");

    if(!file.is_open()){
        throw FileNotFoundException("Error: patient.txt could not be found on Startup");
    }

    int id;
    char name[50];
    char password[50];
    int age;
    char gender[20];
    char contact[20];
    double balance;

    while(file>> id >> name >> password >> age >> gender >> contact >> balance){
        Patient p(id , name , password , age , gender , contact , balance);
        patientDB.add(p);
    }
    file.close();
}



void FileHandler::loadDoctors(Storage<Doctor>& doctorsDB)
{
    std::ifstream file("doctors.txt");

    if(!file.is_open()){
        throw FileNotFoundException("Error: doctors.txt could not be found on Startup");
    }

    int id;
    char name[50];
    char password[50];
    char specialization[50];
    char contact[20];
    double fee;

    while(file>> id >> name >> password >> specialization >> contact >> fee ){
        Doctor d (id , name , password , specialization , contact , fee);
        doctorsDB.add(d);
    }
    file.close();
}


void FileHandler::loadAdmins(Storage<Admin>& adminDB){
    std::ifstream file("admins.txt");

    if(!file.is_open()){
        throw FileNotFoundException("Error: admins.txt could not be found on startup");
    }

    int id;
    char name[50];
    char password[50];

    while(file>> id >> name >>password){
        Admin a(id , name , password);
        adminDB.add(a);
    }

    file.close();
}

void FileHandler::loadAppointments(Storage<Appointment>& appointmentDB){
    std::ifstream fileIn("appointments.txt");
    if(!fileIn.is_open()){
        throw FileNotFoundException("Error: appointments.txt can't be located in loadAppointments");
    }

    int appointmentID , patientID , doctorID;
    char date[20] , time[10] , status[20];

    while(fileIn>> appointmentID >> patientID >> doctorID >> date >> time >> status ){
        Appointment a(appointmentID , patientID , doctorID , date , time , status);
        appointmentDB.add(a);
    }

    fileIn.close();
}


void FileHandler::loadPrescriptions(Storage<Prescription>& prescriptionDB){
    std::ifstream fileIn("prescriptions.txt");
    if(!fileIn.is_open()){
        throw FileNotFoundException("Error: prescriptions.txt can't be located in loadPrescriptions");
    }

    int prescriptionID;
    int appointmentID;
    char medicines[255];
    char notes[255];

    while(fileIn>> prescriptionID >> appointmentID >> medicines >> notes){
        Prescription p(prescriptionID , appointmentID , medicines , notes);
        prescriptionDB.add(p);
    }

    fileIn.close();
}


void FileHandler::loadBills(Storage<Bill>& billDB){
    std::ifstream fileIn("bills.txt");
    if(!fileIn.is_open()){
        throw FileNotFoundException("Error: bills.txt can't be located in loadBills");
    }

    int billID;
    int appointmentID;
    int patientID;
    double amount;
    char status[20];

    while(fileIn>> billID >> appointmentID >> patientID >> amount >> status){
        Bill b(billID , appointmentID , patientID , amount , status);
        billDB.add(b);
    }

    fileIn.close();
}



// Appending


void FileHandler::appendPatient(const Patient& p){
    std::ofstream file("patients.txt" , std::ios::app);
    if(!file.is_open()){
        throw FileNotFoundException("Error: patients.txt could not be found on Append");
    }

    file<<p.getID() <<" "
    <<p.getName()<<" "
    <<p.getPassword()<<" "
    <<p.getAge()<<" "
    <<p.getGender()<<" "
    <<p.getContact()<<" "
    <<p.getBalance()<<'\n';

    file.close();
}



void FileHandler::appendDoctor(const Doctor& d){
    std::ofstream file("doctors.txt" , std::ios::app);
    if(!file.is_open()){
        throw FileNotFoundException("Error: doctors.txt could not be found on Append");
    }

    file<<d.getID()<<" "
    <<d.getName()<<" "
    <<d.getPassword()<<" "
    <<d.getSpecialization()<<" "
    <<d.getContact()<<" "
    <<d.getFee()<<"\n";
    file.close();

}


void FileHandler::appendAdmin(const Admin& adm){
    std::ofstream file("admins.txt" , std::ios::app);
    if(!file.is_open()){
        throw FileNotFoundException("Error: admins.txt could not be found on appendAdmin");
    }

    file<<adm.getID()<<" "
    <<adm.getName()<<" "
    <<adm.getPassword()<<"\n";

    file.close();  
}


void FileHandler::appendAppointment(const Appointment& apt){
    std::ofstream file("appointments.txt" , std::ios::app);
    if(!file.is_open()){
        throw FileNotFoundException("Error: appointments.txt could not be found on appendAppointment");
    }

    file<<apt.getAppointmentID()<<" "
    <<apt.getPatientID()<<" "
    <<apt.getDoctorID()<<" "
    <<apt.getDate()<<" "
    <<apt.getTime()<<" "
    <<apt.getStatus()<<"\n";

    file.close();   
}


void FileHandler::appendPrescription(const Prescription& pres){
    std::ofstream file("prescriptions.txt" , std::ios::app);
    if(!file.is_open()){
        throw FileNotFoundException("Error: prescriptions.txt could not be found on appendPrescription");
    }

    file<<pres.getPrescriptionId()<<" "
    <<pres.getAppointmentId()<<" "
    <<pres.getMedicines()<<" "
    <<pres.getNotes()<<"\n";

    file.close(); 
}

void FileHandler::appendBill(const Bill& bil){
    std::ofstream file("bills.txt" , std::ios::app);
    if(!file.is_open()){
        throw FileNotFoundException("Error: bills.txt could not be found on appendBills");
    }

    file<<bil.getBillID()<<" "
    <<bil.getAppointmentID()<<" "
    <<bil.getPatientID()<<" "
    <<bil.getAmount()<<" "
    <<bil.getStatus()<<"\n";

    file.close();
}


    //Updaters
void FileHandler::updatePatient(const Patient& updatePatient){
    std::ifstream fileIn("patients.txt");
    std::ofstream fileOut("temp.txt");

    if(!fileIn.is_open()){
        throw FileNotFoundException("Error: patients.txt could not be found on updatePatient");
    }

    int id;
    char name[50];
    char password[50];
    int age;
    char gender[20];
    char contact[20];
    double balance;

    while(fileIn>> id >> name >> password >> age >> gender >> contact >> balance){
        if(id == updatePatient.getID()){
            fileOut<<updatePatient.getID() <<" "
            <<updatePatient.getName()<<" "
            <<updatePatient.getPassword()<<" "
            <<updatePatient.getAge()<<" "
            <<updatePatient.getGender()<<" "
            <<updatePatient.getContact()<<" "
            <<updatePatient.getBalance()<<'\n';
        }

        else{
            fileOut<<id<<" "<<name<<" "<<password<<" "<<age<<" "<<gender<<" "<<contact<<" "<<balance<<"\n";

        }

    }
    fileIn.close();
    fileOut.close();


    remove("patients.txt");
    rename("temp.txt", "patients.txt");

}



void FileHandler::updateDoctor(const Doctor& updateDoctor){
    std::ifstream fileIn("doctors.txt");
    std::ofstream fileOut("temp.txt");

    if(!fileIn.is_open()){
        throw FileNotFoundException("Error: doctors.txt could not be found on updateDoctor");
    }



    int id;
    char name[50];
    char password[50];
    char specialization[50];
    char contact[20];
    double fee;

    while(fileIn>> id >> name >> password >> specialization >> contact >> fee){
        if(id == updateDoctor.getID()){
            fileOut<<updateDoctor.getID() <<" "
            <<updateDoctor.getName()<<" "
            <<updateDoctor.getPassword()<<" "
            <<updateDoctor.getSpecialization()<<" "
            <<updateDoctor.getContact()<<" "
            <<updateDoctor.getFee()<<"\n";
        }

        else{
            fileOut<<id<<" "<<name<<" "<<password<<" "<<specialization<<" "<<contact<<" "<<fee<<" "<<"\n";

        }

    }
    fileIn.close();
    fileOut.close();


    remove("doctors.txt");
    rename("temp.txt", "doctors.txt");
}


void FileHandler::updateAdmin(const Admin& updateAdmin){
    std::ifstream fileIn("admins.txt");
    std::ofstream fileOut("temp.txt");

    if(!fileIn.is_open()){
        throw FileNotFoundException("Error: admins.txt could not be found on updateAdmin");
    }

    int id;
    char name[50];
    char password[50];


    while(fileIn>> id >> name >> password ){
        if(id == updateAdmin.getID()){
            fileOut<<updateAdmin.getID() <<" "
            <<updateAdmin.getName()<<" "
            <<updateAdmin.getPassword()<<"\n";
        }

        else{
            fileOut<<id<<" "<<name<<" "<<password<<"\n";

        }

    }
    fileIn.close();
    fileOut.close();


    remove("admins.txt");
    rename("temp.txt", "admins.txt");


}


void FileHandler::updateAppointment(const Appointment& updateAppointment){
    std::ifstream fileIn("appointments.txt");
    std::ofstream fileOut("temp.txt");

    if(!fileIn.is_open()){
        throw FileNotFoundException("Error: appointments.txt could not be found on updateAppointments");
    }



    int appointmentID;
    int patientID;
    int doctorID;
    char date[20];
    char time[10];
    char status[20];

    while(fileIn>> appointmentID >> patientID >> doctorID >> date >> time >> status){
        if(appointmentID == updateAppointment.getAppointmentID()){
            fileOut<<updateAppointment.getAppointmentID() <<" "
            <<updateAppointment.getPatientID()<<" "
            <<updateAppointment.getDoctorID()<<" "
            <<updateAppointment.getDate()<<" "
            <<updateAppointment.getTime()<<" "
            <<updateAppointment.getStatus()<<"\n";
        }

        else{
            fileOut<<appointmentID<<" "<<patientID<<" "<<doctorID<<" "<<date<<" "<<time<<" "<<status<<" "<<"\n";

        }

    }
    fileIn.close();
    fileOut.close();


    remove("appointments.txt");
    rename("temp.txt", "appointments.txt"); 
}


void FileHandler::updatePrescription(const Prescription& updatePrescription){
    std::ifstream fileIn("prescriptions.txt");
    std::ofstream fileOut("temp.txt");

    if(!fileIn.is_open()){
        throw FileNotFoundException("Error: prescriptions.txt could not be found on updatePrescriptions");
    }



    int prescriptionID;
    int appointmentID;
    char medicines[255];
    char notes[255];

    while(fileIn>> prescriptionID >> appointmentID >> medicines >> notes){
        if(prescriptionID == updatePrescription.getPrescriptionId()){
            fileOut<<updatePrescription.getPrescriptionId()<<" "
            <<updatePrescription.getAppointmentId() <<" "
            <<updatePrescription.getMedicines()<<" "
            <<updatePrescription.getNotes()<<"\n";
        }

        else{
            fileOut<<prescriptionID<<" "<<appointmentID<<" "<<medicines<<" "<<notes<<" "<<"\n";

        }

    }
    fileIn.close();
    fileOut.close();


    remove("prescriptions.txt");
    rename("temp.txt", "prescriptions.txt");
}



void FileHandler::updateBill(const Bill& updateBill){
    std::ifstream fileIn("bills.txt");
    std::ofstream fileOut("temp.txt");

    if(!fileIn.is_open()){
        throw FileNotFoundException("Error: bills.txt could not be found on updateBill");
    }


    int billID;
    int appointmentID;
    int patientID;
    double amount;
    char status[20];



    while(fileIn>> billID >> appointmentID >> patientID >> amount >> status ){
        if(billID == updateBill.getBillID()){
            fileOut<<updateBill.getBillID() <<" "
            <<updateBill.getAppointmentID()<<" "
            <<updateBill.getPatientID()<<" "
            <<updateBill.getAmount()<<" "
            <<updateBill.getStatus()<<"\n";
        }

        else{
            fileOut<<billID<<" "<<appointmentID<<" "<<patientID<<" "<<amount<<" "<<status<<"\n";

        }

    }
    fileIn.close();
    fileOut.close();


    remove("bills.txt");
    rename("temp.txt", "bills.txt");
}





// Deleters
void FileHandler::deletePatient(int patientID){
    std::ifstream fileIn("patients.txt");
    std::ofstream fileOut("temp.txt");

    if(!fileIn.is_open()){
        throw FileNotFoundException("Error: patients.txt could not be found on deletePatient");
    }

    int id;
    char name[50];
    char password[50];
    int age;
    char gender[20];
    char contact[20];
    double balance;

    while(fileIn>>id>>name>>password>>age>>gender>>contact>>balance){
        if(id == patientID){
            continue;
        }

        else {
            fileOut<<id<<" "<<name<<" "<<password<<" "<<age<<" "<<gender<<" "<<contact<<" "<<balance<<"\n";
        }
    }

    fileIn.close();
    fileOut.close();


    remove("patients.txt");
    rename("temp.txt" , "patients.txt" );



}


void FileHandler::deleteDoctor(int doctorID){
    std::ifstream fileIn("doctors.txt");
    std::ofstream fileOut("temp.txt");

    if(!fileIn.is_open()){
        throw FileNotFoundException("Error: doctors.txt could not be found on deleteDoctor");
    }

    int id;
    char name[50];
    char password[50];
    char specialization[50];
    char contact[20];
    double fee;

    while(fileIn>>id>>name>>password>>specialization>>contact>>fee){
        if(id == doctorID){
            continue;
        }

        else {
            fileOut<<id<<" "<<name<<" "<<password<<" "<<specialization<<" "<<contact<<" "<<fee<<"\n";
        }
    }

    fileIn.close();
    fileOut.close();


    remove("doctors.txt");
    rename("temp.txt" , "doctors.txt" );



}


void FileHandler::deleteAdmin(int adminID){
    std::ifstream fileIn("admins.txt");
    std::ofstream fileOut("temp.txt");

    if(!fileIn.is_open()){
        throw FileNotFoundException("Error: admins.txt could not be found on deleteAdmin");
    }

    int id;
    char name[50];
    char password[50];


    while(fileIn>> id >> name >> password ){
        if(id == adminID){
            continue;
        }

        else{
            fileOut<<id<<" "<<name<<" "<<password<<"\n";

        }

    }
    fileIn.close();
    fileOut.close();


    remove("admins.txt");
    rename("temp.txt", "admins.txt");
}
void FileHandler::deleteAppointment(int appointmentID){
    std::ifstream fileIn("appointments.txt");
    std::ofstream fileOut("temp.txt");

    if(!fileIn.is_open()){
        throw FileNotFoundException("Error: appointments.txt could not be found on deleteAppointment");
    }



    int appointmentId;
    int patientID;
    int doctorID;
    char date[20];
    char time[10];
    char status[20];

    while(fileIn>> appointmentId >> patientID >> doctorID >> date >> time >> status){
        if(appointmentId == appointmentID){
            continue;
        }

        else{
            fileOut<<appointmentID<<" "<<patientID<<" "<<doctorID<<" "<<date<<" "<<time<<" "<<status<<" "<<"\n";

        }

    }
    fileIn.close();
    fileOut.close();


    remove("appointments.txt");
    rename("temp.txt", "appointments.txt"); 
}
void FileHandler::deletePrescription(int prescriptionID){
    std::ifstream fileIn("prescriptions.txt");
    std::ofstream fileOut("temp.txt");

    if(!fileIn.is_open()){
        throw FileNotFoundException("Error: prescriptions.txt could not be found on deletePrescription");
    }



    int prescriptionId;
    int appointmentID;
    char medicines[255];
    char notes[255];

    while(fileIn>> prescriptionID >> appointmentID >> medicines >> notes){
        if(prescriptionId == prescriptionID){
            continue;
        }

        else{
            fileOut<<prescriptionID<<" "<<appointmentID<<" "<<medicines<<" "<<notes<<" "<<"\n";

        }

    }
    fileIn.close();
    fileOut.close();


    remove("prescriptions.txt");
    rename("temp.txt", "prescriptions.txt");
}


void FileHandler::deleteBill(int BillID){
    std::ifstream fileIn("bills.txt");
    std::ofstream fileOut("temp.txt");

    if(!fileIn.is_open()){
        throw FileNotFoundException("Error: bills.txt could not be found on deleteBill");
    }


    int billID;
    int appointmentID;
    int patientID;
    double amount;
    char status[20];



    while(fileIn>> billID >> appointmentID >> patientID >> amount >> status ){
        if(billID == BillID){
            continue;
        }

        else{
            fileOut<<billID<<" "<<appointmentID<<" "<<patientID<<" "<<amount<<" "<<status<<"\n";

        }

    }
    fileIn.close();
    fileOut.close();


    remove("bills.txt");
    rename("temp.txt", "bills.txt");
}


void FileHandler::deletePatientRecords(int patientID) {
    // Delete from appointments.txt
    {
        Storage<Appointment> temp;
        loadAppointments(temp);
        std::ofstream out("appointments.txt", std::ios::trunc);
        for(int i=0; i<temp.getSize(); i++) {
            if (temp.getAt(i)->getPatientID() != patientID) {
                out << temp.getAt(i)->getAppointmentID() << " " << temp.getAt(i)->getPatientID() << " " 
                    << temp.getAt(i)->getDoctorID() << " " << temp.getAt(i)->getDate() << " " 
                    << temp.getAt(i)->getTime() << " " << temp.getAt(i)->getStatus() << "\n";
            }
        }
        out.close();
    }
    // Delete from bills.txt
    {
        Storage<Bill> temp;
        loadBills(temp);
        std::ofstream out("bills.txt", std::ios::trunc);
        for(int i=0; i<temp.getSize(); i++) {
            if (temp.getAt(i)->getPatientID() != patientID) {
                out << temp.getAt(i)->getBillID() << " " << temp.getAt(i)->getAppointmentID() << " " 
                    << temp.getAt(i)->getPatientID() << " " << temp.getAt(i)->getAmount() << " " 
                    << temp.getAt(i)->getStatus() << "\n";
            }
        }
        out.close();
    }
    // Delete from prescriptions.txt
    {
        Storage<Prescription> temp;
        loadPrescriptions(temp);
        std::ofstream out("prescriptions.txt", std::ios::trunc);
        for(int i=0; i<temp.getSize(); i++) {
            // Prescriptions don't have patientID directly, need to check via appointment
            Storage<Appointment> appts;
            loadAppointments(appts);
            Appointment* a = appts.findByID(temp.getAt(i)->getAppointmentId());
            if (a && a->getPatientID() != patientID) {
                out << temp.getAt(i)->getPrescriptionId() << " " << temp.getAt(i)->getAppointmentId() << " " 
                    << temp.getAt(i)->getMedicines() << " " << temp.getAt(i)->getNotes() << "\n";
            }
        }
        out.close();
    }
}

void FileHandler::archivePatientRecords(int patientID) {
    std::ofstream out("discharged.txt", std::ios::app);
    if (!out.is_open()) return;

    // Archive Patient Info
    {
        Storage<Patient> temp;
        loadPatients(temp);
        Patient* p = temp.findByID(patientID);
        if (p) {
            out << "PATIENT," << p->getID() << "," << p->getName() << "," << p->getAge() << "," 
                << p->getGender() << "," << p->getContact() << "," << p->getBalance() << "\n";
        }
    }

    // Archive Appointments
    {
        Storage<Appointment> temp;
        loadAppointments(temp);
        for(int i=0; i<temp.getSize(); i++) {
            if (temp.getAt(i)->getPatientID() == patientID) {
                out << "APPOINTMENT," << temp.getAt(i)->getAppointmentID() << "," << temp.getAt(i)->getPatientID() << "," 
                    << temp.getAt(i)->getDoctorID() << "," << temp.getAt(i)->getDate() << "," 
                    << temp.getAt(i)->getTime() << "," << temp.getAt(i)->getStatus() << "\n";
            }
        }
    }

    // Archive Bills
    {
        Storage<Bill> temp;
        loadBills(temp);
        for(int i=0; i<temp.getSize(); i++) {
            if (temp.getAt(i)->getPatientID() == patientID) {
                out << "BILL," << temp.getAt(i)->getBillID() << "," << temp.getAt(i)->getAppointmentID() << "," 
                    << temp.getAt(i)->getPatientID() << "," << temp.getAt(i)->getAmount() << "," 
                    << temp.getAt(i)->getStatus() << "\n";
            }
        }
    }

    // Archive Prescriptions
    {
        Storage<Prescription> temp;
        loadPrescriptions(temp);
        Storage<Appointment> appts;
        loadAppointments(appts);
        for(int i=0; i<temp.getSize(); i++) {
            Appointment* a = appts.findByID(temp.getAt(i)->getAppointmentId());
            if (a && a->getPatientID() == patientID) {
                out << "PRESCRIPTION," << temp.getAt(i)->getPrescriptionId() << "," << temp.getAt(i)->getAppointmentId() << "," 
                    << temp.getAt(i)->getMedicines() << "," << temp.getAt(i)->getNotes() << "\n";
            }
        }
    }
    out.close();
}
