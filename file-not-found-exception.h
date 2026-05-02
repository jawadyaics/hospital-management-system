#pragma once
#include "hospital-exception.h"
class FileNotFoundException : public HospitalException {
    public:
    FileNotFoundException(const char* m);
}; 