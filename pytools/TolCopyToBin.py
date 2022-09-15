import sys, os
from shutil import copyfile

class CopyHelper:
    def __init__(self):
        self.BuildType = 'debug'
        self.WorkFmtDir = 'D:\\Work\\WorkFMT'
        self.WorkFmtDirBin = os.path.join(self.WorkFmtDir, 'bin')

        self.__ToolsToCopy = [
            'rsrtlwm.dll',
            'RSScript.dll'
        ]

        self.__RsdToCopy = [
            'rddrvo.dll',
            'rddrvou.dll',
            'rdrset.dll',
            'RsBtr.dll',
            'rsdc.dll',
            'dbinit.exe',
            'fmtxml.exe'
        ]

    def __CopyArray(self, arr, src_path):
        for file in arr:
            __srcpath = os.path.join(self.WorkFmtDir, src_path)
            __srcpath = os.path.join(__srcpath, file)
            __dstpath = os.path.join(self.WorkFmtDirBin, file)
            copyfile(__srcpath, __dstpath)

    def copy(self):
        self.__CopyArray(self.__ToolsToCopy, 'qrsd\\tools\\lib\\{}'.format(self.BuildType))
        self.__CopyArray(self.__RsdToCopy, 'qrsd\\rsd\\lib\\{}'.format(self.BuildType))

helper = CopyHelper()
print('Copy binares: \n1: debug\n2: release \n3: exit\n > ', end='')

try:
    result = int(input())
    if result == 3:
        sys.exit()

    if result == 2:
        helper.BuildType = 'release'

except ValueError:
    print('Wrong answer')
    sys.exit()

helper.copy()
