//Volume Pots
#define POT_1_2 A5
#define POT_2_2 A4
#define POT_3_2 A3
#define POT_4_2 A2

int RED_LED = 13;
int POT_1_VALUE = 0;
int POT_2_VALUE = 0;
int POT_3_VALUE = 0;
int POT_4_VALUE = 0;

void setup() {
    Serial.begin(115200);

    pinMode(RED_LED, OUTPUT);
    digitalWrite(RED_LED, HIGH);
    delay(500);
    digitalWrite(RED_LED, LOW);
}

void loop() {
    digitalWrite(RED_LED, LOW);
    //Read the analog values from the pots
    POT_1_VALUE = analogRead(POT_1_2);
    POT_2_VALUE = analogRead(POT_2_2);
    POT_3_VALUE = analogRead(POT_3_2);
    POT_4_VALUE = analogRead(POT_4_2);
    
    //TODO: FIX CONVERSION LOGIC ON POT_2_2.  
    //WILL NOT SCALE CORRECTLY. POTENTIALLY BAD GROUND. 

    //Convert them to a voltage from 0.0v to 5.0v
    float POT_1_V = (POT_1_VALUE * (5/1023.0));
    float POT_2_V = (POT_2_VALUE * (5/1023.0));
    float POT_3_V = (POT_3_VALUE * (5/1023.0));
    float POT_4_V = (POT_4_VALUE * (5/1023.0));

    //Since the range is 0-1023, we divite the analog in by 1023 for a %
    float POT_1_VOL = 100 - ((POT_1_VALUE / 1023.0) * 100.0);
    float POT_2_VOL = 100 - ((POT_2_VALUE / 1023.0) * 100.0);
    float POT_3_VOL = 100 - ((POT_3_VALUE / 1023.0) * 100.0);
    float POT_4_VOL = 100 - ((POT_4_VALUE / 1023.0) * 100.0);

    int POT_VOLUME[] = {POT_1_VOL, POT_2_VOL, POT_3_VOL, POT_4_VOL};
    int COUNTER;
    for (COUNTER = 0; COUNTER < 4; COUNTER = COUNTER + 1) {
        Serial.print(POT_VOLUME[COUNTER]);
        Serial.print(" ");
    }
    Serial.println();
    digitalWrite(RED_LED, HIGH);
}