import machine
from machine import UART
import time
import uos

SERIAL_IN = UART(0, 115200)
LED_BLUE = machine.Pin(12, machine.Pin.OUT)
LED_RED = machine.Pin(13, machine.Pin.OUT)

FILE_NAME_BYTE = uos.urandom(1)
FILE_NAME = int.from_bytes(FILE_NAME_BYTE, 0)

DATA_FILE = open('INPUT_SERIAL' + str(FILE_NAME) + '.txt', "rw")

LED_RED.value(1)
LED_BLUE.value(0)
time.sleep(10)
LED_RED.value(0)

COUNTER = 0
BUFFER = ""
while COUNTER < 16:
    BUFFER = SERIAL_IN.readLine()
    LED_BLUE.value(1)
    LED_BLUE.value(0)
    COUNTER = COUNTER + 1
    DATA_FILE.write(BUFFER)

print(BUFFER)
DATA_FILE.close()

LED_RED.value(1)
LED_BLUE.value(0)

#RX TO RX
#TX TO TX 
#HOLY BALLSACK IT READS CORRECTLY THIS IS NUTS.
#198, 219, 242