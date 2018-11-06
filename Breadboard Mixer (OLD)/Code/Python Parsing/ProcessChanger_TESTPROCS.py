from pycaw.pycaw import AudioUtilities
import re
import psutil

#Lists for storing PIDs and Process Names.
PROCESS_LIST = []
PPID_LIST = []

#Grab the processes we want from the user here.  This is done until we have PROCESSES_REMAINING
PROCESSES_REMAINING_STR = input("Enter how many processes you want to monitor? (1-4): ")
while int(PROCESSES_REMAINING_STR) >= 4:
    print("Pick a number from 1 to 4.")
    PROCESSES_REMAINING_STR = input("Enter how many processes you want to monitor? (1-4): ")
PROCESSES_REMAINING = int(PROCESSES_REMAINING_STR)

#Check the user process name against the ones currently running.
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
    print("Process:  ", PROCESS_LIST[PROCESS_PPID_PAIRS], " | PID:  ", PPID_LIST[PROCESS_PPID_PAIRS])

#Here we just set the first program given to mute itself for testing. 
CURRENT_AUDIO_ENDPOINTS = AudioUtilities.GetAllSessions()
for ENDPOINT in CURRENT_AUDIO_ENDPOINTS:
    VOLUME = ENDPOINT.SimpleAudioVolume
    if ENDPOINT.Process and (ENDPOINT.Process.name() in str(PROCESS_LIST).lower()):
        print("MUTING TASKS")
        VOLUME.SetMute(0, None) 
    else:
        VOLUME.SetMute(1, None)