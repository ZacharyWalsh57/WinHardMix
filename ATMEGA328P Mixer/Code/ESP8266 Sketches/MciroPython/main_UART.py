import machine
from machine import UART
import time

SERIAL_IN = UART(0, 115200)
LED_BLUE = machine.Pin(12, machine.Pin.OUT)
LED_RED = machine.Pin(13, machine.Pin.OUT)

DATA_FILE = open('INPUT_SERIAL.txt', "wb")

LED_RED.value(1)
LED_BLUE.value(0)
time.sleep(1)
LED_RED.value(0)

COUNTER = 0
BUFFER = bytearray(4)
while COUNTER < 4:
    SERIAL_IN.readinto(BUFFER)
    LED_BLUE.value(1)
    LED_BLUE.value(0)
    DATA_FILE.write(BUFFER)
    COUNTER = COUNTER + 1

print(BUFFER)
DATA_FILE.close()

LED_RED.value(1)
LED_BLUE.value(0)



    