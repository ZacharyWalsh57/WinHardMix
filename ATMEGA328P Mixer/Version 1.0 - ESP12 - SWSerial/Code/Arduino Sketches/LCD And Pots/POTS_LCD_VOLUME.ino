//This sketch is the exact same thing as it was for the breadboard mixer, but now with the whole project on a breadboard, the code is being recreated
//to run on a ATMEGA328P chip (Essentially an arduino UNO instead of a mega)
//This maeans pins are being changed out along with some small chode enhancements to save up on memory for this damn thing.

//This sketch only proves wired use for now.  Included LCD on 11/8/18

//PIN OUTPUTS:
#define STATUS_LED 13 //LED for status. Hooked onto 13
#define POT_1 A5 //Volume 1
#define POT_2 A4 //Volume 2
#define POT_3 A3 //Volume 3
#define POT_4 A2 //Volume 4

//LCD Definitions
#define LCD_RS 8
#define LCD_RW 7
#define LCD_EN 6

//Data Lines for the LCD itself
#define LCD_D4 9
#define LCD_D5 10
#define LCD_D6 11
#define LCD_D7 12

#include <LiquidCrystal.h> //LCD Lib
LiquidCrystal LCD_DISPLAY(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

void setup() {
    Serial.begin(115200);

    LCD_DISPLAY.begin(16,2);
    delay(3000);
    LCD_DISPLAY.clear();

    pinMode(STATUS_LED, OUTPUT);
    digitalWrite(STATUS_LED, HIGH);

    LCD_DISPLAY.setCursor(3,0);
    LCD_DISPLAY.print("WinHardMix");
    LCD_DISPLAY.setCursor(4,1);
    LCD_DISPLAY.print("STARTING");

    delay(3500);

    LCD_DISPLAY.clear(); 
    LCD_DISPLAY.setCursor(0,0);
    LCD_DISPLAY.print("V1:");

    LCD_DISPLAY.setCursor(9,0);
    LCD_DISPLAY.print("V2:");
    
    LCD_DISPLAY.setCursor(0,1);
    LCD_DISPLAY.print("V3:");

    LCD_DISPLAY.setCursor(9,1);
    LCD_DISPLAY.print("V4:");

    LCD_DISPLAY.setCursor(7,0);
    LCD_DISPLAY.print("|");
    LCD_DISPLAY.setCursor(7,1);
    LCD_DISPLAY.print("|");
    
    Serial.println("HARDMIX10 SERIAL PROCESSING. IF YOU SEE THIS I HOPE YOU KNOW WHAT YOURE DOING...");
}

void loop() {
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
        Serial.print(VOLUMES[SELECTED]);
        Serial.print(" ");
    }
    Serial.println("");

    LCD_DISPLAY.setCursor(4,0);
    LCD_DISPLAY.print("   ");
    LCD_DISPLAY.setCursor(4,0);
    LCD_DISPLAY.print((int)VOLUME_1);

    LCD_DISPLAY.setCursor(13,0);
    LCD_DISPLAY.print("   ");
    LCD_DISPLAY.setCursor(13,0);
    LCD_DISPLAY.print((int)VOLUME_2);

    LCD_DISPLAY.setCursor(4,1);
    LCD_DISPLAY.print("   ");
    LCD_DISPLAY.setCursor(4,1);
    LCD_DISPLAY.print((int)VOLUME_3);

    LCD_DISPLAY.setCursor(13,1);
    LCD_DISPLAY.print("   ");
    LCD_DISPLAY.setCursor(13,1);
    LCD_DISPLAY.print((int)VOLUME_4);
}


