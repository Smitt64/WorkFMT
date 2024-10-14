import sys, os
from shutil import copyfile

class CopyHelper:
    def __init__(self):
        self.BuildType = 'debug'
        self.WorkFmtDir = 'D:\\Work\\WorkFMT'
        self.WorkFmtDirBin = os.path.join(self.WorkFmtDir, 'bin')

        self.__ToolsToCopy = ['rsrtlwm.dll', 'RSScript.dll', 'rslocale.dll', 'fs32cm.dll']
        self.__OtherToCopy = ['xerces-c_3_2.dll']

        self.__RsdToCopy = [
            'boost_chrono-vc100-mt-x32-1_72.dll',
            'boost_date_time-vc100-mt-x32-1_72.dll',
            'boost_filesystem-vc100-mt-x32-1_72.dll',
            'boost_filesystem-vc140-mt-x32-1_72.dll',
            'boost_iostreams-vc140-mt-x32-1_72.dll',
            'boost_locale-vc100-mt-x32-1_72.dll',
            'boost_locale-vc140-mt-x32-1_72.dll',
            'boost_regex-vc100-mt-x32-1_72.dll',
            'boost_regex-vc140-mt-x32-1_72.dll',
            'boost_serialization-vc100-mt-x32-1_72.dll',
            'boost_thread-vc100-mt-x32-1_72.dll',
            'boost_thread-vc140-mt-x32-1_72.dll',
            'dbinit.exe',
            'fmtxml.exe',
            'fmtxml.xsd',
            'icudt57.dll',
            'icudt58.dll',
            'icuin57.dll',
            'icuin58.dll',
            'icuuc57.dll',
            'icuuc58.dll',
            'QTSysLog.dll',
            'rddrvou.dll',
            'rdrset.dll',
            'RsBtr.dll',
            'rsdc.dll',
            'rsdutl.dll',
            'SqlConverter.dll',
            'SqlConverterDebugConfig.json'
        ]

    def __CopyArray(self, arr, src_path):
        for file in arr:
            __srcpath = os.path.join(self.WorkFmtDir, src_path)
            __srcpath = os.path.join(__srcpath, file)
            __dstpath = os.path.join(self.WorkFmtDirBin, file)
            copyfile(__srcpath, __dstpath)
            print('File copied: ', file)

    def copy(self):
        self.__CopyArray(self.__ToolsToCopy, 'qrsd\\tools\\lib\\{}'.format(self.BuildType))
        self.__CopyArray(self.__RsdToCopy, 'qrsd\\rsd\\lib\\{}'.format(self.BuildType))
        self.__CopyArray(self.__OtherToCopy, 'qrsd\\installerfiles'.format(self.BuildType))

helper = CopyHelper()
helper.BuildType = 'release'
helper.copy()
