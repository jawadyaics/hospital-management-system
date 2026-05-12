#include "sfml-interface.h"
#include "validator.h"
#include <iostream>
#include <fstream>
#include <cstdio>
#include <ctime>

// Helper functions for manual string handling
void stringCopy(char* destination, const char* source) {
    int i = 0;
    while (source[i] != '\0') {
        destination[i] = source[i];
        i++;
    }
    destination[i] = '\0';
}

bool stringCompare(const char* str1, const char* str2) {
    int i = 0;
    while (str1[i] != '\0' && str2[i] != '\0') {
        if (str1[i] != str2[i]) return false;
        i++;
    }
    return str1[i] == '\0' && str2[i] == '\0';
}

char toLower(char c) {
    if (c >= 'A' && c <= 'Z') return c + ('a' - 'A');
    return c;
}

bool stringCompareCaseInsensitive(const char* str1, const char* str2) {
    int i = 0;
    while (str1[i] != '\0' && str2[i] != '\0') {
        if (toLower(str1[i]) != toLower(str2[i])) return false;
        i++;
    }
    return str1[i] == '\0' && str2[i] == '\0';
}

void stringAppend(char* destination, const char* source) {
    int i = 0;
    while (destination[i] != '\0') i++;
    int j = 0;
    while (source[j] != '\0') {
        destination[i++] = source[j++];
    }
    destination[i] = '\0';
}

SfmlInterface::SfmlInterface(Storage<Patient>& pDB, Storage<Doctor>& dDB, Storage<Admin>& aDB, FileHandler& fh)
    : patientDB(pDB), doctorDB(dDB), adminDB(aDB), fileHandler(fh) 
{
    window.create(sf::VideoMode(800, 600), "MediCore Hospital Management System", sf::Style::Default);
    window.setFramerateLimit(60);
    
    // Set up a fixed view to handle resizing correctly
    sf::View view(sf::FloatRect(0, 0, 800, 600));
    window.setView(view);

    if (!font.loadFromFile("font.ttf")) {
        std::cout << "ERROR: Could not load font.ttf!\n";
    }

    // Load extra data
    try { fileHandler.loadAppointments(appointmentDB); } catch(...) {}
    try { fileHandler.loadBills(billDB); } catch(...) {}
    try { fileHandler.loadPrescriptions(prescriptionDB); } catch(...) {}

    currentState = LOGIN_SCREEN;
    activeInputField = 0;
    selectedRole = 1; // Default to Patient
    loggedInUserID = -1;
    currentChoice = 0;

    inputIdBuffer[0] = '\0';
    inputPasswordBuffer[0] = '\0';
    subState = 0;
    activeFormField = 0;
    for(int i=0; i<6; i++) formBuffers[i][0] = '\0';
}

void SfmlInterface::drawText(const char* text, float x, float y, int size, sf::Color color) {
    sf::Text sfText;
    sfText.setFont(font);
    sfText.setString(text);
    sfText.setCharacterSize(size);
    sfText.setFillColor(color);
    sfText.setPosition(x, y);
    window.draw(sfText);
}

void SfmlInterface::drawButton(const char* text, float x, float y, float width, float height, sf::Color bgColor) {
    sf::RectangleShape rect(sf::Vector2f(width, height));
    rect.setPosition(x, y);
    rect.setFillColor(bgColor);
    rect.setOutlineThickness(2);
    rect.setOutlineColor(sf::Color::White);
    window.draw(rect);

    sf::Text sfText;
    sfText.setFont(font);
    sfText.setString(text);
    sfText.setCharacterSize(18);
    sfText.setFillColor(sf::Color::White);
    
    // Center text
    sf::FloatRect textBounds = sfText.getLocalBounds();
    sfText.setPosition(x + (width - textBounds.width) / 2, y + (height - textBounds.height) / 2 - 5);
    window.draw(sfText);
}

void SfmlInterface::drawTextBox(const char* text, float x, float y, float width, float height, bool isActive) {
    sf::RectangleShape rect(sf::Vector2f(width, height));
    rect.setPosition(x, y);
    rect.setFillColor(isActive ? sf::Color(255, 255, 200) : sf::Color::White);
    rect.setOutlineThickness(isActive ? 3 : 1);
    rect.setOutlineColor(isActive ? sf::Color::Cyan : sf::Color(150, 150, 150));
    window.draw(rect);

    sf::Text sfText;
    sfText.setFont(font);
    sfText.setString(text);
    sfText.setCharacterSize(18);
    sfText.setFillColor(sf::Color::Black);
    sfText.setPosition(x + 10, y + 10);
    window.draw(sfText);
}

void SfmlInterface::run() {
    while (window.isOpen()) {
        handleEvents();
        window.clear(sf::Color(30, 30, 30));

        if (currentState == LOGIN_SCREEN) renderLoginScreen();
        else if (currentState == ADMIN_MENU) renderAdminMenu();
        else if (currentState == PATIENT_MENU) renderPatientMenu();
        else if (currentState == DOCTOR_MENU) renderDoctorMenu();

        window.display();
    }
}

void SfmlInterface::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) window.close();
        if (event.type == sf::Event::Resized) {
            // Update the view to the new size of the window
            sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
            // But we want to keep our 800x600 coordinate system
            // So we don't update the view's size, we just let SFML scale it
            // or we can adjust the viewport to maintain aspect ratio
        }
        if (event.type == sf::Event::MouseButtonPressed) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            sf::Vector2f mappedPos = window.mapPixelToCoords(mousePos);
            processMouseClick(mappedPos.x, mappedPos.y);
        }
        if (event.type == sf::Event::TextEntered) {
            processKeyBoardInput(event.text.unicode);
        }
    }
}

