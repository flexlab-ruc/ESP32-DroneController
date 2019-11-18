#ifndef C8C7268F_7308_4565_BA1F_FDBA5EBBD88F
#define C8C7268F_7308_4565_BA1F_FDBA5EBBD88F

#include <Arduino.h>
#include <string.h>
#include "WiFi.h"
#include "AsyncUDP.h"
#include <ButtonListener.h>
#include <joystick.h>

using namespace std;

class Drone : public ButtonListener
{
    public:
        Drone(String ssid , String password);
        void connect();
        void sendCommand(String commmand);
        void setIp(String ip);
        bool connected;
        String myIp;
        void ButtonPressed();
        void loop();
        Joystick *joystick;
    private:
        void commandResponse(String response);
        void stateReceiver(String state);
        String ssid;
        String password;
        AsyncUDP udpCommand;
        AsyncUDP udpState;
        const int udpPortState = 8890;
        const int udpPortCommand = 8889; 
        String droneIp = "192.168.10.1";
        bool flying = false;
};

#endif /* C8C7268F_7308_4565_BA1F_FDBA5EBBD88F */
