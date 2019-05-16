import os, re
from ctypes import windll
import zipfile
DestribFolder = os.path.realpath('destrib')
ScriptPath = os.path.realpath(__file__)
ProjectPath = os.path.abspath(os.path.join(os.path.dirname( __file__ ), '..'))
QtToolSet = ''

print('ScriptPath    =', ScriptPath)
print('DestribFolder =', DestribFolder)
print('ProjectPath   =', ProjectPath)

def check_dir(path):
    result = True
    try:
        if not os.path.isdir(path):
            print('\'',path , '\' not exists. Create: ', end='', sep = '')
            os.mkdir(path)
            print('Ok')
        else:
            print('\'',path , '\' exists', sep = '')
    except FileExistsError:
        print('Fail')
        result = False
    return result

def get_drives():
    drives = []
    bitmask = windll.kernel32.GetLogicalDrives()
    letter = ord('A')
    while bitmask > 0:
        if bitmask & 1:
            drives.append(chr(letter) + ':\\')
        bitmask >>= 1
        letter += 1
    return drives

def get_qt_bin_folders(path):
    bin_folders = []
    tmp_folders = os.listdir(path)
    for folder in tmp_folders:
        complect_path = os.path.join(path, folder)
        complect_bin_path = os.path.join(complect_path, 'bin\\qmake.exe')
        if os.path.isfile(complect_bin_path):
            bin_folders.append(complect_path)
    return bin_folders

def scan_qt_folder(path):
    qt_folders = []
    tmp_folders = os.listdir(path)
    for folder in tmp_folders:
        if re.match(r'(\d{1,})(\.(\d+))*', folder):
            tmp_path = os.path.join(path, folder)
            tmp = get_qt_bin_folders(tmp_path)
            qt_folders.extend(tmp)

    return qt_folders

def find_qt_bin():
    qt_bin = []
    drives = get_drives()
    for drive in drives:
        tmp_folders = []
        try:
            tmp_folders = os.listdir(drive)
            for folder in tmp_folders:
                if folder.startswith('Qt'):
                    qt_bin.extend(scan_qt_folder(drive + folder))
                    '''qmake_files = [f for f in glob.glob(drive + folder + '\\**/*qmake.exe', recursive = True)]
                    for f in qmake_files:
                        qt_bin.append(f)'''
        except:
            pass
    return qt_bin

def select_qt_complect():
    qt = ''
    qt_dirs = find_qt_bin()

    def read_qt_version(path):
        version = ''
        mkspecs = os.path.join(path, 'mkspecs\\qconfig.pri')
        try:
            f = open(mkspecs, 'r', encoding='utf8')
            lines = f.readlines()
            for line in lines:
                if line.startswith('QT_VERSION'):
                    values = line.split('=')
                    version = values[1].strip()
            f.close()
        except:
            pass
        return version

    if len(qt_dirs) != 0:
        num = 1
        print("Select version to copy files:")
        for qtdir in qt_dirs:
            build_complect = os.path.basename(os.path.normpath(qtdir))
            print(num, ') ', build_complect, ' ', read_qt_version(qtdir), '(', build_complect, ')', sep='')
            num += 1
        select_id = int(input('> ')) - 1
        qt = qt_dirs[select_id]
    else:
        print("Enter Qt folder to copy files:")
        qt = input('> ')

    return qt

print()
QtToolSet = select_qt_complect()
