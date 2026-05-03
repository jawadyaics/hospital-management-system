#include "sfml-interface.h"
#include <iostream>
#include <cstdlib>


bool StringCompare(const char* str1, const char* str2) {
    int i = 0;
    while (str1[i] != '\0' && str2[i] != '\0') {
        if (str1[i] != str2[i]) return false;
        i++;
    }
    return str1[i] == '\0' && str2[i] == '\0';
}



SfmlInterface::SfmlInterface(Storage<Patient>& pDB, Storage<Doctor>& dDB, Storage<Admin>& aDB, FileHandler& fh)
    : patientDB(pDB), doctorDB(dDB), adminDB(aDB), fileHandler(fh) 
{
    // Create an 800x600 window
    window.create(sf::VideoMode(800, 600), "MediCore Hospital Management System");
    window.setFramerateLimit(60);

    if (!font.loadFromFile("font.ttf")) {
        std::cout << "ERROR: Could not load font.ttf! Please add a font file to your folder.\n";
    }

    // Initialize state and variables
    currentState = LOGIN_SCREEN;
    activeInputField = 0;
    selectedRole = 1; // Default to Patient
    loggedInUserID= -1;

    // Clear input buffers
    inputIdBuffer[0] = '\0';
    inputPasswordBuffer[0] = '\0';
}

void SfmlInterface::run(){
    while(window.isOpen()){
        handleEvents();

        window.clear(sf::Color(20,30,50));

        if(currentState == LOGIN_SCREEN){
            renderLoginScreen();
        }
        else if (currentState == PATIENT_MENU){
            renderPatientMenu();
        }
        else if (currentState == DOCTOR_MENU){
            renderDoctorMenu();
        }
        else if (currentState == ADMIN_MENU){
            renderAdminMenu();
        }

        window.display();
    }
}


void SfmlInterface::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        else if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                processMouseClick(event.mouseButton.x, event.mouseButton.y);
            }
        }
        else if (event.type == sf::Event::TextEntered) {
            processKeyBoardInput(event.text.unicode);
        }
    }
}


void SfmlInterface::processMouseClick(float mouseX, float mouseY) {
    if (currentState == LOGIN_SCREEN) {
        // 1. Check Role Selection Buttons (Y: 200)
        if (mouseY >= 200 && mouseY <= 240) {
            if (mouseX >= 200 && mouseX <= 300) selectedRole = 1; // Patient
            if (mouseX >= 350 && mouseX <= 450) selectedRole = 2; // Doctor
            if (mouseX >= 500 && mouseX <= 600) selectedRole = 3; // Admin
        }

        // 2. Check Text Boxes (ID at Y: 300, Password at Y: 400)
        activeInputField = 0; // Reset active field
        if (mouseX >= 300 && mouseX <= 500) {
            if (mouseY >= 300 && mouseY <= 340) activeInputField = 1; // Clicked ID box
            if (mouseY >= 400 && mouseY <= 440) activeInputField = 2; // Clicked Pass box
        }

        // 3. Check Login Button (Y: 500)
        if (mouseX >= 300 && mouseX <= 500 && mouseY >= 500 && mouseY <= 550) {
            attemptLogin();
        }
    }
}

void SfmlInterface::processKeyBoardInput(sf::Uint32 unicodeCode) {
    if (activeInputField == 0) return; // No box is selected

    char* targetBuffer = (activeInputField == 1) ? inputIdBuffer : inputPasswordBuffer;
    
    // Find current length of the buffer manually
    int len = 0;
    while (targetBuffer[len] != '\0') {
        len++;
    }

    // Handle Backspace (ASCII 8)
    if (unicodeCode == 8) {
        if (len > 0) {
            targetBuffer[len - 1] = '\0';
        }
    } 
    // Handle standard typable characters (ASCII 32 to 126)
    else if (unicodeCode >= 32 && unicodeCode <= 126) {
        if (len < 49) { // Ensure we don't overflow the size 50 buffer
            targetBuffer[len] = static_cast<char>(unicodeCode);
            targetBuffer[len + 1] = '\0'; // Null-terminate!
        }
    }
}



