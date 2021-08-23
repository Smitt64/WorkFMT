import os, sys, configparser
import shutil

def versiontuple(v):
    return tuple(map(int, (v.split("."))))

def recursive_overwrite(src, dest, ignore=None):
    if os.path.isdir(src):
        if not os.path.isdir(dest):
            os.makedirs(dest)
        files = os.listdir(src)
        if ignore is not None:
            ignored = ignore(src, files)
        else:
            ignored = set()
        for f in files:
            if f not in ignored:
                recursive_overwrite(os.path.join(src, f),
                                    os.path.join(dest, f),
                                    ignore)
    else:
        shutil.copyfile(src, dest)

class Config:
    def __init__(self):
        self.ToolsComplectRepo = '\\\\ATOM\\ToolsComplect'
        self.WorkFmtDir = 'D:\\Work\\WorkFMT'
        self.QRsdToolsDir = os.path.join(self.WorkFmtDir, 'qrsd\\tools')
        self.QRsdRsdDir = os.path.join(self.WorkFmtDir, 'qrsd\\rsd')

        self.Version = None
        self.ComplectDir = ''
        self.RsdHeadersPath = None
        self.RsdLibPath = None
        self.ToolsHeadersPath = None
        self.ToolLibPath = None
        self.ToolBinPath = None
        self.RsdBinPath = None
        self.VersionFile = None
        self.VersionConfig = None

    def setComplectDir(self,element):
        self.ComplectDir = os.path.join(self.ToolsComplectRepo, element)

    def setVersion(self, ver):
        self.Version = ver
        self.ComplectDir = os.path.join(self.ComplectDir, ver)

    def setVsVersion(self, ver):
        self.ComplectDir = os.path.join(self.ComplectDir, ver)

    def init(self):
        self.RsdHeadersPath = os.path.join(self.ComplectDir, 'Build\RSD\\h')
        self.ToolsHeadersPath = os.path.join(self.ComplectDir, 'Build\\tools\\h')

        self.RsdLibPath = os.path.join(self.ComplectDir, 'x32\\RSD\\Release')
        self.RsdLibPathDebug = os.path.join(self.ComplectDir, 'x32\\RSD\\Debug')

        self.ToolLibPath = os.path.join(self.ComplectDir, 'x32\\Lib\\Release')
        self.ToolLibPathDebug = os.path.join(self.ComplectDir, 'x32\\Lib\\Debug')

        self.ToolBinPath = os.path.join(self.ComplectDir, 'x32\\Obj\\Release')
        self.ToolBinPathDebug = os.path.join(self.ComplectDir, 'x32\\Obj\\Debug')

        self.RsdBinPath = self.RsdLibPath
        self.RsdBinPathdebug = self.RsdLibPathDebug
        self.VersionFile = os.path.join(self.ComplectDir, 'About\\version.txt')

        self.VersionConfig = configparser.ConfigParser()
        self.VersionConfig.read(self.VersionFile)

    def printPaths(self):
        print('Tools headers path: '.ljust(20), self.ToolsHeadersPath)
        print('Tools lib path: '.ljust(20), self.ToolLibPath)
        print('Tools bin path: '.ljust(20), self.ToolBinPath)

        print()
        print('Rsd headers path: '.ljust(20), self.RsdHeadersPath)
        print('Tools lib path: '.ljust(20), self.ToolLibPath)
        print('RsdRsd bin path: '.ljust(20), self.RsdBinPath)

        print()
        print('About file path: '.ljust(20), self.VersionFile)

    def printVersions(self):
        print()
        print('version.txt:')
        print('BuilDate: '.ljust(10), self.VersionConfig['TC']['BuilDate'])
        print('Tools: '.ljust(10), self.VersionConfig['TOOLS']['Version'])
        print('RSD: '.ljust(10), self.VersionConfig['RSD']['Version'])

        '''print(versiontuple(self.VersionConfig['RSD']['Version']))
        print(versiontuple(self.VersionConfig['TOOLS']['Version']))'''

