#include <Arduino.h>
#include <drone.h>
#include <joystick.h>
#include <Position.h>

Drone::Drone(String ssid, String password)
{
    this->ssid = ssid;
    this->password = password; 
}

void Drone::connect()
{
    Serial.println("drone begin");
    //Serial.begin(9600);
    WiFi.mode(WIFI_STA);
    WiFi.begin(this->ssid.c_str(), this->password.c_str());
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial.println("WiFi Failed");
        while(1) {
            delay(1000);
        }
    }
    if(udp.listen(udpPort)) {
        this->myIp = WiFi.localIP().toString();
        Serial.print("UDP Listening on IP: ");
        Serial.println(WiFi.localIP());
      
        udp.onPacket(
            [this](AsyncUDPPacket packet) -> void
            {
                // make a string from the data
                String s((char*)packet.data());
                s = s.substring(0, packet.length()); 
                s.trim();
                // send string to method
                this->commandResponse(s);
            }
        );
    }
}

void Drone::sendCommand(String command)
{
    udpSender.beginPacket(this->droneIp.c_str(), udpPort);
    udpSender.printf(command.c_str());
    udpSender.endPacket();    
}

void Drone::setIp(String ip)
{
    this->droneIp = ip;
}
        
void Drone::commandResponse(String response)
{
    Serial.print("got following response: ");
    Serial.println(response.c_str());
    Serial.print("message length: ");
    Serial.println(response.length());
}

void Drone::ButtonPressed()
{
    if (flying == false)
    {
        this->flying = true;
        Serial.println("takeoff");
        this->sendCommand("takeoff");
    }
    else
    {
        Serial.println("land");
        this->sendCommand("land");
        this->flying = false;
    }
}

void Drone::loop()
{
    
    // Using Position object to retrieve information
    
    Position joystickPosition = this->joystick->getPosition();
    
    if (joystickPosition.x != 0)
    {
        if (joystickPosition.y != 0)
        {
        Serial.print(joystickPosition.x);
        Serial.print(" ");
        Serial.println(joystickPosition.y);
        }
    }
    
    
    /*
    // Using joystick methods
    Serial.print(this->joystick->getX());
    Serial.print(" ");
    Serial.println(this->joystick->getY());
    */
    
    //sendmessage("joystickPosition")

}
