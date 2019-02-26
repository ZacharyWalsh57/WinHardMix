#include <SPI.h>

#define SCK_LED 13     //Status and SPI LED
#define YLW_GRN_LED 15 //Yellow Green LED
#define ORG_LED 14     //Orange LED

//Four Pot Inputs.
#define POT_1 A5
#define POT_2 A4
#define POT_3 A3
#define POT_4 A2

//Class object to establish SPI communication between the ESP And the ATMEGA Chips
class ESPSlave {
    private:
        uint8_t _ss_pin;

    public:
        ESPSlave(uint8_t pin) : _ss_pin(pin) {}
        void begin() {
            pinMode(_ss_pin, OUTPUT);
            digitalWrite(_ss_pin, HIGH);
        }

        uint32_t readStatus() {
            digitalWrite(_ss_pin, LOW);
            SPI.transfer(0x04);
            uint32_t status = (SPI.transfer(0) | ((uint32_t)(SPI.transfer(0)) << 8) | ((uint32_t)(SPI.transfer(0)) << 16) | ((uint32_t)(SPI.transfer(0)) << 24));
            digitalWrite(_ss_pin, HIGH);
            return status;
        }

        void writeStatus(uint32_t status) {
            digitalWrite(_ss_pin, LOW);
            SPI.transfer(0x01);
            SPI.transfer(status & 0xFF);
            SPI.transfer((status >> 8) & 0xFF);
            SPI.transfer((status >> 16) & 0xFF);
            SPI.transfer((status >> 24) & 0xFF);
            digitalWrite(_ss_pin, HIGH);
        }

        void readData(uint8_t *data) {
            digitalWrite(_ss_pin, LOW);
            SPI.transfer(0x03);
            SPI.transfer(0x00);
            for (uint8_t i = 0; i < 99; i++) {
                data[i] = SPI.transfer(0);
            }
            digitalWrite(_ss_pin, HIGH);
        }

        void writeData(uint8_t *data, size_t len) {
            uint8_t i = 0;
            digitalWrite(_ss_pin, LOW);
            SPI.transfer(0x02);
            SPI.transfer(0x00);
            while (len-- && i < 99) {SPI.transfer(data[i++]);}
            while (i++ < 99) {SPI.transfer(0);}
            digitalWrite(_ss_pin, HIGH);
        }

        String readData() {
            char data[100];
            data[99] = 0;
            readData((uint8_t *)data);
            return String(data);
        }

        void writeData(const char *data) {
            writeData((uint8_t *)data, strlen(data));
        }
};

//Class instance of ESPSlave for talking.
ESPSlave ESP_SLAVE(SS);

//Function to send a message or data to the ESP.
void send(const char *message) {
    Serial.print("[ATMEGA] ");
    Serial.println(message);
    ESP_SLAVE.writeData(message);
    delay(10);
    Serial.print("[ESP8266] ");
    Serial.println(ESP_SLAVE.readData());
    Serial.println();
}

void setup() {
    pinMode(ORG_LED, OUTPUT);
    pinMode(YLW_GRN_LED, OUTPUT);

    digitalWrite(ORG_LED, HIGH);
    digitalWrite(YLW_GRN_LED, HIGH);
    delay(500);
    digitalWrite(YLW_GRN_LED, LOW);

    Serial.begin(115200);
    SPI.begin();
    ESP_SLAVE.begin();

    delay(1000);

    bool ESP_READY = false;    
    while (!ESP_READY) {
        send("ATMEGA STARTING");
        delay(10);
        String READY_MSG_RETURN = "";
        READY_MSG_RETURN = ESP_SLAVE.readData();
        if (READY_MSG_RETURN.equals("ESP WAITING")) {
            ESP_READY = true;
            Serial.println("[DEBUG] ATMEGA SEES ESP READY. MOVING ON");
            digitalWrite(YLW_GRN_LED, HIGH);
            delay(500);
            digitalWrite(YLW_GRN_LED, LOW);
            digitalWrite(ORG_LED, HIGH);
            send("ATMEGA READY");
        }
    }
    
    bool ESP_WEBSERVER = false;
    while(!ESP_WEBSERVER) {
        delay(10);
        String WEBSERVER_MSG = "";
        WEBSERVER_MSG = ESP_SLAVE.readData();
        if (WEBSERVER_MSG.equals("ESP SERVER RUNNING")) {
            ESP_WEBSERVER = true;
            Serial.println("[DEBUG] ESP SERVER UP AND RUNNING!!");
            digitalWrite(ORG_LED, LOW);
            digitalWrite(YLW_GRN_LED, LOW);
            digitalWrite(ORG_LED, HIGH);
            digitalWrite(YLW_GRN_LED, HIGH);
            delay(150);
            digitalWrite(ORG_LED, LOW);
            digitalWrite(YLW_GRN_LED, LOW);
            Serial.println("[DEBUG] ALL GOOD HERE FOLKS");
        }
    }

}

void checkPOSTStatus() {
    String ESP_UPDATE_STATUS = ESP_SLAVE.readData();
    if (ESP_UPDATE_STATUS.equals("UPDATING")) {
        digitalWrite(YLW_GRN_LED, LOW);
    }
    if (ESP_UPDATE_STATUS.equals("UPDATED")) {
        digitalWrite(YLW_GRN_LED, HIGH);
    }
}
void sendAnalogs() {
    //Grab analog vlaues on each pot.
    int ANALOG_1 = analogRead(POT_1);
    int ANALOG_2 = analogRead(POT_2);
    int ANALOG_3 = analogRead(POT_3);
    int ANALOG_4 = analogRead(POT_4);

    //Convert each value to a 0-100 volume level.
    //FIXME: Since I built this like an idiot, the volume is subtracted from 100.
    //FIXME: This is because I pull the value from pin 2 compared to power and not ground on the circuit. 
    float VOLUME_1 = 100.0 - ((ANALOG_1 / 1023.0) * 100);
    float VOLUME_2 = 100.0 - ((ANALOG_2 / 1023.0) * 100);
    float VOLUME_3 = 100.0 - ((ANALOG_3 / 1023.0) * 100);
    float VOLUME_4 = 100.0 - ((ANALOG_4 / 1023.0) * 100);

    //Assemble a list of all four volumes.  Take them and print each to the serial output.
    int VOLUMES[] = {VOLUME_1, VOLUME_2, VOLUME_3, VOLUME_4};
    int SELECTED;

    for (SELECTED = 0; SELECTED < 4; SELECTED = SELECTED + 1) {
        Serial.print("[DATA - ATMEGA] VOLUMES: ");
        Serial.print(VOLUMES[SELECTED]);
        Serial.print(" ");
        
    }
    Serial.println("");
}

void loop() {
    /*

    */
}