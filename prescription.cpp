#include "prescription.h"

void Prescription::stringCopy(char* destination , const char* source){
    int i = 0;
    for( ; source[i] != '\0'; i++){
        destination[i] = source[i];
    }
    destination[i] = '\0';
}


Prescription::Prescription() {
    prescriptionID = 0;
    appointmentID = 0;
    medicines[0] = '\0';
    notes[0] = '\0';
}

Prescription::Prescription(int pId, int aId, const char* m, const char* n): prescriptionID(pId) , appointmentID(aId) {
    stringCopy(medicines , m);
    stringCopy(notes , n);
}

int Prescription::getPrescriptionId() const { return prescriptionID; }
int Prescription::getAppointmentId() const { return appointmentID; }
const char* Prescription::getMedicines() const { return medicines; }
const char* Prescription::getNotes() const { return notes; }

//Setters
void Prescription::setMedicines(const char* medicine) { 
    stringCopy(medicines, medicine); 
}
void Prescription::setNotes(const char* note) { 
    stringCopy(notes, note); 
}

std::ostream& operator<<(std::ostream& out , const Prescription& other){
    out<<"Prescription ID: "<<other.getPrescriptionId()<<std::endl;
    out<<"Appointment ID: "<<other.getAppointmentId()<<std::endl;
    out<<"Medicines: "<<other.getMedicines()<<std::endl;
    out<<"Notes: "<<other.getNotes()<<std::endl;
    return out;
}