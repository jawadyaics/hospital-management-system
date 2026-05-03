#pragma once
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"

#include "storage.h"
#include "patient.h"
#include "doctor.h"
#include "admin.h"
#include "file-handler.h"


enum AppState {
    LOGIN_SCREEN,
    ADMIN_MENU,
    PATIENT_MENU,
    DOCTOR_MENU
};


class SfmlInterface {
    private:
    sf::RenderWindow window;
    sf::Font font;
    AppState currentState;

    Storage<Admin>& adminDB;
    Storage<Patient>& patientDB;
    Storage<Doctor>& doctorDB;
    FileHandler& fileHandler;

    char inputIdBuffer[50];
    char inputPasswordBuffer[50];
    
    int activeInputField;
    int selectedRole;
    int loggedInUserID;

    void drawText(const char* text, float x, float y, int size , sf::Color color);
    void drawButton(const char* text , float x , float y , float width , float height , sf::Color bgColor);
    void drawTextBox(const char* text , float x , float y , float width , float heigth , bool isActive);


    void renderLoginScreen();
    void renderAdminMenu();
    void renderPatientMenu();
    void renderDoctorMenu();

    void handleEvents();
    void processMouseClick(float mouseX , float mouseY);
    void processKeyBoardInput(sf::Uint32 unicodeCode);
    void attemptLogin();

    public:

    SfmlInterface(Storage<Patient>& pDB , Storage<Doctor>& dDB , Storage<Admin>& aDB , FileHandler& fh);

    void run();
};