#include "sfml-interface.h"

int main() {
    // 1. Create Data Structures
    Storage<Patient> patientDB;
    Storage<Doctor> doctorDB;
    Storage<Admin> adminDB;
    FileHandler fileHandler;

    // 2. Load Data from Files
    fileHandler.loadPatients(patientDB);
    fileHandler.loadDoctors(doctorDB);
    fileHandler.loadAdmins(adminDB);

    // 3. Initialize and Run the GUI
    SfmlInterface gui(patientDB, doctorDB, adminDB, fileHandler);
    gui.run();

    return 0;
}