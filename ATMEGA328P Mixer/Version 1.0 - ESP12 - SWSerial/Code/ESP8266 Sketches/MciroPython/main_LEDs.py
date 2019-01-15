#Testing the functions of GPIOs on the ESP with uPython.
import time
import machine

LED_12 = machine.Pin(12, machine.Pin.OUT)
LED_13 = machine.Pin(13, machine.Pin.OUT)
LED_BUILTIN = machine.Pin(2, machine.Pin.OUT)

while True:
    LED_12.value(1)
    LED_13.value(0)
    LED_BUILTIN.value(1)
    time.sleep(1)
    LED_12.value(0)
    LED_13.value(1)
    LED_BUILTIN.value(0)
    time.sleep(1)