void SfmlInterface::processMouseClick(float mouseX, float mouseY) {
    if (currentState == LOGIN_SCREEN) {
        if (mouseY >= 200 && mouseY <= 240) {
            if (mouseX >= 200 && mouseX <= 300) selectedRole = 1; // Patient
            if (mouseX >= 350 && mouseX <= 450) selectedRole = 2; // Doctor
            if (mouseX >= 500 && mouseX <= 600) selectedRole = 3; // Admin
        }
        activeInputField = 0;
        if (mouseX >= 300 && mouseX <= 500) {
            if (mouseY >= 300 && mouseY <= 340) activeInputField = 1;
            if (mouseY >= 400 && mouseY <= 440) activeInputField = 2;
        }
        if (mouseX >= 300 && mouseX <= 500 && mouseY >= 500 && mouseY <= 550) attemptLogin();
    } else if (currentState == ADMIN_MENU) {
        if (currentChoice == 0) {
            if (mouseX >= 50 && mouseX <= 380) {
                if (mouseY >= 120 && mouseY <= 165) currentChoice = 1;
                else if (mouseY >= 180 && mouseY <= 225) currentChoice = 2;
                else if (mouseY >= 240 && mouseY <= 285) currentChoice = 3;
                else if (mouseY >= 300 && mouseY <= 345) currentChoice = 4;
                else if (mouseY >= 360 && mouseY <= 405) currentChoice = 5;
                else if (mouseY >= 420 && mouseY <= 465) currentChoice = 6;
            } else if (mouseX >= 420 && mouseX <= 750) {
                if (mouseY >= 120 && mouseY <= 165) currentChoice = 7;
                else if (mouseY >= 180 && mouseY <= 225) currentChoice = 8;
                else if (mouseY >= 240 && mouseY <= 285) currentChoice = 9;
                else if (mouseY >= 300 && mouseY <= 345) currentChoice = 10;
                else if (mouseY >= 360 && mouseY <= 405) currentChoice = 11;
            } else if (mouseX >= 50 && mouseX <= 750 && mouseY >= 510 && mouseY <= 555) {
                currentState = LOGIN_SCREEN;
                loggedInUserID = -1;
                currentChoice = 0;
            }
            // Clear form buffers when entering a sub-menu
            if (currentChoice != 0) {
                activeFormField = 1;
                for(int i=0; i<6; i++) formBuffers[i][0] = '\0';
            }
        } else {
            // Sub-menu handling
            if (mouseX >= 50 && mouseX <= 250 && mouseY >= 510 && mouseY <= 555) {
                currentChoice = 0;
                activeFormField = 0;
            }
            
            if (currentChoice == 1) { // Add Doctor form clicks
                if (mouseX >= 250 && mouseX <= 550) {
                    if (mouseY >= 115 && mouseY <= 150) activeFormField = 1;
                    else if (mouseY >= 165 && mouseY <= 200) activeFormField = 2;
                    else if (mouseY >= 215 && mouseY <= 250) activeFormField = 3;
                    else if (mouseY >= 265 && mouseY <= 300) activeFormField = 4;
                    else if (mouseY >= 315 && mouseY <= 350) activeFormField = 5;
                }
                if (mouseX >= 300 && mouseX <= 500 && mouseY >= 400 && mouseY <= 450) {
                    try {
                        Validator::validateContact(formBuffers[2]);
                        Validator::validatePassword(formBuffers[3]);
                        double fee = atof(formBuffers[4]);
                        Validator::validatePositiveDouble(fee);
                        int newID = doctorDB.getNextID();
                        Doctor d(newID, formBuffers[0], formBuffers[3], formBuffers[1], formBuffers[2], fee);
                        doctorDB.add(d);
                        fileHandler.appendDoctor(d);
                        currentChoice = 0;
                    } catch (...) {}
                }
            } else if (currentChoice == 2) { // Remove Doctor
                if (mouseX >= 300 && mouseX <= 500 && mouseY >= 180 && mouseY <= 220) { activeFormField = 1; return; }
                if (mouseX >= 300 && mouseX <= 500 && mouseY >= 250 && mouseY <= 295) {
                    int id = atoi(formBuffers[0]);
                    bool hasPending = false;
                    for(int i=0; i<appointmentDB.getSize(); i++) {
                        if (appointmentDB.getAt(i)->getDoctorID() == id && stringCompare(appointmentDB.getAt(i)->getStatus(), "Pending")) {
                            hasPending = true; break;
                        }
                    }
                    if (hasPending) {
                        subState = 99; // Error state
                    } else if (doctorDB.findByID(id)) {
                        doctorDB.removeByID(id);
                        fileHandler.deleteDoctor(id);
                        currentChoice = 0;
                    }
                }
            } else if (currentChoice == 3) { // Add Patient
                if (mouseX >= 250 && mouseX <= 550) {
                    if (mouseY >= 115 && mouseY <= 150) activeFormField = 1;
                    else if (mouseY >= 165 && mouseY <= 200) activeFormField = 2;
                    else if (mouseY >= 215 && mouseY <= 250) activeFormField = 3;
                    else if (mouseY >= 265 && mouseY <= 300) activeFormField = 4;
                    else if (mouseY >= 315 && mouseY <= 350) activeFormField = 5;
                    else if (mouseY >= 365 && mouseY <= 400) activeFormField = 6;
                }
                if (mouseX >= 300 && mouseX <= 500 && mouseY >= 420 && mouseY <= 470) {
                    try {
                        int age = atoi(formBuffers[1]);
                        Validator::validateContact(formBuffers[3]);
                        Validator::validatePassword(formBuffers[4]);
                        double bal = atof(formBuffers[5]);
                        int newID = patientDB.getNextID();
                        Patient p(newID, formBuffers[0], formBuffers[4], age, formBuffers[2], formBuffers[3], bal);
                        patientDB.add(p);
                        fileHandler.appendPatient(p);
                        currentChoice = 0;
                    } catch (...) {}
                }
            } else if (currentChoice == 4) { // Remove Patient
                if (mouseX >= 300 && mouseX <= 500 && mouseY >= 180 && mouseY <= 220) { activeFormField = 1; return; }
                if (mouseX >= 300 && mouseX <= 500 && mouseY >= 250 && mouseY <= 295) {
                    int id = atoi(formBuffers[0]);
                    bool hasPending = false;
                    bool hasUnpaid = false;
                    for(int i=0; i<appointmentDB.getSize(); i++) {
                        if (appointmentDB.getAt(i)->getPatientID() == id && stringCompare(appointmentDB.getAt(i)->getStatus(), "Pending")) {
                            hasPending = true; break;
                        }
                    }
                    for(int i=0; i<billDB.getSize(); i++) {
                        if (billDB.getAt(i)->getPatientID() == id && stringCompare(billDB.getAt(i)->getStatus(), "Unpaid")) {
                            hasUnpaid = true; break;
                        }
                    }
                                     if (hasPending) subState = 98;
                    else if (hasUnpaid) subState = 97;
                    else if (patientDB.findByID(id)) {
                        patientDB.removeByID(id);
                        fileHandler.deletePatient(id);
                        // Cascading deletions
                        fileHandler.deletePatientRecords(id);
                        // Refresh in-memory DBs
                        appointmentDB.clear(); billDB.clear(); prescriptionDB.clear();
                        fileHandler.loadAppointments(appointmentDB);
                        fileHandler.loadBills(billDB);
                        fileHandler.loadPrescriptions(prescriptionDB);
                        currentChoice = 0;
                    }
                }
            } else if (currentChoice == 9) { // Discharge Patient
                if (mouseX >= 300 && mouseX <= 500 && mouseY >= 200 && mouseY <= 240) { activeFormField = 1; return; }
                if (mouseX >= 300 && mouseX <= 500 && mouseY >= 300 && mouseY <= 350) {
                    int pid = atoi(formBuffers[0]);
                    bool hasPending = false;
                    bool hasUnpaid = false;
                    for(int i=0; i<appointmentDB.getSize(); i++) {
                        if (appointmentDB.getAt(i)->getPatientID() == pid && stringCompare(appointmentDB.getAt(i)->getStatus(), "Pending")) {
                            hasPending = true; break;
                        }
                    }
                    for(int i=0; i<billDB.getSize(); i++) {
                        if (billDB.getAt(i)->getPatientID() == pid && stringCompare(billDB.getAt(i)->getStatus(), "Unpaid")) {
                            hasUnpaid = true; break;
                        }
                    }

                    if (hasPending) subState = 96;
                    else if (hasUnpaid) subState = 95;
                    else if (patientDB.findByID(pid)) {
                        fileHandler.archivePatientRecords(pid);
                        patientDB.removeByID(pid);
                        fileHandler.deletePatient(pid);
                        fileHandler.deletePatientRecords(pid);
                        // Refresh
                        appointmentDB.clear(); billDB.clear(); prescriptionDB.clear();
                        fileHandler.loadAppointments(appointmentDB);
                        fileHandler.loadBills(billDB);
                        fileHandler.loadPrescriptions(prescriptionDB);
                        currentChoice = 0;
                    }
                }
            }
        }
    } else if (currentState == DOCTOR_MENU) {
        if (currentChoice == 0) {
            if (mouseX >= 200 && mouseX <= 600) {
                if (mouseY >= 100 && mouseY <= 145) currentChoice = 1;
                else if (mouseY >= 160 && mouseY <= 205) currentChoice = 2;
                else if (mouseY >= 220 && mouseY <= 265) currentChoice = 3;
                else if (mouseY >= 280 && mouseY <= 325) currentChoice = 4;
                else if (mouseY >= 340 && mouseY <= 385) currentChoice = 5;
                else if (mouseY >= 500 && mouseY <= 545) { currentState = LOGIN_SCREEN; loggedInUserID = -1; }
            }
            if (currentChoice != 0) {
                activeFormField = 1; subState = 0;
                for(int i=0; i<6; i++) formBuffers[i][0] = '\0';
            }
        } else {
            if (mouseX >= 50 && mouseX <= 250 && mouseY >= 510 && mouseY <= 555) { currentChoice = 0; subState = 0; }
            
            if (currentChoice == 2 || currentChoice == 3) { // Mark Complete/No-Show
                if (mouseX >= 300 && mouseX <= 500 && mouseY >= 420 && mouseY <= 460) { activeFormField = 1; return; }
                if (mouseX >= 300 && mouseX <= 500 && mouseY >= 470 && mouseY <= 515) {
                    time_t t = time(0); char* dt = ctime(&t);
                    char today[20];
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
                    sprintf(today, "%02d-%02d-%04d", curD, curM, curY);

                    int aid = atoi(formBuffers[0]);
                    Appointment* a = appointmentDB.findByID(aid);
                    if (a && a->getDoctorID() == loggedInUserID && 
                        (stringCompare(a->getStatus(), "Pending") || stringCompare(a->getStatus(), "Schedualed")) && 
                        stringCompare(a->getDate(), today)) {
                        
                        if (currentChoice == 2) {
                            a->setStatus("Completed");
                            fileHandler.updateAppointment(*a);
                            subState = 1; // Success
                        } else {
                            a->setStatus("No-Show");
                            fileHandler.updateAppointment(*a);
                            // Cancel corresponding bill
                            for(int i=0; i<billDB.getSize(); i++) {
                                if (billDB.getAt(i)->getAppointmentID() == aid) {
                                    billDB.getAt(i)->setStatus("Cancelled");
                                    fileHandler.updateBill(*billDB.getAt(i));
                                }
                            }
                            subState = 1; // Success
                        }
                    } else {
                        subState = 2; // Invalid ID or criteria
                    }
                }
            } else if (currentChoice == 4) { // Write Prescription
                if (mouseY >= 115 && mouseY <= 150 && mouseX >= 150 && mouseX <= 250) { activeFormField = 1; return; }
                else if (mouseY >= 165 && mouseY <= 200 && mouseX >= 150 && mouseX <= 650) { activeFormField = 2; return; }
                else if (mouseY >= 215 && mouseY <= 315 && mouseX >= 150 && mouseX <= 650) { activeFormField = 3; return; }
                
                if (mouseX >= 300 && mouseX <= 500 && mouseY >= 350 && mouseY <= 395) {
                    int aid = atoi(formBuffers[0]);
                    Appointment* a = appointmentDB.findByID(aid);
                    if (a && a->getDoctorID() == loggedInUserID && stringCompare(a->getStatus(), "Completed")) {
                        int pid = prescriptionDB.getNextID();
                        Prescription p(pid, aid, formBuffers[1], formBuffers[2]);
                        prescriptionDB.add(p);
                        fileHandler.appendPrescription(p);
                        currentChoice = 0;
                    }
                }
            } else if (currentChoice == 5) { // View History
                if (mouseX >= 250 && mouseX <= 350 && mouseY >= 115 && mouseY <= 150) { activeFormField = 1; return; }
                if (mouseX >= 400 && mouseX <= 500 && mouseY >= 115 && mouseY <= 150) {
                    int pid = atoi(formBuffers[0]);
                    if (patientDB.findByID(pid)) subState = 1;
                }
            }
        }
    } else if (currentState == PATIENT_MENU) {
        if (currentChoice == 0) {
            if (mouseX >= 50 && mouseX <= 380) {
                if (mouseY >= 120 && mouseY <= 165) currentChoice = 1;
                else if (mouseY >= 180 && mouseY <= 225) currentChoice = 2;
                else if (mouseY >= 240 && mouseY <= 285) currentChoice = 3;
                else if (mouseY >= 300 && mouseY <= 345) currentChoice = 4;
            } else if (mouseX >= 420 && mouseX <= 750) {
                if (mouseY >= 120 && mouseY <= 165) currentChoice = 5;
                else if (mouseY >= 180 && mouseY <= 225) currentChoice = 6;
                else if (mouseY >= 240 && mouseY <= 285) currentChoice = 7;
            } else if (mouseX >= 50 && mouseX <= 750 && mouseY >= 510 && mouseY <= 555) {
                currentState = LOGIN_SCREEN; loggedInUserID = -1;
            }
            if (currentChoice != 0) {
                activeFormField = 1; subState = 0;
                for(int i=0; i<6; i++) formBuffers[i][0] = '\0';
            }
        } else {
            if (mouseX >= 50 && mouseX <= 250 && mouseY >= 510 && mouseY <= 555) { currentChoice = 0; subState = 0; }
            
            if (currentChoice == 1) { // Book Appointment logic
                if (subState == 0) {
                    if (mouseX >= 250 && mouseX <= 550 && mouseY >= 115 && mouseY <= 150) { activeFormField = 1; return; }
                    if (mouseX >= 560 && mouseX <= 660 && mouseY >= 115 && mouseY <= 150) subState = 1;
                } else if (subState == 1) {
                    if (mouseX >= 200 && mouseX <= 300 && mouseY >= 395 && mouseY <= 430) { activeFormField = 2; return; }
                    if (mouseX >= 320 && mouseX <= 420 && mouseY >= 395 && mouseY <= 430) {
                        if (doctorDB.findByID(atoi(formBuffers[1]))) subState = 2;
                    }
                } else if (subState == 2) {
                    if (mouseX >= 300 && mouseX <= 500 && mouseY >= 145 && mouseY <= 180) { activeFormField = 3; return; }
                    if (mouseX >= 300 && mouseX <= 500 && mouseY >= 200 && mouseY <= 240) {
                        if (Validator::validateDate(formBuffers[2], true)) subState = 3;
                        else {
                            static int dateAttempts = 0;
                            dateAttempts++;
                            if (dateAttempts >= 3) { dateAttempts = 0; currentChoice = 0; }
                        }
                    }
                } else if (subState == 3) {
                    if (mouseX >= 300 && mouseX <= 500 && mouseY >= 180 && mouseY <= 220) { activeFormField = 4; return; }
                    if (mouseX >= 300 && mouseX <= 500 && mouseY >= 250 && mouseY <= 300) {
                        const char* validSlots[] = {"09:00", "10:00", "11:00", "12:00", "13:00", "14:00", "15:00", "16:00"};
                        bool validSlot = false;
                        for(int i=0; i<8; i++) if(stringCompare(formBuffers[3], validSlots[i])) validSlot = true;
                        
                        if (!validSlot) return;

                        Doctor* d = doctorDB.findByID(atoi(formBuffers[1]));
                        Patient* p = patientDB.findByID(loggedInUserID);
                        
                        // Check if slot is taken
                        for(int i=0; i<appointmentDB.getSize(); i++) {
                            Appointment* a = appointmentDB.getAt(i);
                            if (a->getDoctorID() == d->getID() && stringCompare(a->getDate(), formBuffers[2]) && 
                                stringCompare(a->getTime(), formBuffers[3]) && !stringCompare(a->getStatus(), "Cancelled")) {
                                return; // Slot taken
                            }
                        }

                        if (p->getBalance() >= d->getFee()) {
                            int aid = appointmentDB.getNextID();
                            Appointment a(aid, loggedInUserID, d->getID(), formBuffers[2], formBuffers[3], "Pending");
                            appointmentDB.add(a); fileHandler.appendAppointment(a);
                            int bid = billDB.getNextID();
                            Bill b(bid, aid, loggedInUserID, d->getFee(), "Unpaid");
                            billDB.add(b); fileHandler.appendBill(b);
                            currentChoice = 0;
                        }
                    }
                }
            } else if (currentChoice == 2) { // Cancel Appointment
                if (mouseX >= 300 && mouseX <= 500 && mouseY >= 420 && mouseY <= 460) { activeFormField = 1; return; }
                if (mouseX >= 300 && mouseX <= 500 && mouseY >= 470 && mouseY <= 515) {
                    int aid = atoi(formBuffers[0]);
                    Appointment* a = appointmentDB.findByID(aid);
                    if (a && a->getPatientID() == loggedInUserID && stringCompare(a->getStatus(), "Pending")) {
                        a->setStatus("Cancelled"); fileHandler.updateAppointment(*a);
                        for(int i=0; i<billDB.getSize(); i++) {
                            if (billDB.getAt(i)->getAppointmentID() == aid) {
                                billDB.getAt(i)->setStatus("Cancelled");
                                fileHandler.updateBill(*billDB.getAt(i));
                            }
                        }
                        subState = 1; // Success flag
                    } else {
                        subState = 2; // Error flag
                    }
                }
            } else if (currentChoice == 6) { // Pay Bill
                if (mouseX >= 300 && mouseX <= 500 && mouseY >= 420 && mouseY <= 460) { activeFormField = 1; return; }
                if (mouseX >= 300 && mouseX <= 500 && mouseY >= 470 && mouseY <= 515) {
                    int bid = atoi(formBuffers[0]);
                    Bill* b = billDB.findByID(bid);
                    Patient* p = patientDB.findByID(loggedInUserID);
                    if (b && b->getPatientID() == loggedInUserID && stringCompare(b->getStatus(), "Unpaid")) {
                        if (p->getBalance() >= b->getAmount()) {
                            (*p) -= b->getAmount(); // Use overloaded -=
                            b->setStatus("Paid");
                            fileHandler.updatePatient(*p); fileHandler.updateBill(*b);
                            subState = 1; // Success
                        } else {
                            subState = 2; // Insufficient funds
                        }
                    } else {
                        subState = 3; // Invalid ID
                    }
                }
            } else if (currentChoice == 7) { // Top Up
                if (mouseX >= 300 && mouseX <= 500 && mouseY >= 180 && mouseY <= 220) { activeFormField = 1; return; }
                if (mouseX >= 300 && mouseX <= 500 && mouseY >= 250 && mouseY <= 300) {
                    double amt = atof(formBuffers[0]);
                    if (amt > 0) {
                        Patient* p = patientDB.findByID(loggedInUserID);
                        p->addFunds(amt); fileHandler.updatePatient(*p);
                        currentChoice = 0;
                    }
                }
            }
        }
    }
}

