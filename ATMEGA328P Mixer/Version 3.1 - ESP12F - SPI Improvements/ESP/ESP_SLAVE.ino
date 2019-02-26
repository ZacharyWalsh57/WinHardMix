#include "SPISlave.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#define LED_GREEN 5
#define LED_YELLOW 16
#define LED_BUILTIN 2

//TODO: Set this up so that the user can feed serial data to the ESP for an AP and password.
const char *ssid = "39GreystoneRd";
const char *password = "zW07059801";

//Char buffers for the ATMEGA and ESP Status strings.
char ATMEGA_STATUS[30];
char ESP_STATUS[30];

//Boolean to stop multiple webserver start requests. 
bool WEBSERVER_RUNNING = false;
bool STARTUP = true;

//Webhost class object.
ESP8266WebServer BACK_END(80);

//Method used for processing client requests on "/"
void handleRoot() {
    if (WEBSERVER_RUNNING) {
        SPISlave.setData("UPDATING");
        Serial.print("[INFO] Updating at: ");
        Serial.print(millis());
        Serial.println();

        char temp[2000];
        int sec = millis() / 1000;
        int min = sec / 60;
        int hr = min / 60;

        snprintf(temp, 2000, "<html>\
        <head>\
        <meta charset=\"utf-8\">\
        <title>WinHardMix Back End</title?></title>\
        </head>\
        <body>\
            <h1 style=\"font-family: Segoe, \'Segoe UI\', \'DejaVu Sans\', \'Trebuchet MS\', Verdana, sans-serif; text-align: center; color: #109B0F; font-weight: 400; font-style: oblique;\">WinHardMix V3.0 - Now Wireless!</h1>\
            <h2 style=\"font-family: Segoe, \'Segoe UI\', \'DejaVu Sans\', \'Trebuchet MS\', Verdana, sans-serif; font-weight: 300; text-align: center;\">System Uptime and Component States</h2><ul><table width=\"500\" border=\"05\" align=\"center\">\
            <tbody>\
                <tr>\
                    <td align=\"left\" style=\"text-align: left; font-family: Segoe, \'Segoe UI\', \'DejaVu Sans\', \'Trebuchet MS\', Verdana, sans-serif;\" scope=\"col\">System Uptime&nbsp;</td>\
                    <th scope=\"col\">%02d:%02d:%02d&nbsp;</th>\
                </tr>\
                <tr>\
                    <td style=\"font-family: Segoe, \'Segoe UI\', \'DejaVu Sans\', \'Trebuchet MS\', Verdana, sans-serif\">ATMEGA 328P&nbsp;</td>\
                    <td style=\"font-family: Segoe, \'Segoe UI\', \'DejaVu Sans\', \'Trebuchet MS\', Verdana, sans-serif\">STATUS: %s&nbsp;</td>\
                </tr>\
                <tr style=\"font-family: Segoe, \'Segoe UI\', \'DejaVu Sans\', \'Trebuchet MS\', Verdana, sans-serif\">\
                    <td style=\"font-family: Segoe, \'Segoe UI\', \'DejaVu Sans\', \'Trebuchet MS\', Verdana, sans-serif\">ESP8266&nbsp;</td>\
                    <td align=\"left\">STATUS: %s&nbsp;</td>\
                </tr>\
            </tbody>\
        </table>\
        </ul>\
        </body>\
        </html>\ ", hr, min % 60, sec % 60, ATMEGA_STATUS, ESP_STATUS);
        
        BACK_END.send(200, "text/html", temp);
        SPISlave.setData("UPDATED");
    }

    else {
        Serial.println("[ERROR] ESP HOST NOT RUNNING!!!!! TRYING TO REBOOT HOST.");
    }  
}

void handleNotFound() {
    digitalWrite(LED_YELLOW, HIGH);
    String message = "File Not Found\n\n";
    message += "URI: ";
    message += BACK_END.uri();
    message += "\nMethod: ";
    message += (BACK_END.method() == HTTP_GET) ? "GET" : "POST";
    message += "\nArguments: ";
    message += BACK_END.args();
    message += "\n";

    for (uint8_t i = 0; i < BACK_END.args(); i++) {
        message += " " + BACK_END.argName(i) + ": " + BACK_END.arg(i) + "\n";
    }

    BACK_END.send(404, "text/plain", message);
}

