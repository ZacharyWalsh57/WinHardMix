#define SCK_LED 13 //Status and SPI LED 
#define YLW_GRN_LED 14 //Yellow Green LED
#define ORG_LED 15 //Organge LED

//This is a setup script used to prove that the ATMEGA328p takes a burn and all resistors are
//assigned correctly. If the script burns and all components work, the LCD will be programmed next

void setup() {
    Serial.begin(115200);
    pinMode(YLW_GRN_LED, OUTPUT);
    pinMode(ORG_LED, OUTPUT);
    pinMode(SCK_LED, OUTPUT);
    Serial.println("RUNNING.....");
    delay(1000);
}

void loop() {
    Serial.println("ALL ON.");
    digitalWrite(SCK_LED, HIGH);
    digitalWrite(ORG_LED, HIGH);
    digitalWrite(YLW_GRN_LED, HIGH);
    delay(1000);

    Serial.println("SEQUENTIAL OFF.");
    digitalWrite(SCK_LED, LOW);
    delay(250);
    digitalWrite(ORG_LED, LOW);
    delay(250);
    digitalWrite(YLW_GRN_LED, LOW);
    delay(1000);

    Serial.println("SEQUENTIAL ON.");
    digitalWrite(SCK_LED, HIGH);
    delay(250);
    digitalWrite(ORG_LED, HIGH);
    delay(250);
    digitalWrite(YLW_GRN_LED, HIGH);
    delay(1000);

    Serial.println("ALL OFF");
    digitalWrite(SCK_LED, LOW);
    digitalWrite(ORG_LED, LOW);
    digitalWrite(YLW_GRN_LED, LOW);
}