void SfmlInterface::processKeyBoardInput(sf::Uint32 unicodeCode) {
    char* buffer = nullptr;
    if (currentState == LOGIN_SCREEN) {
        if (activeInputField == 1) buffer = inputIdBuffer;
        else if (activeInputField == 2) buffer = inputPasswordBuffer;
    } else if ((currentState == ADMIN_MENU || currentState == DOCTOR_MENU || currentState == PATIENT_MENU) && currentChoice != 0) {
        if (activeFormField >= 1 && activeFormField <= 6) buffer = formBuffers[activeFormField - 1];
    }

    if (buffer) {
        int len = 0;
        while (buffer[len] != '\0') len++;
        if (unicodeCode == 8) { if (len > 0) buffer[len - 1] = '\0'; }
        else if (unicodeCode >= 32 && unicodeCode <= 126) {
            if (len < 49) { buffer[len] = static_cast<char>(unicodeCode); buffer[len + 1] = '\0'; }
        }
    }
}

void SfmlInterface::attemptLogin() {
    int id = atoi(inputIdBuffer);
    if (selectedRole == 1) {
        Patient* p = patientDB.findByID(id);
        if (p && stringCompare(p->getPassword(), inputPasswordBuffer)) {
            currentState = PATIENT_MENU; loggedInUserID = id;
            fileHandler.logSecurityEvent("Patient", id, "Login Successful");
        } else fileHandler.logSecurityEvent("Patient", id, "Login Failed");
    } else if (selectedRole == 2) {
        Doctor* d = doctorDB.findByID(id);
        if (d && stringCompare(d->getPassword(), inputPasswordBuffer)) {
            currentState = DOCTOR_MENU; loggedInUserID = id;
            fileHandler.logSecurityEvent("Doctor", id, "Login Successful");
        } else fileHandler.logSecurityEvent("Doctor", id, "Login Failed");
    } else if (selectedRole == 3) {
        Admin* a = adminDB.findByID(id);
        if (a && stringCompare(a->getPassword(), inputPasswordBuffer)) {
            currentState = ADMIN_MENU; loggedInUserID = id;
            fileHandler.logSecurityEvent("Admin", id, "Login Successful");
        } else fileHandler.logSecurityEvent("Admin", id, "Login Failed");
    }
}

