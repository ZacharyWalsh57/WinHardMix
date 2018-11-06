import psutil
import re

#User inputs name of process they want to try and use.
#checks that string against a list of processes running. 
#If theres a match return it. 

PROCESS_LIST = []
PPID_LIST = []

PROCESSES_REMAINING_STR = input("Enter how many processes you want to monitor? (1-4): ")
while int(PROCESSES_REMAINING_STR) >= 4:
    print("Pick a number from 1 to 4.")
    PROCESSES_REMAINING_STR = input("Enter how many processes you want to monitor? (1-4): ")
PROCESSES_REMAINING = int(PROCESSES_REMAINING_STR)

while PROCESSES_REMAINING != 0:
    PROCESS_NAME = input("Enter a program name:    ")
    for ACTIVE_PROCESS in psutil.process_iter():
        try:
            SET_NAME = ACTIVE_PROCESS.name()
            SET_PPID = ACTIVE_PROCESS.ppid()
        except psutil.NoSuchProcess:
            print("PROCESS DOES NOT EXIST")
            continue

        if re.search(PROCESS_NAME, SET_NAME, re.IGNORECASE):
            if SET_NAME not in PROCESS_LIST:
                PROCESS_LIST.append(SET_NAME)
                PPID_LIST.append(SET_PPID)

    if PROCESS_NAME in str(PROCESS_LIST).lower():
        (PROCESSES_REMAINING) = (PROCESSES_REMAINING) - 1
        print("ADDED TO THE LIST! REMAINING: ", PROCESSES_REMAINING, "\n")
    else:
        print("COULD NOT ADD. TRY AGAIN. ", PROCESSES_REMAINING, " PROCESSES LEFT. \n")
        
for PROCESS_PPID_PAIRS in range(len(PROCESS_LIST)):
    print("Process:  ", PROCESS_LIST[PROCESS_PPID_PAIRS], " | PID:  ", PPID_LIST[PROCESS_PPID_PAIRS])








