#define LED_SERIAL_EXISTS 12
#define LED_SERIAL_NA 13

const byte CHAR_COUNT = 32; //This is a buffer for the serial input.
//This takes 100 max each (12) plus a start and stop delim so 20 total. 
char CHARS_IN[CHAR_COUNT]; //This is where we hold the new chars with their markers.
boolean NEW_DATA = false; //When we hit new data or the stop and start line char, flip this.

void setup() {
    //Open a Serial Monitor and pinMode LEDs.
    Serial.begin(115200);
    pinMode(LED_SERIAL_EXISTS, OUTPUT);
    pinMode(LED_SERIAL_NA, OUTPUT);
    
    //Test both LEDs.
    digitalWrite(LED_SERIAL_EXISTS, HIGH);
    digitalWrite(LED_SERIAL_NA, HIGH);
    delay(1000);
    digitalWrite(LED_SERIAL_EXISTS, LOW);
    digitalWrite(LED_SERIAL_NA, LOW);
    delay(1000);
}

void loop() {
    RECEIVE_DATA();
    CHECK_DATA();
}

void RECEIVE_DATA() {
    static boolean RX_IN_PROGRESS = false; //Indicate if we are reading.
    static byte NDX = 0; //New byte
    char START_MARKER = '<'; //Delim for start
    char END_MARKER = '>'; //Delim for stop
    char RC; //Received char
 
    while (Serial.available() > 0 && NEW_DATA == false) {
        RC = Serial.read();
        if (RX_IN_PROGRESS == true) {
            digitalWrite(LED_SERIAL_EXISTS, HIGH);
            digitalWrite(LED_SERIAL_NA, LOW);
            if (RC != END_MARKER) {
                CHARS_IN[NDX] = RC;
                NDX++;
                if (NDX >= CHAR_COUNT) {
                    NDX = CHAR_COUNT - 1;
                }
            }
            else {
                CHARS_IN[NDX] = '\0'; // terminate the string
                RX_IN_PROGRESS = false;
                NDX = 0;
                NEW_DATA = true;
            }
        }
        else if (RC == START_MARKER) {
            digitalWrite(LED_SERIAL_EXISTS, LOW);
            digitalWrite(LED_SERIAL_NA, HIGH);
            RX_IN_PROGRESS = true;
        }
    }
}

void CHECK_DATA() {
    if (NEW_DATA == true) {
        NEW_DATA = false;
    }
}