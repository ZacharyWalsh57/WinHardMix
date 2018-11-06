//LEDs
#define RED_1 2
#define GRN_1 3

//Volume Pots
#define POT_1_2 A4
#define POT_2_2 A5
#define POT_3_2 A6
#define POT_4_2 A7

//LCD Screen
#define LCD_RS 8
#define LCD_ENABLE 9
#define LCD_DATA_4 46
#define LCD_DATA_5 48
#define LCD_DATA_6 50
#define LCD_DATA_7 52
#define LCD_CONTRAST 11

#include <LiquidCrystal.h>
LiquidCrystal LCD_DISPLAY(LCD_RS, LCD_ENABLE, LCD_DATA_4, LCD_DATA_5, LCD_DATA_6, LCD_DATA_7);

int POT_1_VALUE = 0;
int POT_2_VALUE = 0;
int POT_3_VALUE = 0;
int POT_4_VALUE = 0;

void setup() {
    Serial.begin(115200);

    pinMode(RED_1, OUTPUT);
    pinMode(GRN_1, OUTPUT);
    pinMode(LCD_CONTRAST, OUTPUT);

    analogWrite(RED_1, 255);
    analogWrite(LCD_CONTRAST, 1);

    LCD_DISPLAY.begin(16,2);
    delay(3000);
    LCD_DISPLAY.clear();
    
    LCD_DISPLAY.setCursor(3,0);
    LCD_DISPLAY.print("WinHardMix");
    LCD_DISPLAY.setCursor(4,1);
    LCD_DISPLAY.print("STARTING");
    
    delay(3500);
    analogWrite(GRN_1, 255);
    
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
}

void loop() {
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

    //Print out the conversions/raw values to the serial Monitor and LCD.
    //TODO: Add in a way of checking if the value is +/- 2 of the last value to stop the LCD flicker.
    //New variables called POT_X_VOL_LAST and compare it to POT_X_VOL.
    Serial.print("Channel 1:    ");
    Serial.print(POT_1_V);
    Serial.print("v - ");
    Serial.print(POT_1_VOL);

    LCD_DISPLAY.setCursor(4,0);
    LCD_DISPLAY.print("   ");
    LCD_DISPLAY.setCursor(4,0);
    LCD_DISPLAY.print((int)(POT_1_VOL));

    Serial.print("  |  Channel 2:    ");
    Serial.print(POT_2_V);
    Serial.print("v - ");
    Serial.print(POT_2_VOL);

    LCD_DISPLAY.setCursor(13,0);
    LCD_DISPLAY.print("   ");
    LCD_DISPLAY.setCursor(13,0);
    LCD_DISPLAY.print((int)POT_2_VOL);

    Serial.print("  |  Channel 3:    ");
    Serial.print(POT_3_V);
    Serial.print("v - ");
    Serial.print(POT_3_VOL);

    LCD_DISPLAY.setCursor(4,1);
    LCD_DISPLAY.print("   ");
    LCD_DISPLAY.setCursor(4,1);
    LCD_DISPLAY.print((int)POT_3_VOL);

    Serial.print("  |  Channel 4:    ");
    Serial.print(POT_4_V);
    Serial.print("v - ");
    Serial.println(POT_4_VOL);

    LCD_DISPLAY.setCursor(13,1);
    LCD_DISPLAY.print("   ");
    LCD_DISPLAY.setCursor(13,1);
    LCD_DISPLAY.print((int)POT_4_VOL);

    delay(100);
}