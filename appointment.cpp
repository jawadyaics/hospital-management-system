#include "appointment.h" 
 
void Appointment::stringCopy(char* destination , const char* source){
    int i = 0;
    for( ; source[i] != '\0'; i++){
        destination[i] = source[i];
    }
    destination[i] = '\0';
}


bool Appointment::stringCompare(const char* str1 , const char* str2){
    int i = 0;
    while(str1[i] != '\0' && str2[i] != '\0'){
        if(str1[i] != str2[i]) return false;
        i++;
    }
    return str1[i] == '\0' && str2[i] == '\0';
}

 
Appointment::Appointment(){
    appointmentID = 0;
    patientID = 0;
    doctorID = 0;
    date[0] = '\0';
    time[0] = '\0';
    stringCopy(status , "Schedualed");

}

Appointment::Appointment(int aptID, int pID, int dID, const char* dt , const char* tm , const char * st):
appointmentID(aptID) , patientID(pID) , doctorID(dID)
{
    stringCopy(date , dt);
    stringCopy(time , tm);
    stringCopy(status , st);
}


    // Getters
int Appointment::getAppointmentID() const{
    return appointmentID;
}

int Appointment::getPatientID() const{
    return patientID;
}

int Appointment::getDoctorID()const {
    return doctorID;
}

const char* Appointment::getDate() const{
    return date;
}
const char* Appointment::getTime() const{
    return time;
}
const char* Appointment::getStatus() const{
    return status;
}


    //Setters
void Appointment::setDate(const char* date){
    stringCopy(this->date , date);
}
void Appointment::setTime(const char* time){
    stringCopy(this->time , time);

}
void Appointment::getStatus(const char* status){
    stringCopy(this->status , status);

}


    //Operator Overloads
bool Appointment::operator== (const Appointment& other){
    if(doctorID != other.getDoctorID()) return false;

    if(!stringCompare(date , other.date)) return false;

    if(!stringCompare(time , other.time)) return false;

    if(!stringCompare(status , "Cancelled") || !stringCompare(other.status , "Cancelled")) return false;


    return true;
}

std::ostream& operator<<(std::ostream& out, const Appointment& other){
    out<<"Appointment ID: "<<other.getAppointmentID()<<std::endl;
    out<<"Doctor ID: "<<other.getDoctorID()<<std::endl;
    out<<"Patient ID: "<<other.getPatientID()<<std::endl;
    out<<"Date: "<<other.getDate()<<std::endl;
    out<<"Time: "<<other.getTime()<<std::endl;
    out<<"Status: "<<other.getStatus()<<std::endl;
    return out;
}