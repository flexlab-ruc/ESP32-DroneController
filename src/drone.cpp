#include <Arduino.h>
#include <drone.h>
#include <joystick.h>
#include <Position.h>
#include <vector>

Drone::Drone(String ssid, String password)
{
    this->ssid = ssid;
    this->password = password; 
}

void Drone::connect()
{
    Serial.println("Connecting to drone...");
    //Serial.begin(9600);
    WiFi.mode(WIFI_STA);
    WiFi.begin(this->ssid.c_str(), this->password.c_str());
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial.println("WiFi Failed");
        while(1) {
            delay(1000);
        }
    }

    Serial.println("Connected!");

    // listen for state information from drone
    if(udpState.listen(udpPortState)) {
        Serial.print("UDP Listening on IP: ");
        Serial.println(WiFi.localIP());
        udpState.onPacket([this](AsyncUDPPacket packet) {
            //Serial.print("state: ");
            //Serial.write(packet.data(), packet.length());
             String s((char*)packet.data());
            s = s.substring(0, packet.length()); 
            s.trim();
            // send string to method
            this->stateReceiver(s);
        });
    }

    // listen for command reply from drone
    if(udpCommand.listen(udpPortCommand)) {
        Serial.print("UDP Listening on IP: ");
        Serial.println(WiFi.localIP());
        udpCommand.onPacket([this](AsyncUDPPacket packet) {
            // make a string from the data
                String s((char*)packet.data());
                s = s.substring(0, packet.length()); 
                s.trim();
                // send string to method
                this->commandResponse(s);

        });
    }

    IPAddress ipOther = IPAddress(192,168,10,1);
    udpCommand.connect(ipOther, udpPortCommand);

    String reply = "command";
    udpCommand.print(reply.c_str());
}

void Drone::sendCommand(String command)
{
    udpCommand.print(command.c_str());   
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

void Drone::stateReceiver(String state)
{
    this->fillStateArray(state);
    
}

void Drone::fillStateArray(String state)
{
        //Serial.println("received following state: "+ state);
        
        int colon = 0; 
        int semicolon = 0;

        while (state.indexOf(':') != -1) // while there are colons left in state
        {
            colon = state.indexOf(':');
            semicolon = state.indexOf(';');
            
            this->state[state.substring(0,colon)] = state.substring(colon+1, semicolon).toDouble();

            //Serial.print(state.substring(0,colon));
            //Serial.print(" : ");
            //Serial.println(state.substring(colon+1, semicolon).toDouble());
            //Serial.println(state.length());
            
            state = state.substring(semicolon+1);
        }
            

            
    

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
        //Serial.print(joystickPosition.x);
        //Serial.print(" ");
        //Serial.println(joystickPosition.y);
        }
    }
    
    Serial.print("Battery : ");
    Serial.print(this->state["bat"]);
    Serial.print("\tBarometer : ");
    Serial.print(this->state["baro"]);
    Serial.print("agz : ");
    Serial.print(this->state["agz"]);
    Serial.println(); 

    
    // Using joystick methods
    //Serial.print(this->joystick->getX());
    //Serial.print(" ");
    //Serial.println(this->joystick->getY());
    
    
    //sendmessage("joystickPosition")

}
