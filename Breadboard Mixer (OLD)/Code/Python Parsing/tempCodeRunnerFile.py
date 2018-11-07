try:
    ARDUINO_SERIAL = serial.Serial('COM5', 57600, timeout=0)
    print("PORT FOUND")
except:
    print("SERIAL PORT NOT FOUND")
    exit()