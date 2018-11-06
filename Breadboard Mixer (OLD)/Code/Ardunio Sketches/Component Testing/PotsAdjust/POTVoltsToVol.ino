//This file is going to be used to read a value on the analog pins, and then show it
//in the serial monitor.  This means that we have to assign the pot power pin to output
//and also assign the pot in pins to inputs.

//Pin Definitions:
#define POT_1_2 A4
#define POT_2_2 A5
#define POT_3_2 A6
#define POT_4_2 A7
#define RED_1 2
#define GRN_1 3

int POT_1_VALUE = 0;
int POT_2_VALUE = 0;
int POT_3_VALUE = 0;
int POT_4_VALUE = 0;

void setup() {
    Serial.begin(115200);

    pinMode(RED_1, OUTPUT);
    pinMode(GRN_1, OUTPUT);

    Serial.println("SETTING POT CALIBRATION PIN TO 5V NOW.");
    Serial.println("POT READINGS:");
}

void loop() {
    analogWrite(RED_1, 255);

    //Read the analog values from the pots
    POT_1_VALUE = analogRead(POT_1_2);
    POT_2_VALUE = analogRead(POT_2_2);
    POT_3_VALUE = analogRead(POT_3_2);
    POT_4_VALUE = analogRead(POT_4_2);
    
    //Convert them to a voltage from 0.0v to 5.0v
    float POT_1_V = (POT_1_VALUE * (5/1023.0));
    float POT_2_V = (POT_2_VALUE * (5/1023.0));
    float POT_3_V = (POT_3_VALUE * (5/1023.0));
    float POT_4_V = (POT_4_VALUE * (5/1023.0));

    //Since the range is 0-1023, we divite the analog in by 1023 for a %
    float POT_1_VOL = (POT_1_VALUE / 1023.0) * 100.0;
    float POT_2_VOL = (POT_2_VALUE / 1023.0) * 100.0;
    float POT_3_VOL = (POT_3_VALUE / 1023.0) * 100.0;
    float POT_4_VOL = (POT_4_VALUE / 1023.0) * 100.0;

    //Print out the conversions/raw values.
    Serial.print("Channel 1:    ");
    Serial.print(POT_1_V);
    Serial.print("v - ");
    Serial.print(POT_1_VOL);

    Serial.print("  |  Channel 2:    ");
    Serial.print(POT_2_V);
    Serial.print("v - ");
    Serial.print(POT_2_VOL);

    Serial.print("  |  Channel 3:    ");
    Serial.print(POT_3_V);
    Serial.print("v - ");
    Serial.print(POT_3_VOL);

    Serial.print("  |  Channel 4:    ");
    Serial.print(POT_4_V);
    Serial.print("v - ");
    Serial.println(POT_4_VOL);

    analogWrite(GRN_1, 255);
}