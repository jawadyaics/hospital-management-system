#include "hospital-exception.h"

void HospitalException::stringCopy(char* destination , const char* source){
    int i = 0;
    for( ; source[i] != '\0'; i++){
        destination[i] = source[i];
    }
    destination[i] = '\0';
}


HospitalException::HospitalException(){
    message[0] = '\0';
}


HospitalException::HospitalException(const char* m){
    stringCopy(message , m);
    }


HospitalException::~HospitalException(){

}


const char* HospitalException::what() const{
    return message;
}
