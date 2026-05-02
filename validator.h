#pragma once
//#include "invalid-input-exception.h"


class Validator {
    private:
    static int stringLength(const char* str);
    static bool isDigit(char c);


    public:

    static void validateID(int id);
    static void validateDate(const char* date);
    static void validateTimeSlot(const char* time);
    static void validateContact(const char* contact);
    static void validatePassword(const char* password);
    static void validatePositiveDouble(double value);
    static void validateMenuChoice(int choice , int min , int max);
};