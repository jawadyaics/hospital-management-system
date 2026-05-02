#pragma once
#include "hospital-exception.h"

class InsufficientFundsException : public HospitalException {
    public:
    InsufficientFundsException(const char* m);
};