import serial
import time

ARDUINO_SERIAL = serial.Serial('COM4', 115200, timeout=0)

time.sleep(10)
while True:
    try:
        print(ARDUINO_SERIAL.readline())
        time.sleep(.09)
    except:
        print("SERIAL ERROR")