void SfmlInterface::renderLoginScreen() {
    drawText("MediCore Hospital Management", 150, 50, 32, sf::Color::White);
    drawText("System Login", 320, 120, 24, sf::Color::Cyan);
    drawText("Select Role:", 340, 170, 18, sf::Color::White);
    drawButton("Patient", 200, 200, 100, 40, selectedRole == 1 ? sf::Color::Green : sf::Color(100, 100, 100));
    drawButton("Doctor",  350, 200, 100, 40, selectedRole == 2 ? sf::Color::Green : sf::Color(100, 100, 100));
    drawButton("Admin",   500, 200, 100, 40, selectedRole == 3 ? sf::Color::Green : sf::Color(100, 100, 100));
    drawText("User ID:", 300, 270, 18, sf::Color::White);
    drawTextBox(inputIdBuffer, 300, 300, 200, 40, activeInputField == 1);
    drawText("Password:", 300, 370, 18, sf::Color::White);
    drawTextBox(inputPasswordBuffer, 300, 400, 200, 40, activeInputField == 2);
    drawButton("LOGIN", 300, 500, 200, 50, sf::Color(50, 150, 250));
}

void SfmlInterface::renderAdminMenu() {
    drawText("MediCore Admin Panel", 220, 20, 28, sf::Color::White);
    if (currentChoice == 0) {
        drawButton("1. Add Doctor", 50, 120, 330, 45, sf::Color(70, 130, 180));
        drawButton("2. Remove Doctor", 50, 180, 330, 45, sf::Color(70, 130, 180));
        drawButton("3. Add Patient", 50, 240, 330, 45, sf::Color(70, 130, 180));
        drawButton("4. Remove Patient", 50, 300, 330, 45, sf::Color(70, 130, 180));
        drawButton("5. View All Patients", 50, 360, 330, 45, sf::Color(70, 130, 180));
        drawButton("6. View All Doctors", 50, 420, 330, 45, sf::Color(70, 130, 180));
        drawButton("7. View All Appointments", 420, 120, 330, 45, sf::Color(70, 130, 180));
        drawButton("8. View Unpaid Bills", 420, 180, 330, 45, sf::Color(70, 130, 180));
        drawButton("9. Discharge Patient", 420, 240, 330, 45, sf::Color(70, 130, 180));
        drawButton("10. View Security Log", 420, 300, 330, 45, sf::Color(70, 130, 180));
        drawButton("11. Generate Daily Report", 420, 360, 330, 45, sf::Color(34, 139, 34));
        drawButton("12. Logout", 50, 510, 700, 45, sf::Color(178, 34, 34));
    } else {
        drawButton("Back to Admin Menu", 50, 510, 200, 45, sf::Color(100, 100, 100));
        
        if (currentChoice == 1) { // Add Doctor
            drawText("Add New Doctor", 320, 70, 24, sf::Color::Cyan);
            drawText("Name:", 100, 120, 18, sf::Color::White); drawTextBox(formBuffers[0], 250, 115, 300, 35, activeFormField == 1);
            drawText("Specialization:", 100, 170, 18, sf::Color::White); drawTextBox(formBuffers[1], 250, 165, 300, 35, activeFormField == 2);
            drawText("Contact (11):", 100, 220, 18, sf::Color::White); drawTextBox(formBuffers[2], 250, 215, 300, 35, activeFormField == 3);
            drawText("Password (6+):", 100, 270, 18, sf::Color::White); drawTextBox(formBuffers[3], 250, 265, 300, 35, activeFormField == 4);
            drawText("Fee (PKR):", 100, 320, 18, sf::Color::White); drawTextBox(formBuffers[4], 250, 315, 300, 35, activeFormField == 5);
            drawButton("SAVE DOCTOR", 300, 400, 200, 50, sf::Color::Green);
        } else if (currentChoice == 3) { // Add Patient
            drawText("Add New Patient", 320, 70, 24, sf::Color::Cyan);
            drawText("Name:", 100, 120, 18, sf::Color::White); drawTextBox(formBuffers[0], 250, 115, 300, 35, activeFormField == 1);
            drawText("Age:", 100, 170, 18, sf::Color::White); drawTextBox(formBuffers[1], 250, 165, 300, 35, activeFormField == 2);
            drawText("Gender (M/F):", 100, 220, 18, sf::Color::White); drawTextBox(formBuffers[2], 250, 215, 300, 35, activeFormField == 3);
            drawText("Contact (11):", 100, 270, 18, sf::Color::White); drawTextBox(formBuffers[3], 250, 265, 300, 35, activeFormField == 4);
            drawText("Password (6+):", 100, 320, 18, sf::Color::White); drawTextBox(formBuffers[4], 250, 315, 300, 35, activeFormField == 5);
            drawText("Balance:", 100, 370, 18, sf::Color::White); drawTextBox(formBuffers[5], 250, 365, 300, 35, activeFormField == 6);
            drawButton("SAVE PATIENT", 300, 420, 200, 50, sf::Color::Green);
        } else if (currentChoice == 2) { // Remove Doctor
            drawText("Remove Doctor", 320, 70, 24, sf::Color::Cyan);
            drawText("Enter Doctor ID:", 300, 150, 18, sf::Color::White);
            drawTextBox(formBuffers[0], 300, 180, 200, 40, activeFormField == 1);
            drawButton("REMOVE", 300, 250, 200, 45, sf::Color::Red);
            if (subState == 99) drawText("Cannot remove doctor with pending appointments.", 180, 320, 16, sf::Color::Red);
        } else if (currentChoice == 4) { // Remove Patient
            drawText("Remove Patient", 320, 70, 24, sf::Color::Cyan);
            drawText("Enter Patient ID:", 300, 150, 18, sf::Color::White);
            drawTextBox(formBuffers[0], 300, 180, 200, 40, activeFormField == 1);
            drawButton("REMOVE", 300, 250, 200, 45, sf::Color::Red);
            if (subState == 98) drawText("Cannot remove patient with pending appointments.", 180, 320, 16, sf::Color::Red);
            else if (subState == 97) drawText("Cannot remove patient with unpaid bills.", 180, 320, 16, sf::Color::Red);
        } else if (currentChoice == 5) { // View All Patients
            drawText("All Patients", 320, 70, 24, sf::Color::Cyan);
            drawText("ID | Name | Age | Gender | Balance", 60, 110, 16, sf::Color::Yellow);
            float y = 140;
            for(int i=0; i<patientDB.getSize(); i++) {
                Patient* p = patientDB.getAt(i);
                char line[255];
                sprintf(line, "%d | %s | %d | %s | %.2f", p->getID(), p->getName(), p->getAge(), p->getGender(), p->getBalance());
                drawText(line, 60, y, 14, sf::Color::White);
                y += 20; if (y > 480) break;
            }
        } else if (currentChoice == 6) { // View All Doctors
            drawText("All Doctors", 320, 70, 24, sf::Color::Cyan);
            drawText("ID | Name | Specialization | Fee", 60, 110, 16, sf::Color::Yellow);
            float y = 140;
            for(int i=0; i<doctorDB.getSize(); i++) {
                Doctor* d = doctorDB.getAt(i);
                char line[255];
                sprintf(line, "%d | %s | %s | %.2f", d->getID(), d->getName(), d->getSpecialization(), d->getFee());
                drawText(line, 60, y, 14, sf::Color::White);
                y += 20; if (y > 480) break;
            }
        } else if (currentChoice == 7) { // View All Appointments
            drawText("All Appointments", 320, 70, 24, sf::Color::Cyan);
            drawText("ID | Patient | Doctor | Date | Time | Status", 60, 110, 16, sf::Color::Yellow);
            float y = 140;
            for(int i=0; i<appointmentDB.getSize(); i++) {
                Appointment* a = appointmentDB.getAt(i);
                Patient* p = patientDB.findByID(a->getPatientID());
                Doctor* d = doctorDB.findByID(a->getDoctorID());
                char line[255];
                sprintf(line, "%d | %s | %s | %s | %s | %s", a->getAppointmentID(), 
                        p ? p->getName() : "Unknown", d ? d->getName() : "Unknown",
                        a->getDate(), a->getTime(), a->getStatus());
                drawText(line, 60, y, 14, sf::Color::White);
                y += 20; if (y > 480) break;
            }
        } else if (currentChoice == 8) { // View Unpaid Bills
            drawText("Unpaid Bills", 320, 70, 24, sf::Color::Cyan);
            drawText("Bill ID | Patient | Amount | Status", 60, 110, 16, sf::Color::Yellow);
            float y = 140;
            for(int i=0; i<billDB.getSize(); i++) {
                Bill* b = billDB.getAt(i);
                if (!stringCompare(b->getStatus(), "Paid")) {
                    Patient* p = patientDB.findByID(b->getPatientID());
                    char line[255];
                    sprintf(line, "%d | %s | %.2f | %s", b->getBillID(), 
                            p ? p->getName() : "Unknown", b->getAmount(), b->getStatus());
                    drawText(line, 60, y, 14, sf::Color::White);
                    y += 20; if (y > 480) break;
                }
            }
         } else if (currentChoice == 9) { // Discharge Patient
            drawText("Discharge Patient", 320, 70, 24, sf::Color::Cyan);
            drawText("Enter Patient ID:", 300, 150, 18, sf::Color::White);
            drawTextBox(formBuffers[0], 300, 200, 200, 40, activeFormField == 1);
            drawButton("DISCHARGE", 300, 300, 200, 50, sf::Color::Magenta);
            if (subState == 96) drawText("Cannot discharge patient with pending appointments.", 180, 370, 16, sf::Color::Red);
            else if (subState == 95) drawText("Cannot discharge patient with unpaid bills.", 180, 370, 16, sf::Color::Red);
        } else if (currentChoice == 11) { // Daily Report
            drawText("Daily Report", 320, 70, 24, sf::Color::Cyan);
            
            time_t t = time(0); char* dt = ctime(&t);
            char today[20];
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
            sprintf(today, "%02d-%02d-%04d", curD, curM, curY);

            int totalToday = 0, completed = 0, pending = 0, noshow = 0, cancelled = 0;
            double revenue = 0;

            for(int i=0; i<appointmentDB.getSize(); i++) {
                Appointment* a = appointmentDB.getAt(i);
                if (stringCompare(a->getDate(), today)) {
                    totalToday++;
                    if (stringCompare(a->getStatus(), "Completed")) completed++;
                    else if (stringCompare(a->getStatus(), "Pending") || stringCompare(a->getStatus(), "Schedualed")) pending++;
                    else if (stringCompare(a->getStatus(), "No-Show")) noshow++;
                    else if (stringCompare(a->getStatus(), "Cancelled")) cancelled++;
                }
            }

            for(int i=0; i<billDB.getSize(); i++) {
                Bill* b = billDB.getAt(i);
                Appointment* a = appointmentDB.findByID(b->getAppointmentID());
                if (a && stringCompare(a->getDate(), today) && stringCompare(b->getStatus(), "Paid")) {
                    revenue += b->getAmount();
                }
            }

            char buf[255];
            sprintf(buf, "Total Today: %d (P:%d C:%d N:%d X:%d)", totalToday, pending, completed, noshow, cancelled);
            drawText(buf, 60, 110, 16, sf::Color::White);
            sprintf(buf, "Revenue Today (Paid Bills): PKR %.2f", revenue);
            drawText(buf, 60, 135, 16, sf::Color::Green);

            drawText("Unpaid Bills (Patient | Total Owed):", 60, 170, 16, sf::Color::Yellow);
            float y = 195;
            for(int i=0; i<patientDB.getSize(); i++) {
                Patient* p = patientDB.getAt(i);
                double owed = 0;
                for(int j=0; j<billDB.getSize(); j++) {
                    if (billDB.getAt(j)->getPatientID() == p->getID() && stringCompare(billDB.getAt(j)->getStatus(), "Unpaid"))
                        owed += billDB.getAt(j)->getAmount();
                }
                if (owed > 0) {
                    sprintf(buf, "%s | PKR %.2f", p->getName(), owed);
                    drawText(buf, 80, y, 14, sf::Color::White); y += 20;
                    if (y > 300) break;
                }
            }

            drawText("Doctor Summary Today (Name | C | P | N):", 60, 320, 16, sf::Color::Yellow);
            y = 345;
            for(int i=0; i<doctorDB.getSize(); i++) {
                Doctor* d = doctorDB.getAt(i);
                int dC = 0, dP = 0, dN = 0;
                for(int j=0; j<appointmentDB.getSize(); j++) {
                    Appointment* a = appointmentDB.getAt(j);
                    if (a->getDoctorID() == d->getID() && stringCompare(a->getDate(), today)) {
                        if (stringCompare(a->getStatus(), "Completed")) dC++;
                        else if (stringCompare(a->getStatus(), "Pending") || stringCompare(a->getStatus(), "Schedualed")) dP++;
                        else if (stringCompare(a->getStatus(), "No-Show")) dN++;
                    }
                }
                if (dC + dP + dN > 0) {
                    sprintf(buf, "%s | %d | %d | %d", d->getName(), dC, dP, dN);
                    drawText(buf, 80, y, 14, sf::Color::White); y += 20;
                    if (y > 480) break;
                }
            }
        } else if (currentChoice == 10) {
            drawText("Security Log", 320, 70, 24, sf::Color::Cyan);
            std::ifstream file("security_log.txt");
            if (file.is_open()) {
                char line[255]; float y = 120;
                while (file.getline(line, 255) && y < 480) { drawText(line, 60, y, 14, sf::Color::White); y += 20; }
                file.close();
            } else drawText("No security events logged.", 300, 200, 18, sf::Color::Red);
        } else {
            drawText("Implementation in progress for this option...", 200, 200, 20, sf::Color::Yellow);
        }
    }
}

