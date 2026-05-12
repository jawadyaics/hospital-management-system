#include "validator.h"
#include "invalid-input-exception.h"
#include <ctime>


int Validator::stringLength(const char* str){
    int i = 0;
    while(str[i] != '\0') i++;
    return i;
}


bool Validator::isDigit(char c){
    if(c>='0' && c<='9') return true;
    else return false;
}


void Validator::validateID(int id){
    if(id <= 0){
        throw InvalidInputException("Invalid ID , it must be a positive number");
    }
}


void Validator::validateDate(const char* date){
    int len = stringLength(date);

    if(len != 10){
        throw InvalidInputException("The date must be in the Formate DD-MM-YYYY.");
    }

    for(int i = 0; i<10; i++){
        if(i == 2 || i == 5){
            if(date[i] != '-') throw InvalidInputException("Date must contain hyphen at position 3 and 6. Formate is DD-MM-YYYY");
        }

        else{
            if(!isDigit(date[i])) throw InvalidInputException("Date must contain only digits and hyphens");
        }
    }

    int dd = static_cast<int>(date[0] - '0');
    dd *= 10;
    dd += static_cast<int>(date[1] - '0');

    if(dd <= 0 || dd > 31){
        throw InvalidInputException("Date must be with in 01 to 31");
    }

    int mm = static_cast<int>(date[3] - '0');
    mm *= 10;
    mm += static_cast<int>(date[4] - '0');

    if(mm <= 0 || mm > 12){
        throw InvalidInputException("Month must be with in 01 to 12");
    }


    int y = static_cast<int>(date[6] - '0') * 1000;
    y += static_cast<int>(date[7] - '0') * 100;
    y += static_cast<int>(date[8] - '0') * 10;
    y += static_cast<int> (date[9] - '0');

    if (y < 2026) {
        throw InvalidInputException("Choose a Valid year 2026 or Later");
    }

}

bool Validator::validateDate(const char* date, bool returnBool) {
    try {
        validateDate(date);
        
        int d = (date[0]-'0')*10 + (date[1]-'0');
        int m = (date[3]-'0')*10 + (date[4]-'0');
        int y = (date[6]-'0')*1000 + (date[7]-'0')*100 + (date[8]-'0')*10 + (date[9]-'0');

        time_t t = time(0);
        char* dt = ctime(&t);
        // ctime format: "Wed Jun 30 21:49:08 1993\n"
        // Month at 4, Day at 8, Year at 20
        
        int curY = (dt[20]-'0')*1000 + (dt[21]-'0')*100 + (dt[22]-'0')*10 + (dt[23]-'0');
        int curD = (dt[8] == ' ' ? 0 : (dt[8]-'0')*10) + (dt[9]-'0');
        
        int curM = 1;
        char mon[4]; mon[0] = dt[4]; mon[1] = dt[5]; mon[2] = dt[6]; mon[3] = '\0';
        if (mon[0] == 'J' && mon[1] == 'a' && mon[2] == 'n') curM = 1;
        else if (mon[0] == 'F' && mon[1] == 'e' && mon[2] == 'b') curM = 2;
        else if (mon[0] == 'M' && mon[1] == 'a' && mon[2] == 'r') curM = 3;
        else if (mon[0] == 'A' && mon[1] == 'p' && mon[2] == 'r') curM = 4;
        else if (mon[0] == 'M' && mon[1] == 'a' && mon[2] == 'y') curM = 5;
        else if (mon[0] == 'J' && mon[1] == 'u' && mon[2] == 'n') curM = 6;
        else if (mon[0] == 'J' && mon[1] == 'u' && mon[2] == 'l') curM = 7;
        else if (mon[0] == 'A' && mon[1] == 'u' && mon[2] == 'g') curM = 8;
        else if (mon[0] == 'S' && mon[1] == 'e' && mon[2] == 'p') curM = 9;
        else if (mon[0] == 'O' && mon[1] == 'c' && mon[2] == 't') curM = 10;
        else if (mon[0] == 'N' && mon[1] == 'o' && mon[2] == 'v') curM = 11;
        else if (mon[0] == 'D' && mon[1] == 'e' && mon[2] == 'c') curM = 12;

        if (y < curY) return false;
        if (y == curY) {
            if (m < curM) return false;
            if (m == curM && d < curD) return false;
        }
        return true;
    } catch (...) {
        return false;
    }
}
void Validator::validateTimeSlot(const char* time){
    if(stringLength(time) != 5){
        throw InvalidInputException("Time musst contain 5 Characters. Formate is HH:MM");
    }

    for(int i = 0; i<5; i++){
        if(i ==2){
            if(time[i] != ':') throw InvalidInputException("Time must conatin a colon : ");
        }

        else {
            if(!isDigit(time[i])) throw InvalidInputException("Time must not contain only Integers and digits");
        }

        int hh = static_cast<int>(time[0] - '0') * 10;
        hh += static_cast<int>(time[1] - '0');

        if (hh<0 || hh>24){
            throw InvalidInputException("Hours must be with in 00 and 24");
        }

        int dd = static_cast<int>(time[3] - '0') * 10;
        dd += static_cast<int>(time[4] - '0'); 

        if (dd<0 || dd>60){
            throw InvalidInputException("Minutes must be with in 00 and 60");
        }

    }
}



void Validator::validateContact(const char* contact){
    int len = stringLength(contact);

    if(len != 11){
        throw InvalidInputException("Contact must be exactly 11 digits all numeric.");
    }

    for(int i = 0; i<11; i++){
        if(!isDigit(contact[i])) throw InvalidInputException("Contact must contain only digits.");
    }
}


void Validator::validatePassword(const char* password){
    int len = stringLength(password);

    if(len < 6){
        throw InvalidInputException("Password cannot be less then 6 characters");
    }

    for(int i = 0; i<len; i++){
        if(password[i] == ' '){
            throw InvalidInputException("Password cannot contain any Space Character");
        }
    }
}


void Validator::validatePositiveDouble(double value){
    if(value <= 0.0){
        throw InvalidInputException("Invalid Amount. Value cannot be negative");
    }
}
void Validator::validateMenuChoice(int choice , int min , int max){
    if(choice < min || choice > max){
        throw InvalidInputException("Invalid Choice. Choose a valide option");

    }
}