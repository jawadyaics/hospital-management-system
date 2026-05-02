#pragma once
#include "hospital-exception.h"

class InvalidInputException : public HospitalException {
    public:
    InvalidInputException(const char* m);
};