void SfmlInterface::renderPatientMenu() {
    Patient* p = patientDB.findByID(loggedInUserID);
    if (!p) return;

    char welcome[100] = "Welcome, ";
    stringAppend(welcome, p->getName());
    drawText(welcome, 50, 20, 24, sf::Color::White);

    char bal[50];
    sprintf(bal, "Balance: PKR %.2f", p->getBalance());
    drawText(bal, 50, 55, 18, sf::Color::Yellow);

    if (currentChoice == 0) {
        drawButton("1. Book Appointment", 50, 120, 330, 45, sf::Color(70, 130, 180));
        drawButton("2. Cancel Appointment", 50, 180, 330, 45, sf::Color(70, 130, 180));
        drawButton("3. View My Appointments", 50, 240, 330, 45, sf::Color(70, 130, 180));
        drawButton("4. View My Medical Records", 50, 300, 330, 45, sf::Color(70, 130, 180));
        drawButton("5. View My Bills", 420, 120, 330, 45, sf::Color(70, 130, 180));
        drawButton("6. Pay Bill", 420, 180, 330, 45, sf::Color(34, 139, 34));
        drawButton("7. Top Up Balance", 420, 240, 330, 45, sf::Color(34, 139, 34));
        drawButton("8. Logout", 50, 510, 700, 45, sf::Color::Red);
    } else {
        drawButton("Back to Menu", 50, 510, 200, 45, sf::Color(100, 100, 100));
        
        if (currentChoice == 1) { // Book Appointment
            if (subState == 0) { // Enter specialization
                drawText("Enter Specialization:", 50, 120, 18, sf::Color::White);
                drawTextBox(formBuffers[0], 250, 115, 300, 35, activeFormField == 1);
                drawButton("SEARCH", 560, 115, 100, 35, sf::Color::Blue);
            } else if (subState == 1) { // Display Doctors
                drawText("Select Doctor:", 50, 100, 18, sf::Color::Cyan);
                float y = 130; bool found = false;
                for(int i=0; i<doctorDB.getSize(); i++) {
                    Doctor* d = doctorDB.getAt(i);
                    if (stringCompareCaseInsensitive(d->getSpecialization(), formBuffers[0])) {
                        char line[255];
                        sprintf(line, "ID: %d | %s | Fee: %.2f", d->getID(), d->getName(), d->getFee());
                        drawText(line, 60, y, 14, sf::Color::White);
                        y += 20; found = true;
                    }
                }
                if (!found) drawText("No doctors available for that specialization.", 100, 200, 18, sf::Color::Red);
                else {
                    drawText("Enter Doctor ID:", 50, 400, 18, sf::Color::White);
                    drawTextBox(formBuffers[1], 200, 395, 100, 35, activeFormField == 2);
                    drawButton("NEXT", 320, 395, 100, 35, sf::Color::Green);
                }
            } else if (subState == 2) { // Enter Date
                drawText("Enter Date (DD-MM-YYYY):", 50, 150, 18, sf::Color::White);
                drawTextBox(formBuffers[2], 300, 145, 200, 35, activeFormField == 3);
                drawButton("VIEW SLOTS", 300, 200, 200, 40, sf::Color::Blue);
            } else if (subState == 3) { // Select Slot
                drawText("Available Slots:", 50, 100, 18, sf::Color::Cyan);
                const char* slots[] = {"09:00", "10:00", "11:00", "12:00", "13:00", "14:00", "15:00", "16:00"};
                float y = 130;
                for(int i=0; i<8; i++) {
                    bool taken = false;
                    for(int j=0; j<appointmentDB.getSize(); j++) {
                        Appointment* a = appointmentDB.getAt(j);
                        if (a->getDoctorID() == atoi(formBuffers[1]) && 
                            stringCompare(a->getDate(), formBuffers[2]) && 
                            stringCompare(a->getTime(), slots[i]) &&
                            !stringCompare(a->getStatus(), "Cancelled")) {
                            taken = true; break;
                        }
                    }
                    if (!taken) {
                        drawText(slots[i], 100, y, 14, sf::Color::Green);
                    } else {
                        drawText(slots[i], 100, y, 14, sf::Color::Red);
                        drawText("(Taken)", 160, y, 14, sf::Color::Red);
                    }
                    y += 25;
                }
                drawText("Enter Time Slot:", 300, 150, 18, sf::Color::White);
                drawTextBox(formBuffers[3], 300, 180, 200, 40, activeFormField == 4);
                drawButton("CONFIRM BOOKING", 300, 250, 200, 50, sf::Color::Green);
            }
        } else if (currentChoice == 3) { // View My Appointments
            drawText("My Appointments", 300, 70, 24, sf::Color::Cyan);
            drawText("ID | Doctor | Date | Time | Status", 60, 110, 16, sf::Color::Yellow);
            float y = 140;
            for(int i=0; i<appointmentDB.getSize(); i++) {
                Appointment* a = appointmentDB.getAt(i);
                if (a->getPatientID() == loggedInUserID) {
                    Doctor* d = doctorDB.findByID(a->getDoctorID());
                    char line[255];
                    sprintf(line, "%d | %s | %s | %s | %s", a->getAppointmentID(), 
                            d ? d->getName() : "Unknown", a->getDate(), a->getTime(), a->getStatus());
                    drawText(line, 60, y, 14, sf::Color::White);
                    y += 20; if (y > 480) break;
                }
            }
        } else if (currentChoice == 2) { // Cancel Appointment
            drawText("Cancel Appointment", 320, 70, 24, sf::Color::Cyan);
            drawText("Pending Appointments:", 60, 110, 18, sf::Color::Yellow);
            float y = 140; int count = 0;
            for(int i=0; i<appointmentDB.getSize(); i++) {
                Appointment* a = appointmentDB.getAt(i);
                if (a->getPatientID() == loggedInUserID && stringCompare(a->getStatus(), "Pending")) {
                    Doctor* d = doctorDB.findByID(a->getDoctorID());
                    char line[255];
                    sprintf(line, "ID: %d | Dr. %s | %s | %s", a->getAppointmentID(), d?d->getName():"N/A", a->getDate(), a->getTime());
                    drawText(line, 80, y, 14, sf::Color::White); y += 20; count++;
                    if (y > 400) break;
                }
            }
            if (count == 0) drawText("You have no pending appointments.", 80, 140, 14, sf::Color::Red);
            
            drawText("Enter Appointment ID to cancel:", 300, 380, 16, sf::Color::White);
            drawTextBox(formBuffers[0], 300, 420, 200, 40, activeFormField == 1);
            drawButton("CONFIRM CANCEL", 300, 470, 200, 45, sf::Color::Red);
            if (subState == 1) drawText("Appointment cancelled successfully.", 250, 520, 14, sf::Color::Green);
            else if (subState == 2) drawText("Invalid appointment ID.", 320, 520, 14, sf::Color::Red);
        } else if (currentChoice == 4) { // Medical Records
            drawText("Medical Records", 300, 70, 24, sf::Color::Cyan);
            drawText("Date | Doctor | Medicines | Notes", 60, 110, 16, sf::Color::Yellow);
            float y = 140;
            for(int i=prescriptionDB.getSize()-1; i>=0; i--) {
                Prescription* pr = prescriptionDB.getAt(i);
                Appointment* ap = appointmentDB.findByID(pr->getAppointmentId());
                if (ap && ap->getPatientID() == loggedInUserID) {
                    Doctor* d = doctorDB.findByID(ap->getDoctorID());
                    char line[512];
                    sprintf(line, "%s | %s | %s | %s", ap->getDate(), d ? d->getName() : "Unknown", pr->getMedicines(), pr->getNotes());
                    drawText(line, 60, y, 14, sf::Color::White);
                    y += 20; if (y > 480) break;
                }
            }
        } else if (currentChoice == 5) { // View My Bills
            drawText("My Bills", 300, 70, 24, sf::Color::Cyan);
            drawText("ID | Appt | Amount | Status", 60, 110, 16, sf::Color::Yellow);
            float y = 140; double total = 0;
            for(int i=0; i<billDB.getSize(); i++) {
                Bill* b = billDB.getAt(i);
                if (b->getPatientID() == loggedInUserID) {
                    char line[255];
                    sprintf(line, "%d | %d | %.2f | %s", b->getBillID(), b->getAppointmentID(), b->getAmount(), b->getStatus());
                    drawText(line, 60, y, 14, sf::Color::White);
                    y += 20; if (stringCompare(b->getStatus(), "Unpaid")) total += b->getAmount();
                }
            }
            char totalLine[100]; sprintf(totalLine, "Total Unpaid: PKR %.2f", total);
            drawText(totalLine, 60, 500, 18, sf::Color::Red);
        } else if (currentChoice == 6) { // Pay Bill
            drawText("Pay Bill", 320, 70, 24, sf::Color::Cyan);
            drawText("Unpaid Bills:", 60, 110, 18, sf::Color::Yellow);
            float y = 140; int count = 0;
            for(int i=0; i<billDB.getSize(); i++) {
                Bill* b = billDB.getAt(i);
                if (b->getPatientID() == loggedInUserID && stringCompare(b->getStatus(), "Unpaid")) {
                    char line[255];
                    sprintf(line, "Bill ID: %d | Appt ID: %d | Amount: PKR %.2f", b->getBillID(), b->getAppointmentID(), b->getAmount());
                    drawText(line, 80, y, 14, sf::Color::White); y += 20; count++;
                    if (y > 400) break;
                }
            }
            if (count == 0) drawText("No unpaid bills.", 80, 140, 14, sf::Color::Red);

            drawText("Enter Bill ID to pay:", 300, 380, 16, sf::Color::White);
            drawTextBox(formBuffers[0], 300, 420, 200, 40, activeFormField == 1);
            drawButton("PAY BILL", 300, 470, 200, 45, sf::Color::Green);
            
            if (subState == 1) {
                Patient* p = patientDB.findByID(loggedInUserID);
                char res[100]; sprintf(res, "Bill paid successfully. Balance: PKR %.2f", p->getBalance());
                drawText(res, 220, 520, 14, sf::Color::Green);
            } else if (subState == 2) drawText("Insufficient funds.", 320, 520, 14, sf::Color::Red);
            else if (subState == 3) drawText("Invalid Bill ID.", 340, 520, 14, sf::Color::Red);
        } else if (currentChoice == 7) { // Top Up Balance
            drawText("Top Up Balance", 320, 70, 24, sf::Color::Cyan);
            drawText("Enter Amount (PKR):", 300, 150, 18, sf::Color::White);
            drawTextBox(formBuffers[0], 300, 180, 200, 40, activeFormField == 1);
            drawButton("ADD FUNDS", 300, 250, 200, 50, sf::Color::Green);
        }
    }
}

