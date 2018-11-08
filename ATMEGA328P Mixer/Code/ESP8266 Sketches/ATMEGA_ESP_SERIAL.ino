String SERIAL_IN;

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    if (Serial.available())   {
        SERIAL_IN = Serial.readString(); // read the incoming data as string
        Serial.println(SERIAL_IN); // print data to serial esp8266 for test
    }
}