void SfmlInterface::attemptLogin() {
    int id = atoi(inputIdBuffer); // Convert text ID to integer

    if (selectedRole == 1) { // PATIENT LOGIN
        Patient* p = patientDB.findByID(id);
        if (p != nullptr && StringCompare(p->getPassword(), inputPasswordBuffer)) {
            loggedInUserID = id;
            currentState = PATIENT_MENU;
        } else {
            std::cout << "Login Failed: Invalid Patient ID or Password\n";
        }
    } 
    else if (selectedRole == 2) { // DOCTOR LOGIN
        Doctor* d = doctorDB.findByID(id);
        if (d != nullptr && StringCompare(d->getPassword(), inputPasswordBuffer)) {
            loggedInUserID = id;
            currentState = DOCTOR_MENU;
        } else {
            std::cout << "Login Failed: Invalid Doctor ID or Password\n";
        }
    } 
    else if (selectedRole == 3) { // ADMIN LOGIN
        Admin* a = adminDB.findByID(id);
        if (a != nullptr && StringCompare(a->getPassword(), inputPasswordBuffer)) {
            loggedInUserID = id;
            currentState = ADMIN_MENU;
        } else {
            std::cout << "Login Failed: Invalid Admin ID or Password\n";
        }
    }
}



void SfmlInterface::drawText(const char* textStr, float x, float y, int size, sf::Color color) {

    sf::Text text(textStr, font, size);
    text.setPosition(x, y);
    text.setFillColor(color);
    window.draw(text);
}

void SfmlInterface::drawButton(const char* textStr, float x, float y, float width, float height, sf::Color bgColor) {
    sf::RectangleShape btn(sf::Vector2f(width, height));
    btn.setPosition(x, y);
    btn.setFillColor(bgColor);
    window.draw(btn);
    
    // Center text somewhat roughly in the button
    drawText(textStr, x + 10, y + 5, 20, sf::Color::White);
}

void SfmlInterface::drawTextBox(const char* textStr, float x, float y, float width, float height, bool isActive) {
    sf::RectangleShape box(sf::Vector2f(width, height));
    box.setPosition(x, y);
    
    // Highlight yellow if active, otherwise white
    box.setFillColor(isActive ? sf::Color(255, 255, 200) : sf::Color::White);
    box.setOutlineColor(sf::Color::Black);
    box.setOutlineThickness(2);
    window.draw(box);

    drawText(textStr, x + 5, y + 5, 24, sf::Color::Black);
}



void SfmlInterface::renderLoginScreen() {
    drawText("MediCore - System Login", 250, 50, 30, sf::Color::White);

    drawText("Select Role:", 350, 150, 20, sf::Color::White);

    // Draw Role Buttons (Highlight the selected one in Green)
    drawButton("Patient", 200, 200, 100, 40, selectedRole == 1 ? sf::Color::Green : sf::Color(100, 100, 100));
    drawButton("Doctor",  350, 200, 100, 40, selectedRole == 2 ? sf::Color::Green : sf::Color(100, 100, 100));
    drawButton("Admin",   500, 200, 100, 40, selectedRole == 3 ? sf::Color::Green : sf::Color(100, 100, 100));

    // Draw Input Fields
    drawText("Enter ID:", 300, 270, 20, sf::Color::White);
    drawTextBox(inputIdBuffer, 300, 300, 200, 40, activeInputField == 1);

    drawText("Password:", 300, 370, 20, sf::Color::White);
    // For a real app, you might want to print '*' instead of the real password characters here!
    drawTextBox(inputPasswordBuffer, 300, 400, 200, 40, activeInputField == 2);

    // Draw Login Button
    drawButton("LOGIN", 300, 500, 200, 50, sf::Color(50, 150, 250));
}

void SfmlInterface::renderPatientMenu() {
    drawText("Patient Dashboard", 300, 50, 30, sf::Color::White);
    drawText("Successfully Logged In!", 300, 150, 20, sf::Color::Green);
    // You will expand this later with buttons for "Book Appointment", "View Bills", etc.
}

void SfmlInterface::renderDoctorMenu() {
    drawText("Doctor Dashboard", 300, 50, 30, sf::Color::White);
    drawText("Successfully Logged In!", 300, 150, 20, sf::Color::Green);
}

void SfmlInterface::renderAdminMenu() {
    drawText("Admin Dashboard", 300, 50, 30, sf::Color::White);
    drawText("Successfully Logged In!", 300, 150, 20, sf::Color::Green);
}