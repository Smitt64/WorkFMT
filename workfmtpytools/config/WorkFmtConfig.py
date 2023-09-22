import os

class WorkFmtConfig:
    __instance = None

    def __init__(self):
        self.__qtComplect = None
        self.__qtInstallerFramework = None
        self.__installerPath = 'd:\\Work\\WorkFMTInstaller'
        self.__workfmtsourcedir = 'D:\\Work\\WorkFMT'
        self.__msvcvars = 'c:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\VC\\Auxiliary\\Build\\vcvars32.bat'
        #os.path.dirname(__file__)

    @staticmethod
    def inst():
        if WorkFmtConfig.__instance is None:
            WorkFmtConfig.__instance = WorkFmtConfig()

        return WorkFmtConfig.__instance

    def getMsvcBat(self):
        return self.__msvcvars

    def setQtComplect(self, complect):
        self.__qtComplect = complect
    
    def getQtComplect(self):
        return self.__qtComplect

    def setQtInstallerFramework(self, complect):
        self.__qtInstallerFramework = complect

    def getInstallerPath(self):
        return self.__installerPath

    def getQtInstallerFramework(self):
        return self.__qtInstallerFramework

    def getWorkFmtSourceDir(self):
        return self.__workfmtsourcedir

    def getBinaryType(self):
        return 'release'#'debug'