void setup() {
    Serial.begin(115200); //Serial Output
    Serial.setDebugOutput(false); //Debug Level
    //Pinmodes for LEDs.
    pinMode(LED_GREEN, OUTPUT); 

    //Wait for everything to settle down now.
    Serial.print("\n[INFO] BACKEND LOW LEVEL DEBUG INFO. IF YOU SEE THIS RIGHT NOW OH BOY.");
    delay(1000);

    //Actually Start the back end server now.
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.println("");

    //Wait for a valid connection to be created.
    Serial.print("[DEBUG] CONNECTING TO WiFI NETWORK. ");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");

    //Inform of active DNS registration.  
    Serial.print("[ESP8266] SSID: ");
    Serial.println(ssid);
    Serial.print("[ESP8266] IP ADDRESS: ");
    Serial.println(WiFi.localIP());

    //DNS responder up and running for requests to be processed from clients.
    //This MUST PASS for the server to run.  
    if (MDNS.begin("esp8266")) {
        Serial.println("[ESP8266] MDNS RESPONDER RUNNING.");
        
        //Inform the user we (by the grace of the non-existent gods) have a webpage going.
        Serial.println("[ESP8266] STARTED BACK END");
        digitalWrite(LED_GREEN, HIGH);

        //Set the SPI data to tell the ATMEGA to start sending more info. 
        Serial.println("[DEBUG] WHOOOOOO. ESP SERVER UP AND RUNNING. LETS GO GRAB A BREWSKI.");

        //Set the status of the ESP for the webpage status info. 
        String ESP_STATUS_STRING = "ESP READY TO POST INFO";

        //This is fuckin stupid but since we don't know the string lenght at runtime they need to be assumed less than 30. 
        //keep this in mind for future commands. 
        ESP_STATUS_STRING.toCharArray(ESP_STATUS, 30);

        //Here's that boolean used to stop multiple web host startup commands. 
        WEBSERVER_RUNNING = true;
    }
    
    //Declare what we should do when the server gets a new client on it.  
    BACK_END.on("/", handleRoot);
    BACK_END.onNotFound(handleNotFound);
    BACK_END.begin();

    //On data input from the ATMEGA.
    SPISlave.onData([](uint8_t * data, size_t len) {
        String message = String((char *)data);
        (void) len;
        if (STARTUP && WEBSERVER_RUNNING) {
            if (message.equals("ATMEGA STARTING")) {
                Serial.println("[ATMEGA] ATMEGA STARTING....");
                SPISlave.setData("ESP WAITING");
            } 

            else if (message.equals("ATMEGA READY")) {
                Serial.println("[ATMEGA] ATMEGA READY COMMAND RECIEVED.");
                if (WEBSERVER_RUNNING){
                    //Only if the server is working do we keep going.
                    SPISlave.setData("ESP SERVER RUNNING.");
                    //See above at line String ESP_STATUS_STRING for explination. 
                    String ATMEGA_STATUS_STRING = "ATMEGA READY TO SEND DATA";
                    ATMEGA_STATUS_STRING.toCharArray(ATMEGA_STATUS, 30);
                    STARTUP = false;
                } 
                else {Serial.println("[ERROR] WEBSERVER IS NOT WORKING!!! REBOOT.");}
            }

            else { //Bad SPI communications. 
                SPISlave.setData("UNKNOWN RESPONSE");
                Serial.println("UNKNOWN COMMAND GIVEN! : ");
                Serial.print(message);
            }
        }

        else {
            //INSERT FOR WEBSERVER ERROR.
        }
    });

    SPISlave.begin();

    SPISlave.setData("ESP READY");

    //Hang out a second to read debug logging. 
    delay(5000);
}

void loop() {
    BACK_END.handleClient();
    
}

