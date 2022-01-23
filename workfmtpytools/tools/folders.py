import os#, ntpath
from ctypes import windll

def getDrives():
    drives = []
    bitmask = windll.kernel32.GetLogicalDrives()
    letter = ord('A')
    while bitmask > 0:
        if bitmask & 1:
            drives.append(chr(letter) + ':\\')
        bitmask >>= 1
        letter += 1
    return drives

def getParent(path, levels = 1):
    common = path

    for i in range(levels + 1):
        common = os.path.dirname(common)
        
    return os.path.relpath(path, common)