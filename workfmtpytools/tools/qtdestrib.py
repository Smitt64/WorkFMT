import os, ntpath, re
from ctypes import windll
from tools.folders import *

class QtInstallerElement:
    def __init__(self, path : str):
        self.__path = str(path)
        self.__version = os.path.basename(path)

    def getVersion(self):
        return self.__version

    def getPath(self):
        return self.__path

class QtDestribElement:
    def __init__(self, path : str):
        self.__path = str(path)
        self.__version, self.__release_date = self.__read_qt_version()
        self.__complect = self.__read_complect()

    def __read_complect(self):
        return str(os.path.basename(os.path.normpath(self.__path)))

    def __read_qt_version(self):
        mkspecs = os.path.join(self.__path, 'mkspecs\\qconfig.pri')
        version = ''
        release_date = ''

        try:
            f = open(mkspecs, 'r', encoding='utf8')
            lines = f.readlines()
            for line in lines:
                if line.startswith('QT_VERSION'):
                    values = line.split('=')
                    version = values[1].strip()

                if line.startswith('QT_RELEASE_DATE'):
                    values = line.split('=')
                    release_date = values[1].strip()
            f.close()
        except:
            raise Exception('Can\'t read file ' + mkspecs)
        return str(version), str(release_date)

    def getVersion(self):
        return self.__version

    def getReleaseDate(self):
        return self.__release_date

    def getPath(self):
        return self.__path

    def getComplect(self):
        return self.__complect

class QtDestribList:
    def __init__(self):
        self.__qt_bin = []
        self.__qt_installer_bin = []
        drives = getDrives()

        for drive in drives:
            tmp_folders = []
            try:
                tmp_folders = os.listdir(drive)
                for folder in tmp_folders:
                    if folder.startswith('Qt'):
                        qt_folders_arr, qt_installer_folders_arr = self.__scan_qt_folder(drive + folder)
                        self.__qt_bin.extend(qt_folders_arr)
                        self.__qt_installer_bin.extend(qt_installer_folders_arr)
            except:
                pass

    def size(self):
        return len(self.__qt_bin)
    
    def installerCount(self):
        return len(self.__qt_installer_bin)

    def getElement(self, index : int) -> QtDestribElement:
        return self.__qt_bin[index]

    def getInstallerElement(self, index : int) -> QtInstallerElement:
        return self.__qt_installer_bin[index]

    def getQtList(self):
        return self.__qt_bin

    def getQtInstallerList(self):
        return self.__qt_installer_bin

    def __get_qt_bin_folders(self, path : str):
        bin_folders = []
        tmp_folders = os.listdir(path)
        for folder in tmp_folders:
            complect_path = os.path.join(path, folder)
            complect_bin_path = os.path.join(complect_path, 'bin\\qmake.exe')
            if os.path.isfile(complect_bin_path):
                bin_folders.append(QtDestribElement(complect_path))
        return bin_folders

    def __get_qt_installer_bin_folders(self, path : str):
        bin_folders = []
        complect_bin_path = os.path.join(path, 'bin\\binarycreator.exe')
        if os.path.isfile(complect_bin_path):
            bin_folders.append(QtInstallerElement(path))

        return bin_folders

    def __scan_qt_installer_folder(self, path : str):
        qt_installer_folders = []
        tmp_folders = os.listdir(path)

        for folder in tmp_folders:
            if re.match(r'(\d{1,})(\.(\d+))*', folder):
                tmp_path = os.path.join(path, folder)
                tmp = self.__get_qt_installer_bin_folders(tmp_path)
                qt_installer_folders.extend(tmp)

        return qt_installer_folders

    def __scan_qt_folder(self, path : str):
        qt_folders = []
        qt_installer_folders = []
        tmp_folders = os.listdir(path)

        for folder in tmp_folders:
            if re.match(r'(\d{1,})(\.(\d+))*', folder):
                tmp_path = os.path.join(path, folder)
                tmp = self.__get_qt_bin_folders(tmp_path)
                qt_folders.extend(tmp)
            elif folder == 'Tools':
                tools_path = os.path.join(path, folder)
                installer_framework_path = os.path.join(tools_path, 'QtInstallerFramework')
                tmp = self.__scan_qt_installer_folder(installer_framework_path)
                qt_installer_folders.extend(tmp)

        return qt_folders, qt_installer_folders