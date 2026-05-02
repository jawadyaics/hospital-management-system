#pragma once
#include "hospital-exception.h"

class SlotUnavailableException : public HospitalException {
    public:
    SlotUnavailableException(const char* m);
};