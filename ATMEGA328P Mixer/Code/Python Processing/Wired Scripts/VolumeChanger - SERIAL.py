import serial #Talks to the serial port on the ATMEGA Chip
import serial.tools.list_ports #Used to real all possible COM Ports
import time #Used to delay reading from serial to avoid overflow errors.
import re #Used for searching the list of all current processes
import psutil #Used to gather all running tasks and their names
import string #Used to convert the intergers and strings required for program selection
from pycaw.pycaw import AudioUtilities, ISimpleAudioVolume #Used for setting the actual volumes.
import os #Use this to clear out the terminal output for the user when debug is set to off.

#TRIGGER TO SHOW SERIAL OUTPUT OR NOT!!
SERIAL_DEBUG = True
BAUD_RATE = 115200 #Added this in.  I'm suddenly having issue keeping the audio from cutting... no idea why

#Welcome Message
os.system('cls' if os.name == 'nt' else 'clear') #Clear Screen.
print("\n \t\t----- HardMix10 ----- ")
print("\tThe Mixer Everyone Wanted, But I Built")
print("\t\tVERSION 1.0 - ATMEGA328\n")

#Get a list of all possible serial ports the user can pick from.  This way anyone can use this thing.
COMLIST = serial.tools.list_ports.comports()
ACTIVE_PORTS = []
for PORT in COMLIST: ACTIVE_PORTS.append(PORT.device)

#Ask the user which port they want to use now if there is more than one port.
#Instead of consistent loops, just use a recusrive function. This gets a port and makes sure it actually exists.  
def PORT_SELECTION():
    VALID_PORT = False #Trigger to keep looping or not.
    while VALID_PORT == False:
        print("Pick which serial port your mixer is connected to:")
        for ELEM in ACTIVE_PORTS:
            print(ELEM)
        PORT_SELECTED = input("\nEnter your port number: ")
        for ELEM in ACTIVE_PORTS:
            if PORT_SELECTED in ELEM:
                VALID_PORT = True
                return ELEM
 
if ACTIVE_PORTS.__len__() == 1: 
    PORT = ACTIVE_PORTS[0]
    print("Only one port found.  Using port " + ACTIVE_PORTS[0])
else: 
    PORT = PORT_SELECTION()
    print("Using port " + PORT + "\n")

#Now we try to open the port the user picked.  If the port isn't ready, just raise an exception and try again.
ATMEGA_SERIAL = None
def PORT_CONNECITON(PORT_NUMBER):
    PORT_CONNECTED = False
    if SERIAL_DEBUG == True:
        ATTEMPTS = 1
        print("OPENING PORT....")
        while ATTEMPTS <= 3 and PORT_CONNECTED == False:
            try: 
                print("ATTEMPT " + str(ATTEMPTS) + ".....")
                ATMEGA_SERIAL = serial.Serial(PORT_NUMBER, BAUD_RATE, timeout=0)
                ATTEMPTS = 3
                print("PORT OPENED!\n")
                PORT_CONNECTED = True
                ATMEGA_SERIAL.close()
                return PORT_CONNECTED

            except:
                ATTEMPTS = ATTEMPTS + 1
                print("PORT WONT OPPEN. DELAYING AND TRYING AGAIN....\n")
                time.sleep(2)
        if ATTEMPTS == 3 and PORT_CONNECTED == False:
            print("CANT OPEN THE PORT.  NO CLUE WHY.  ERHMEHGAWD.\n")
            exit()
        
    if SERIAL_DEBUG == False:
        ATTEMPTS = 1
        while ATTEMPTS <= 3 and PORT_CONNECTED == False:
            try: 
                ATMEGA_SERIAL = serial.Serial(PORT_NUMBER, BAUD_RATE, timeout=0)
                ATTEMPTS = 3
                PORT_CONNECTED = True
                ATMEGA_SERIAL.close()
                return PORT_CONNECTED

            except:
                ATTEMPTS = ATTEMPTS + 1
                time.sleep(2)
        if ATTEMPTS == 3 and PORT_CONNECTED == False:
            exit()

if PORT_CONNECITON(PORT): print("Mixer Connected to " + str(PORT) + " Ready to Work!")
else: print("The port is not opening. Restart the script and make sure it is not in use.") | exit()

#Once the user has the port picked and it opens like it should, we need programs to control. 
#These two lists are globally accessable for all functions to fux around with.  
PROCESS_LIST = [] #Task Names
PPID_LIST = [] #PIDs / PPIDs to lock for volume changes.

