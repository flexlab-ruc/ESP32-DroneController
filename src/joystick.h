#ifndef A812D987_C035_417F_B3E8_E665B02237A4
#define A812D987_C035_417F_B3E8_E665B02237A4

#include <Arduino.h>
#include <Position.h>
#include <ButtonListener.h>

class Joystick 
{
    public:
        Joystick(int pinBtn, int pinX, int pinY);
        Position getPosition();
        void loop();
        void addButtonListener(ButtonListener *btnLsn);
        int getX();
        int getY();
    private:
        int deadZone = 192;
        Position position;
        int pinBtn;
        int pinX;
        int pinY;
        int y;
        int x;
        bool listenerIsSet = false;
        // TODO: change buttonListener to vector of buttonlisteners?
        ButtonListener *buttonListener;
        bool buttonIsPressed = false;
};

#endif /* A812D987_C035_417F_B3E8_E665B02237A4 */
