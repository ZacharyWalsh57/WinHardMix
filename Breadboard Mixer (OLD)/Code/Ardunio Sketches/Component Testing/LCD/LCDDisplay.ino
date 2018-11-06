//LCD Screen
#define LCD_RS 8
#define LCD_ENABLE 9
#define LCD_DATA_4 46
#define LCD_DATA_5 48
#define LCD_DATA_6 50
#define LCD_DATA_7 52
#define LCD_CONTRAST 11
#define RED_1 2
#define GRN_1 3

#include <LiquidCrystal.h>
LiquidCrystal LCD_DISPLAY(LCD_RS, LCD_ENABLE, LCD_DATA_4, LCD_DATA_5, LCD_DATA_6, LCD_DATA_7);

void setup() {
    Serial.begin(115200);
    pinMode(RED_1, OUTPUT);
    pinMode(GRN_1, OUTPUT);
    pinMode(LCD_CONTRAST, OUTPUT);

    analogWrite(LCD_CONTRAST, 1);
    LCD_DISPLAY.begin(16,2);
    LCD_DISPLAY.clear();
    delay(1200);

    LCD_DISPLAY.setCursor(3,0);
    LCD_DISPLAY.print("WinHardMix");
    LCD_DISPLAY.setCursor(4,1);
    LCD_DISPLAY.print("STARTING");
    
    delay(3500);
    
    digitalWrite(RED_1, HIGH);
    digitalWrite(GRN_1, HIGH);
    
    LCD_DISPLAY.clear();
    LCD_DISPLAY.setCursor(0,0);
    LCD_DISPLAY.print("Displaying Time:");
    
}

void loop() {
    LCD_DISPLAY.setCursor(0,1);
    LCD_DISPLAY.print(millis() / 1000);
    Serial.println(millis()/1000);
}