#Task Picking function.  Does the following things:
# 1: Get how many tasks we want to control.
# 2: Get the names of said tasks. 
#   2a: Make sure the tasks actually exist.
# 3: If it exists add it.  If not try again, bitch.
# 4: Print the task names along with the PPIDs for the user.  
# 5: Get ready to run the actual volume mixing side of things. 
def TASK_SELECTION():
    #Number of tasks to watch:
    try:
        PROCESSES_REMAINING_STR = input("How many tasks are we controlling? (1-4): ")
        while int(PROCESSES_REMAINING_STR) > 4:
            print("Pick a number from 1 to 4.")
            PROCESSES_REMAINING_STR = input("Try that again.  I said 1-4: ")
        PROCESSES_REMAINING = int(PROCESSES_REMAINING_STR)
    except:
        pass

    #Gathering names of the tasks. 
    while PROCESSES_REMAINING != 0:
        print("\nJust a heads up, for a mic monitor, use the task SVCHOST.")
        PROCESS_NAME = input("Enter a program name:    ")
        for ACTIVE_PROCESS in psutil.process_iter(): #checks for a match.
            try:
                SET_NAME = ACTIVE_PROCESS.name() #Get an actual name
                SET_PPID = ACTIVE_PROCESS.pid #Get an actual process PID
            except psutil.NoSuchProcess:
                print("PROCESS DOES NOT EXIST")
                continue

            #Actual Searching process here:
            if re.search(PROCESS_NAME, SET_NAME, re.IGNORECASE):
                if SET_NAME not in PROCESS_LIST:
                    PROCESS_LIST.append(SET_NAME)
                    PPID_LIST.append(SET_PPID)
            
        #Confirm or decline the request to add to the lists. 
        if PROCESS_NAME.lower() in str(PROCESS_LIST).lower():
            (PROCESSES_REMAINING) = (PROCESSES_REMAINING) - 1
            print("ADDED TO THE LIST! REMAINING: ", PROCESSES_REMAINING, "\n")
        else:
            print("COULD NOT ADD. TRY AGAIN. ", PROCESSES_REMAINING, " PROCESSES LEFT. \n")

    #Print new lists for user to check everything over.  
    for PROCESS_PPID_PAIRS in range(len(PROCESS_LIST)):
        print("Process:  ", PROCESS_LIST[PROCESS_PPID_PAIRS], " -- ", PPID_LIST[PROCESS_PPID_PAIRS])
        
TASK_SELECTION()

#Now for the just super duper fun part.  Volume control.  Really (I MEAN REALLY) RIVETING stuff in here.
#Allocate all potential endpoints for every program. 
VOLUME_ENDPOINTS = AudioUtilities.GetAllSessions()
SERIAL_COMS = serial.Serial(PORT, BAUD_RATE, timeout=0)

#This function does the following things:
# 1: It reads the serial inputs (Finally lol. When we go GUI it's not this bad.)
#   1a: Take the serial input and delim it based on commas and '' since that's how the ATMEGA sends data.
#   1b: Assign each one of the processes a pot and hook a process to it.
#   1c: Make sure we got 4 values into the python script.  Last thing we wanna do it blow someone's ears out because it's reading wrong.
# 2: Find the process PID and current volume.
# 3: Take all that good shit and set the volume to whatever the pot says it should be.  Rince and repeat. 
def ENDPOINT_ADJUSTMENT(SERIAL_INPUT):
    try:
        #Open the Serial Port and delim it.
        SERIAL_IN = str((SERIAL_INPUT.readline()).rstrip(), "utf-8")
        POT_VOLUME_CURRENT = SERIAL_IN.split()
        
        #If this list just exists. (Set the bar low here lol)
        if POT_VOLUME_CURRENT:
            if len(POT_VOLUME_CURRENT) == 4:
                if SERIAL_DEBUG:
                    print(POT_VOLUME_CURRENT)

                #The fun part.... For each process stored in the process list.....
                for PROCESS in PROCESS_LIST:    
                    #Compare the process name to the potenaial endpoints.
                    for ENDPOINT in VOLUME_ENDPOINTS:
                        PROCESS_VOLUME = ENDPOINT.SimpleAudioVolume
                        
                        #If the process has an endpoint and the name matches:
                        if ENDPOINT.Process and ENDPOINT.Process.name() == PROCESS:
                            INDEX = PROCESS_LIST.index(PROCESS)
                            VOLUME_SET = float(POT_VOLUME_CURRENT[INDEX]) / 100
                            if (VOLUME_SET * 100) < 1.0:
                                VOLUME_SET = 0.0
                            VOLUME_CURRENT = float(PROCESS_VOLUME.GetMasterVolume())
                            #Get the properties of this volume and hook it.
                            #Adjust as needed down there.
                            
                            #BUG: BAUD RATE NEEDS TO BE SET AT AT LEAST 115200 FOR THIS TO WORK OVER WIRE???? WTF WHY?
                            #LATENCY MIGHT FUCK ME NOW. YAY.
                            #These three Ifs stop the volume from flickering around like mad up/down.  
                            #This is due to baud rate latency, I think? The issue is worse at lower baud rates.
                            #That kinda sucks because I was hoping to run the ESP at 9600 not 115200.                   
                            if VOLUME_CURRENT != 0 and (VOLUME_SET) < VOLUME_CURRENT / 2.0:
                                PROCESS_VOLUME.SetMasterVolume(VOLUME_CURRENT, None)
                            
                            if (VOLUME_SET + .075) > VOLUME_CURRENT: 
                                PROCESS_VOLUME.SetMasterVolume(VOLUME_SET, None)
                            
                            if (VOLUME_SET - .75) < VOLUME_CURRENT:
                                PROCESS_VOLUME.SetMasterVolume(VOLUME_SET, None)

                            

    except:
        if SERIAL_DEBUG:
            print("Whoops pulled a tad too quickly.")
        else:
            print("")

#Run that function and let it fly.  
time.sleep(1)
os.system('cls' if os.name == 'nt' else 'clear')

#FIXME: GET RID OF THIS SHIT. IT HURTS ME A BIT.
#This delay blows, but it makes 100% sure that the bootloader has finished up and is passing serial values. 
print("Waiting a second to let everything settle down...")
time.sleep(5)

print("Running Now.....")
while True:
    ENDPOINT_ADJUSTMENT(SERIAL_COMS)
    
#BOOM BITCH IT WORKS.
#Not sure what else this needs aside from getting the wirleess module connected and programmed but that goes in a new file.
