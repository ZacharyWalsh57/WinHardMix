#define ESP_TX 13
#define ESP_RX 12

#define COMMS_LED 14
SoftwareSerial ATMEGA(ESP_RX, ESP_TX);

void setup() {
    ATMEGA.begin(115200);
    Serial.begin(115200);
    pinMode(COMMS_LED, OUTPUT);

    digitalWrite(COMMS_LED, HIGH);
    delay(2000);
    digitalWrite(COMMS_LED, LOW);
    delay(2000);
    Serial.println("SERIAL_TEST");
}