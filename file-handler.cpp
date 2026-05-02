#include "file-handler.h"
#include <fstream>

// Loaders Implementation

void FileHandler::loadPatients(Storage<Patient>& patientDB){
    std::ifstream file("patients.txt");

    if(!file.is_open()){
        //throw FileNotFoundException("Error: patient.txt could not be found on Startup");
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
        //throw FileNotFoundException("Error: doctors.txt could not be found on Startup");
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
        //throw FileNotFoundException("Error: admins.txt could not be found on startup");
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

//void loadAppointments(Storage<Appointment>& appointmentDB){};


// Appending


void FileHandler::appendPatient(const Patient& p){
    std::ofstream file("patients.txt" , std::ios::app);
    if(!file.is_open()){
        //throw FileNotFoundException("Error: patients.txt could not be found on Append");
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
        //throw FileNotFoundException("Error: doctors.txt could not be found on Append");
    }

    file<<d.getID()<<" "
    <<d.getName()<<" "
    <<d.getPassword()<<" "
    <<d.getSpecialization()<<" "
    <<d.getContact()<<" "
    <<d.getFee()<<"\n";
    file.close();

}



    //Updaters
void FileHandler::updateDoctor(const Patient& updatePatient){
    std::ifstream fileIn("patients.txt");
    std::ofstream fileOut("temp.txt");

    if(!fileIn.is_open()){
        //throw FileNotFoundException("Error: patients.txt could not be found on updatePatient");
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
        //throw FileNotFoundException("Error: doctors.txt could not be found on updateDoctor");
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


void FileHandler::deletePatient(int patientID){
    std::ifstream fileIn("patients.txt");
    std::ofstream fileOut("temp.txt");

    if(!fileIn.is_open()){
        //throw fileNotFoundException("Error: patients.txt could not be found on deletePatient");
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
        //throw fileNotFoundException("Error: doctors.txt could not be found on deleteDoctor");
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