void SfmlInterface::renderDoctorMenu() {
    Doctor* d = doctorDB.findByID(loggedInUserID);
    if (!d) return;

    char welcome[100] = "Welcome, Dr. ";
    stringAppend(welcome, d->getName());
    stringAppend(welcome, " | ");
    stringAppend(welcome, d->getSpecialization());
    drawText(welcome, 100, 20, 24, sf::Color::White);

    if (currentChoice == 0) {
        drawButton("1. View Today's Appointments", 200, 100, 400, 45, sf::Color(70, 130, 180));
        drawButton("2. Mark Appointment Complete", 200, 160, 400, 45, sf::Color(70, 130, 180));
        drawButton("3. Mark Appointment No-Show", 200, 220, 400, 45, sf::Color(70, 130, 180));
        drawButton("4. Write Prescription", 200, 280, 400, 45, sf::Color(70, 130, 180));
        drawButton("5. View Patient Medical History", 200, 340, 400, 45, sf::Color(70, 130, 180));
        drawButton("6. Logout", 200, 500, 400, 45, sf::Color::Red);
    } else {
        drawButton("Back to Menu", 50, 510, 200, 45, sf::Color(100, 100, 100));
        
        if (currentChoice == 1) { // View Today's Appointments
            drawText("Today's Appointments", 300, 70, 24, sf::Color::Cyan);
            time_t t = time(0); char* dt = ctime(&t);
            char today[20];
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
            sprintf(today, "%02d-%02d-%04d", curD, curM, curY);
            
            drawText("ID | Patient Name | Time | Status", 60, 110, 16, sf::Color::Yellow);
            float y = 140; bool found = false;
            
            // Manual sorting by time slot ascending
            Appointment* todayAppts[100];
            int todayCount = 0;
            for(int i=0; i<appointmentDB.getSize(); i++) {
                Appointment* a = appointmentDB.getAt(i);
                if (a->getDoctorID() == loggedInUserID && stringCompare(a->getDate(), today)) {
                    todayAppts[todayCount++] = a;
                }
            }
            
            for(int i=0; i<todayCount-1; i++) {
                for(int j=0; j<todayCount-i-1; j++) {
                    if (stringCompare(todayAppts[j]->getTime(), todayAppts[j+1]->getTime()) > 0) {
                        Appointment* temp = todayAppts[j];
                        todayAppts[j] = todayAppts[j+1];
                        todayAppts[j+1] = temp;
                    }
                }
            }

            for(int i=0; i<todayCount; i++) {
                Appointment* a = todayAppts[i];
                Patient* p = patientDB.findByID(a->getPatientID());
                char line[255];
                sprintf(line, "%d | %s | %s | %s", a->getAppointmentID(), 
                        p ? p->getName() : "Unknown", a->getTime(), a->getStatus());
                drawText(line, 60, y, 14, sf::Color::White);
                y += 20; found = true;
            }
            if (!found) drawText("No appointments scheduled for today.", 250, 200, 18, sf::Color::Red);
        } else if (currentChoice == 2 || currentChoice == 3) { // Mark Complete / No-Show
            const char* title = (currentChoice == 2) ? "Mark Appointment Complete" : "Mark Appointment No-Show";
            drawText(title, 320, 70, 24, sf::Color::Cyan);
            drawText("Today's Pending Appointments:", 60, 110, 18, sf::Color::Yellow);
            
            time_t t = time(0); char* dt = ctime(&t);
            char today[20];
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
            sprintf(today, "%02d-%02d-%04d", curD, curM, curY);

            float y = 140; int count = 0;
            for(int i=0; i<appointmentDB.getSize(); i++) {
                Appointment* a = appointmentDB.getAt(i);
                if (a->getDoctorID() == loggedInUserID && stringCompare(a->getDate(), today) && 
                    (stringCompare(a->getStatus(), "Pending") || stringCompare(a->getStatus(), "Schedualed"))) {
                    Patient* p = patientDB.findByID(a->getPatientID());
                    char line[255];
                    sprintf(line, "ID: %d | Patient: %s | Time: %s", a->getAppointmentID(), p?p->getName():"N/A", a->getTime());
                    drawText(line, 80, y, 14, sf::Color::White); y += 20; count++;
                    if (y > 400) break;
                }
            }
            if (count == 0) drawText("No pending appointments for today.", 80, 140, 14, sf::Color::Red);

            drawText("Enter Appointment ID:", 300, 380, 16, sf::Color::White);
            drawTextBox(formBuffers[0], 300, 420, 200, 40, activeFormField == 1);
            const char* btnLabel = (currentChoice == 2) ? "MARK COMPLETE" : "MARK NO-SHOW";
            drawButton(btnLabel, 300, 470, 200, 45, (currentChoice == 2) ? sf::Color::Green : sf::Color::Red);
            
            if (subState == 1) {
                drawText(currentChoice == 2 ? "Appointment marked as completed." : "Appointment marked as no-show.", 250, 520, 14, sf::Color::Green);
            } else if (subState == 2) {
                drawText("Invalid ID or criteria not met.", 320, 520, 14, sf::Color::Red);
            }
        } else if (currentChoice == 4) { // Write Prescription
            drawText("Write Prescription", 320, 70, 24, sf::Color::Cyan);
            drawText("Appt ID:", 50, 120, 18, sf::Color::White); drawTextBox(formBuffers[0], 150, 115, 100, 35, activeFormField == 1);
            drawText("Meds:", 50, 170, 18, sf::Color::White); drawTextBox(formBuffers[1], 150, 165, 500, 35, activeFormField == 2);
            drawText("Notes:", 50, 220, 18, sf::Color::White); drawTextBox(formBuffers[2], 150, 215, 500, 100, activeFormField == 3);
            drawButton("SAVE", 300, 350, 200, 45, sf::Color::Green);
        } else if (currentChoice == 5) { // Medical History
            drawText("Patient History", 320, 70, 24, sf::Color::Cyan);
            drawText("Enter Patient ID:", 100, 120, 18, sf::Color::White);
            drawTextBox(formBuffers[0], 250, 115, 100, 35, activeFormField == 1);
            drawButton("VIEW", 400, 115, 100, 35, sf::Color::Blue);
            
            if (subState == 1) { // Show History
                float y = 180;
                for(int i=prescriptionDB.getSize()-1; i>=0; i--) {
                    Prescription* pr = prescriptionDB.getAt(i);
                    Appointment* ap = appointmentDB.findByID(pr->getAppointmentId());
                    if (ap && ap->getPatientID() == atoi(formBuffers[0]) && ap->getDoctorID() == loggedInUserID) {
                        char line[512];
                        sprintf(line, "Date: %s | Meds: %s", ap->getDate(), pr->getMedicines());
                        drawText(line, 60, y, 14, sf::Color::White);
                        y += 20; if (y > 480) break;
                    }
                }
            }
        }
    }
}

void SfmlInterface::sortAppointmentsByDate(Appointment** arr, int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            // Very simple date comparison YYYY-MM-DD or DD-MM-YYYY
            // Assuming DD-MM-YYYY as per prompt: 21-22 compare 22 is later
            if (stringCompare(arr[j]->getDate(), arr[j+1]->getDate()) > 0) {
                Appointment* temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
}

void SfmlInterface::sortPrescriptionsByDate(Prescription** arr, int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            Appointment* a1 = appointmentDB.findByID(arr[j]->getAppointmentId());
            Appointment* a2 = appointmentDB.findByID(arr[j+1]->getAppointmentId());
            if (a1 && a2 && stringCompare(a1->getDate(), a2->getDate()) < 0) {
                Prescription* temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
}
