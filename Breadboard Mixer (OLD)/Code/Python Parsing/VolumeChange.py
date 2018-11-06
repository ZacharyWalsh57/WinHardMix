#This file uses pycaw to change the volume of a desiered PID.  We test it first just muting all but chrome.

from pycaw.pycaw import AudioUtilities

sessions = AudioUtilities.GetAllSessions()
print(sessions)
for session in sessions:
    volume = session.SimpleAudioVolume
    if session.Process and session.Process.name() == "chrome.exe":
        volume.SetMute(0, None)
    else:
        volume.SetMute(1, None)