class WorkFmtUpdate:
    def __init__(self, config: Config):
        super().__init__()
        self.__config = config
        self.__RsdHeaderDir = os.path.join(self.__config.QRsdRsdDir, 'include')
        self.__RsdLibDir = os.path.join(self.__config.QRsdRsdDir, 'lib')

        self.__ToolsHeaderDir = os.path.join(self.__config.QRsdToolsDir, 'h')
        self.__ToolsLibDir = os.path.join(self.__config.QRsdToolsDir, 'lib')

        self.__RsdLibToCopy = ['rsdc.lib']
        self.__RsdDLLToCopy = ['rddrvo.dll', 'rddrvou.dll',
                               'rdrset.dll', 'RsBtr.dll', 'rsdc.dll']

        self.__ToolsLibToCopy = ['rsrtlwm.lib']
        self.__ToolsdDllToCopy = ['rsrtlwm.dll', 'RSScript.dll']

    def __copyFileList(self, src: str, dst: str, files: list):
        for file in files:
            srcfile = os.path.join(src, file)
            dstfile = os.path.join(dst, file)
            shutil.copyfile(srcfile, dstfile)

    def copyRsd(self) :
        print(self.__RsdHeaderDir)
        shutil.rmtree(self.__RsdHeaderDir)
        recursive_overwrite(self.__config.RsdHeadersPath, self.__RsdHeaderDir)

        debugdir = os.path.join(self.__RsdLibDir, 'debug')
        releasedir = os.path.join(self.__RsdLibDir, 'release')

        try:
            shutil.rmtree(debugdir)
        except BaseException:
            pass

        try:
            shutil.rmtree(releasedir)
        except BaseException:
            pass

        os.mkdir(debugdir)
        os.mkdir(releasedir)

        self.__copyFileList(self.__config.RsdLibPath, releasedir, self.__RsdLibToCopy)
        self.__copyFileList(self.__config.RsdLibPathDebug, debugdir, self.__RsdLibToCopy)

        self.__copyFileList(self.__config.RsdBinPath, releasedir, self.__RsdDLLToCopy)
        self.__copyFileList(self.__config.RsdBinPathdebug, debugdir, self.__RsdDLLToCopy)

    def copyTools(self):
        print(self.__ToolsHeaderDir)
        shutil.rmtree(self.__ToolsHeaderDir)
        recursive_overwrite(self.__config.ToolsHeadersPath, self.__ToolsHeaderDir)

        debugdir = os.path.join(self.__ToolsLibDir, 'debug')
        releasedir = os.path.join(self.__ToolsLibDir, 'release')

        try:
            shutil.rmtree(debugdir)
        except BaseException:
            pass

        try:
            shutil.rmtree(releasedir)
        except BaseException:
            pass

        os.mkdir(debugdir)
        os.mkdir(releasedir)

        self.__copyFileList(self.__config.ToolLibPath, releasedir, self.__ToolsLibToCopy)
        self.__copyFileList(self.__config.ToolLibPathDebug, debugdir, self.__ToolsLibToCopy)

        self.__copyFileList(self.__config.ToolBinPath, releasedir, self.__ToolsdDllToCopy)
        self.__copyFileList(self.__config.ToolBinPathDebug, debugdir, self.__ToolsdDllToCopy)

def selectSubDir(parent, msg, errmsg):
    subdirs = os.listdir(parent)
    selecteddir = None
    print(msg)

    for index, item in enumerate(subdirs, start=1):
        print('%2d' % index + ': ' + item)

    print(' > ', end='')

    try:
        selected = int(input())

        if selected <= 0 or selected > len(subdirs):
            raise ValueError()

        selecteddir = subdirs[selected - 1]

    except ValueError:
        print(errmsg)
        sys.exit()

    print()
    return selecteddir

config = Config()

ToolsComplectVersion = selectSubDir(config.ToolsComplectRepo, 'Select complect: ', 'Wrong complect selected')
config.setComplectDir(ToolsComplectVersion)

Version = selectSubDir(config.ComplectDir, 'Select version: ', 'Wrong version selected')
config.setVersion(Version)

VsVersion = selectSubDir(config.ComplectDir, 'Select VS version: ', 'Wrong VS version selected')
config.setVsVersion(VsVersion)

config.init()
config.printPaths()
config.printVersions()


fmtupd = WorkFmtUpdate(config)
fmtupd.copyRsd()
fmtupd.copyTools()
