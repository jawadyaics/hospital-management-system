#include "sfml-interface.h"
#include <iostream>

int main() {
    try {
        Storage<Patient> patientDB;
        Storage<Doctor> doctorDB;
        Storage<Admin> adminDB;
        FileHandler fileHandler;

        // Load data with basic error suppression for missing files
        try { fileHandler.loadPatients(patientDB); } catch (...) {}
        try { fileHandler.loadDoctors(doctorDB); } catch (...) {}
        try { fileHandler.loadAdmins(adminDB); } catch (...) {
            // Add default admin if none exist
            Admin defaultAdmin(1, "Admin", "admin123");
            adminDB.add(defaultAdmin);
            fileHandler.appendAdmin(defaultAdmin);
        }

        SfmlInterface gui(patientDB, doctorDB, adminDB, fileHandler);
        gui.run();

    } catch (const std::exception& e) {
        std::cerr << "Fatal Error: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "An unknown fatal error occurred." << std::endl;
    }

    return 0;
}
