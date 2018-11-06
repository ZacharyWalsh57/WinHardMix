import serial
import time
import re
import psutil
import string
from pycaw.pycaw import AudioUtilities, ISimpleAudioVolume

print("\n---- WinHardMix V1.0 ----\n")

try:
    ARDUINO_SERIAL = serial.Serial('COM5', 115200, timeout=0)
    print("PORT FOUND")
except:
    print("SERIAL PORT NOT FOUND")
    exit()

#Lists for storing PIDs and Process Names.
PROCESS_LIST = []
PPID_LIST = []

print("STARTING......\n")
time.sleep(10)

#Check the user process name against the ones currently running.
PROCESSES_REMAINING_STR = input("Enter how many processes you want to monitor? (1-4): ")
while int(PROCESSES_REMAINING_STR) >= 4:
    print("Pick a number from 1 to 4.")
    PROCESSES_REMAINING_STR = input("Enter how many processes you want to monitor? (1-4): ")
PROCESSES_REMAINING = int(PROCESSES_REMAINING_STR)

while PROCESSES_REMAINING != 0:
    PROCESS_NAME = input("Enter a program name:    ")
    for ACTIVE_PROCESS in psutil.process_iter(): #checks for a match.
        try:
            SET_NAME = ACTIVE_PROCESS.name() #Get an actual name
            SET_PPID = ACTIVE_PROCESS.pid #Get an actual process PID
        except psutil.NoSuchProcess:
            print("PROCESS DOES NOT EXIST")
            continue

        #Search for a match
        if re.search(PROCESS_NAME, SET_NAME, re.IGNORECASE):
            if SET_NAME not in PROCESS_LIST:
                PROCESS_LIST.append(SET_NAME)
                PPID_LIST.append(SET_PPID)

    #confirm or decline the request to add to the lists. 
    if PROCESS_NAME.lower() in str(PROCESS_LIST).lower():
        (PROCESSES_REMAINING) = (PROCESSES_REMAINING) - 1
        print("ADDED TO THE LIST! REMAINING: ", PROCESSES_REMAINING, "\n")
    else:
        print("COULD NOT ADD. TRY AGAIN. ", PROCESSES_REMAINING, " PROCESSES LEFT. \n")

#Print new lists for user to check everything over.  
for PROCESS_PPID_PAIRS in range(len(PROCESS_LIST)):
    print("Process:  ", PROCESS_LIST[PROCESS_PPID_PAIRS], " -- ", PPID_LIST[PROCESS_PPID_PAIRS])


VOLUME_ENDPOINTS = AudioUtilities.GetAllSessions()
while True:
    try:
        SERIAL_IN = str((ARDUINO_SERIAL.readline()).rstrip(), "utf-8")
        POT_VOLUME_CURRENT = SERIAL_IN.split()

        if POT_VOLUME_CURRENT:
            if len(POT_VOLUME_CURRENT) == 4:
                print(POT_VOLUME_CURRENT)
                
                for PROCESS in PROCESS_LIST:    
                    for ENDPOINT in VOLUME_ENDPOINTS:
                        PROCESS_VOLUME = ENDPOINT.SimpleAudioVolume
                        
                        if ENDPOINT.Process and ENDPOINT.Process.name() == PROCESS:
                            INDEX = PROCESS_LIST.index(PROCESS)
                            VOLUME_SET = float(POT_VOLUME_CURRENT[INDEX]) / 100
                            VOLUME_CURRENT = float(PROCESS_VOLUME.GetMasterVolume())
                            
                            if VOLUME_CURRENT != 0 and (VOLUME_SET) < VOLUME_CURRENT / 2.0:
                                PROCESS_VOLUME.SetMasterVolume(VOLUME_CURRENT, None)
                            
                            #if VOLUME_SET != 0:
                                #PROCESS_VOLUME.SetMute(0, None)
                            
                            elif (VOLUME_SET + .075) > VOLUME_CURRENT: 
                                #print("PROCESS: ", PROCESS, " | SET: ", VOLUME_SET, " | INDEX: ", INDEX)     
                                PROCESS_VOLUME.SetMasterVolume(VOLUME_SET, None)
                            
                            elif (VOLUME_SET - .75) < VOLUME_CURRENT:
                                #print("PROCESS: ", PROCESS, " | SET: ", VOLUME_SET, " | INDEX: ", INDEX)     
                                PROCESS_VOLUME.SetMasterVolume(VOLUME_SET, None)

                            #elif VOLUME_SET == 0:
                                #PROCESS_VOLUME.SetMute(1, None)
                        
    except:
        print("NAH FAM LOL")
        continue
        
    