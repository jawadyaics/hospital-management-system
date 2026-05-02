#include "bill.h"

void Bill::stringCopy(char* destination , const char* source){
    int i = 0;
    for( ; source[i] != '\0'; i++){
        destination[i] = source[i];
    }
    destination[i] = '\0';
}


Bill::Bill(){
    billID = 0;
    appointmentID = 0;
    patientID = 0;
    amount = 0;
    status[0] = '\0';
}


Bill::Bill(int bID , int aID , int pID , double a , const char* s):billID(bID) , appointmentID(aID) , patientID(pID), amount(a){
    stringCopy(this->status , s);
}


int Bill::getBillID() const {
    return billID;
}
int Bill::getAppointmentID() const{
    return appointmentID;
}
int Bill::getPatientID() const {
    return patientID;
}

double Bill::getAmount() const {
    return amount;
}
const char* Bill::getStatus() const {
    return status;
}


void Bill::setStatus(const char* status){
    stringCopy(this->status , status);
}

void Bill::setAmount(double amount){
    this->amount = amount;
}


std::ostream& operator<<(std::ostream& out, const Bill& other) {
    out << "Bill ID: " << other.getBillID() <<"\n"
       << "Appt ID: " << other.getAppointmentID() <<"\n"
       << "Amount: PKR " << other.getAmount() <<"\n"
       << "Status: " << other.getStatus() <<"\n";
    return out;
}