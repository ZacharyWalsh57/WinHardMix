//This file is a list of all the pins which are being mapped for the ATMEGA2650 based board.
//The pins are mapped as follows reading "Actual Name" --> Pin Number -- Define Call Variable.

//LEDs: (Analog Outputs) 
//  Red --> Pin D2-- RED_1       Used to indicate power is being supplied correctly at the correct voltage.
//  Green --> Pin D3 -- GRN_1     Used to show everything is working normally.  
//
//  The second pins of the LEDs are all run to GND

//Potentiometers: (Analog Inputs)
//  Channel 1 --> Pin A7 -- POT_1_2 
//  Channel 2 --> Pin A6 -- POT_2_2
//  Channel 3 --> Pin A5 -- POT_3_2
//  
//  For each Potentiometer, Pin 1 is connected to D44  
//  The D44 pin is used to calibrate the voltage to volume ratio.
//  Calibration:
//      This is done by sweeping from 0-5V with each Potentiometer set to min resistance.
//      The voltage on each pot is recorded and assigned as a MAX Volume for a MAX voltage.
//      This ensures the knobs all adjust the volume the same amount per turn.

//Mute Button (Digital Input)
//  Button 1 --> A15 -- BTN_OUT
//
//  The second pin on the button is connected directly to 5v and A15 reads the voltage constantly to look
//  for when the button is pressed.

//LCD Screen (D0-3 )
//  VSS -- GND -- N/A
//  VDD -- 5v -- N/A
//  RS -- Pin D8 -- LCD_RS
//  E -- Pin D9 -- LCD_ENABLE
//  D4 -- Pin D46 -- LCD_DATA_0  Data for LCD 
//  D5 -- Pin D48 -- LCD_DATA_1  Data for LCD
//  D6 -- Pin D50 -- LCD_DATA_2  Data for LCD
//  D7 -- Pin D52 -- LCD_DATA_3  Data for LCD
//  A -- 3.3v -- N/A            Annode for LCD Backlight
//  K -- GND -- N/A             Cathode for LCD Backlight
//  RW -- GND -- N/A

//-------------------------------------------------------------------------------------------------------------------------------------------
//LEDs
#define RED_1 2
#define GRN_1 3

//Volume Pots
#define POT_1_2 A4
#define POT_2_2 A5
#define POT_3_2 A6
#define POT_4_2 A7

//Mute Button
#define BTN_OUT A14

//LCD Screen
#define LCD_RS 8
#define LCD_ENABLE 9
#define LCD_DATA_4 46
#define LCD_DATA_5 48
#define LCD_DATA_6 50
#define LCD_DATA_7 52
#define LCD_CONTRAST 11