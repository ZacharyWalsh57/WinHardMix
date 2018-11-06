//This sketch makes the 2 LEDs go from bright to dim every 2.5 seconds. 
//LEDs
#define RED_1 2
#define GRN_1 3

void setup() {
    pinMode(RED_1, OUTPUT);
    pinMode(GRN_1, OUTPUT);
}

void loop() {
    analogWrite(RED_1, 255);
    analogWrite(GRN_1, 255);

    delay(2500);

    analogWrite(RED_1, 50);
    analogWrite(GRN_1, 50);

    delay(2500);
}