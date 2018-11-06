//This sketch is the exact same thing as it was for the breadboard mixer, but now with the whole project on a breadboard, the code is being recreated
//to run on a ATMEGA328P chip (Essentially an arduino UNO instead of a mega)
//This maeans pins are being changed out along with some small chode enhancements to save up on memory for this damn thing.

//This sketch is ONLY FOR THE POTS.  The LCD will be added later on.

//PIN OUTPUTS:
#define STATUS_LED 19 //LED for status. Hooked onto 19
#define POT_1 A5 //Volume 1
#define POT_2 A4 //Volume 2
#define POT_3 A3 //Volume 3
#define POT_4 A2 //Volume 4


//Variables to hold the POT values in a 0-1023 value.
int ANALOG_1 = 0;
int ANALOG_2 = 0;
int ANALOG_3 = 0;
int ANALOG_4 = 0;

void setup() {
    Serial.begin(115200);
    
    pinMode(STATUS_LED, OUTPUT);
    digitalWrite(STATUS_LED, HIGH);
    
    Serial.println("HARDMIX10 SERIAL PROCESSING. IF YOU SEE THIS I HOPE YOU KNOW WHAT YOURE DOING...");
}

void loop() {
    //Grab analog vlaues on each pot.
    ANALOG_1 = analogRead(POT_1);
    ANALOG_2 = analogRead(POT_2);
    ANALOG_3 = analogRead(POT_3);
    ANALOG_4 = analogRead(POT_4);

    //Convert each value to a 0-100 Volume level. 
    float VOLUME_1 = ((ANALOG_1 * (5/1023.0)) / 1023.0) * 100;
    float VOLUME_2 = ((ANALOG_2 * (5/1023.0)) / 1023.0) * 100;
    float VOLUME_3 = ((ANALOG_3 * (5/1023.0)) / 1023.0) * 100;
    float VOLUME_4 = ((ANALOG_1 * (5/1023.0)) / 1023.0) * 100;


}

