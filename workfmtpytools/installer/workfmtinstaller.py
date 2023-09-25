import os
import glob
import ntpath
import shutil
import configparser
from installer.installer import InstallCreator
from installer.instpackageinfo import *
from config.WorkFmtConfig import WorkFmtConfig
from win32api import GetFileVersionInfo, LOWORD, HIWORD
from shutil import copyfile
from datetime import date

def getExeVersion(path):
    info = GetFileVersionInfo (path, "\\")
    ms = info['FileVersionMS']
    ls = info['FileVersionLS']
    ver = str(HIWORD (ms)) + '.' + str(LOWORD (ms)) + '.' + str(HIWORD (ls)) + '.' + str(LOWORD (ls))

    return ver
#D:\Work\WfmtInstalldir
class WorkFmtMainPackage(InstallerPackageInfoBase):
    def __init__(self):
        self.__filesToCopy = ['WorkFMT/{}/WorkFMT.exe',
            'FmtLib/{}/FmtLib.dll',
            'FmtDbgHelp/{}/FmtDbgHelp.dll']

        super(WorkFmtMainPackage, self).__init__()
        
        today = date.today()
        self.DisplayName = 'Work Fmt'
        self.Description = 'WorkFMT – утилита, предоставляющая интерфейсный режим работы с FMT словарем непосредственно в схеме в БД. ' + \
            'Программное обеспечение позволяет управлять уже существующими данными в словаре, а так же создавать новые записи.'
        self.Name = 'com.rs.fmt.workfmt'
        self.ForcedInstallation = True
        self.ReleaseDate = today.strftime("%Y-%m-%d")

    def makeData(self, datadir):
        fmtdir = WorkFmtConfig.inst().getWorkFmtSourceDir()
        for cpfiletemplate in self.__filesToCopy:
            filetocopy = cpfiletemplate.format(WorkFmtConfig.inst().getBinaryType())
            srcexefile = os.path.join(fmtdir, filetocopy)
            dstexefile = os.path.join(self.DataPath, os.path.basename(filetocopy))
            copyfile(srcexefile, dstexefile)

    def getVersion(self):
        try:
            releasedir = os.path.join(WorkFmtConfig.inst().getWorkFmtSourceDir(), self.__filesToCopy[0].format(WorkFmtConfig.inst().getBinaryType()))
            ver = getExeVersion(releasedir)
            print(ver)
            return ver
        except:
            return "Unknown version"

class QtPackage(InstallerPackageInfoBase):
    def __init__(self):
        self.__qt_element = WorkFmtConfig.inst().getQtComplect()
        self.__QtBinDir = os.path.join(self.__qt_element.getPath(), 'bin')
        self.__QtPluginsDir = os.path.join(self.__qt_element.getPath(), 'plugins')

        self.__QtDllsToCopy = [
            'Qt{}Core.dll', 
            'Qt{}Gui.dll', 
            'Qt{}Script.dll', 
            'Qt{}ScriptTools.dll', 
            'Qt{}Sql.dll', 
            'Qt{}Widgets.dll', 
            'Qt{}Xml.dll',
            'Qt{}Svg.dll',
            'libEGL.dll'
        ]
        self.__VersionedDlls = [
            'd3dcompiler*.dll',
            'libgcc_s_dw*.dll',
            'libGLES*.dll',
            'libstdc++*.dll',
            'libwinpthread*.dll'
        ]
        self.__QtFoldersToCopy  = [
            'platforms',
            'imageformats',
            'iconengines',
            'sqldrivers',
            'styles'
        ]

        super(QtPackage, self).__init__()

        self.DisplayName = 'Qt Framework Binaries'
        self.Description = 'Библиотеки, необходимые для работы программы'
        self.Name = 'com.rs.fmt.workfmt.qtbinaries'
        self.ReleaseDate = self.__qt_element.getReleaseDate()
        self.ForcedInstallation = True

    def getVersion(self):
        return self.__qt_element.getVersion() + '.1'

    def __extract_major(self, version):
        dot = version.find('.')
        major = version
        if dot != -1:
            major = version[:dot]
        return major

    def __is_qt_debug_dll(self, path):
        is_not_qt_debug = False
        base_path = os.path.dirname(path)
        base = ntpath.basename(path)
        filename, file_extension = os.path.splitext(base)
        filename_tmp = os.path.join(base_path, filename[:-1] + file_extension)
        if (os.path.isfile(filename_tmp)):
            is_not_qt_debug = True
        return is_not_qt_debug

    def __copy_qt_files(self, src_path, dst_path, mask = '/*.dll'):
        files = glob.glob(src_path + mask)
        for file in files:
            if not self.__is_qt_debug_dll(file):
                dst_dll = os.path.join(dst_path, ntpath.basename(file))
                self.__copy_file_destrib(file, dst_dll)

    def __copy_file_destrib(self, fromf, tof):
        self.copyFileToDestrib(fromf, tof)

    def __check_dir(self, path):
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

    def __copy_qt_dir(self, qtpluginsdir, destribfolder, _dir, mask = '/*.dll'):
        print('Copy Qt ', _dir, ' : ', sep='')
        destrib_platforms = os.path.join(destribfolder, _dir)
        QtDir = os.path.join(qtpluginsdir, _dir)
        if self.__check_dir(destrib_platforms):
            self.__copy_qt_files(QtDir, destrib_platforms, mask)

    def makeData(self, datadir):
        major = self.__extract_major(self.__qt_element.getVersion())

        for dll in self.__QtDllsToCopy:
            qtdll = dll.format(major)
            qtdllpath = os.path.join(self.__QtBinDir, qtdll)
            qtdlldstpath = os.path.join(self.DataPath, qtdll)
            self.__copy_file_destrib(qtdllpath, qtdlldstpath)

        for dll in self.__VersionedDlls:
            qtdllpath = os.path.join(self.__QtBinDir, dll)
            for customdll in glob.glob(qtdllpath):
                qtdlldstpath = os.path.join(self.DataPath, os.path.basename(customdll))
                self.__copy_file_destrib(customdll, qtdlldstpath)

        for folder in self.__QtFoldersToCopy:
            print()
            self.__copy_qt_dir(self.__QtPluginsDir, self.DataPath, folder)

        print()
        self.__copy_qt_dir(self.__qt_element.getPath(), self.DataPath, 'translations', '/*_ru.qm')

class DBFileTool(InstallerPackageInfoBase):
    def __init__(self):
        self.__fmtdir = WorkFmtConfig.inst().getWorkFmtSourceDir()
        self.__releasfile = os.path.join(self.__fmtdir, 'DBFileTool/{}/DBFileTool.exe'.format(WorkFmtConfig.inst().getBinaryType()))
        super(DBFileTool, self).__init__()

        today = date.today()
        self.DisplayName = 'DB file tool'
        self.Description = 'Утилита для работы с *.dat файлами'
        self.Name = 'com.rs.fmt.workfmt.dbfiletool'
        self.ReleaseDate = today.strftime("%Y-%m-%d")
        self.ForcedInstallation = True

    def getVersion(self):
        try:
            ver = getExeVersion(self.__releasfile)
            print(ver)
            return ver
        except:
            return "1.0.0.0"

    def makeData(self, datadir):
        srcexefile = self.__releasfile
        basedbfiletoolname = os.path.basename(self.__releasfile)
        dstexefile = os.path.join(self.DataPath, os.path.basename(basedbfiletoolname))
        copyfile(srcexefile, dstexefile)

class RsComponentBase(InstallerPackageInfoBase):
    def __init__(self):
        self.FmtDir = WorkFmtConfig.inst().getWorkFmtSourceDir()
        self.__VersionFile = os.path.join(self.FmtDir, 'qrsd\\toolsversion.txt')
        self.VersionConfig = configparser.ConfigParser()
        self.__readVersionFile()

        super(RsComponentBase, self).__init__()

        today = date.today()
        self.ReleaseDate = today.strftime("%Y-%m-%d")
        self.ForcedInstallation = True

    def __readVersionFile(self):
        self.VersionConfig.read(self.__VersionFile)

    def getVersion(self):
        try:
            ver = self.VersionConfig['Version']['Rsd'] + \
                '.' + self.VersionConfig['Version']['QRsdAddVersion']
            return ver
        except:
            return "1.0.0.0"

    def copyArray(self, rsdDllDir, srcDir):
        for dll in rsdDllDir:
            dllpath = os.path.join(srcDir, dll)
            dlldstpath = os.path.join(self.DataPath, dll)
            self.copyFileToDestrib(dllpath, dlldstpath)

class RsdDriver(RsComponentBase):
    def __init__(self):
        super(RsdDriver, self).__init__()
        self.__RsdDllDir = os.path.join(
            self.FmtDir, 'qrsd/rsd/lib/{}'.format(WorkFmtConfig.inst().getBinaryType()))
        self.__releasfile = os.path.join(
            self.FmtDir, 'qrsd/{}/qrsd.dll'.format(WorkFmtConfig.inst().getBinaryType()))
        
        '''self.__ToolsToCopy = [
            'rsrtlwm.dll',
            'RSScript.dll'
        ]'''

        self.__RsdToCopy = [
            'rddrvo.dll',
            'rddrvou.dll',
            'rdrset.dll',
            'RsBtr.dll',
            'rsdc.dll',
            'dbinit.exe', 
            'fmtxml.exe',
            'fmtxml.xsd'
        ]

        self.DisplayName = 'Rsd Connector For Qt'
        self.Description = 'Драйвер подключения к БД через rsd'
        self.Name = 'com.rs.fmt.workfmt.rsddriver'

    def getVersion(self):
        try:
            ver = self.VersionConfig['Version']['Rsd'] + \
                '.' + self.VersionConfig['Version']['QRsdAddVersion']
            return ver
        except:
            return "1.0.0.0"

    def makeData(self, datadir):
        srcexefile = self.__releasfile
        basedbfiletoolname = os.path.basename(self.__releasfile)
        dstexefile = os.path.join(self.DataPath, 'sqldrivers/' + os.path.basename(basedbfiletoolname))
        
        try:
            driverdir = os.path.join(self.DataPath, 'sqldrivers')
            os.mkdir(driverdir)
            copyfile(srcexefile, dstexefile)
        except FileExistsError:
            copyfile(srcexefile, dstexefile)
        except Exception as e:
            print('Fail: ' + str(e))

        self.copyArray(self.__RsdToCopy, self.__RsdDllDir)

class RsTools(RsComponentBase):
    def __init__(self):
        super(RsTools, self).__init__()
        self.__RsDllDir = os.path.join(
            self.FmtDir, 'qrsd/tools/lib/{}'.format(WorkFmtConfig.inst().getBinaryType()))

        self.__ToolsToCopy = [
            'rsrtlwm.dll',
            'RSScript.dll'
        ]

        self.DisplayName = 'Rs Tools Binaries'
        self.Description = 'Динамический инструментальный Runtime'
        self.Name = 'com.rs.fmt.workfmt.rsruntime'

    def getVersion(self):
        try:
            ver = self.VersionConfig['Version']['Tools'] + \
                '.' + self.VersionConfig['Version']['QRsdAddVersion']
            return ver
        except:
            return "1.0.0.0"

    def makeData(self, datadir):
        self.copyArray(self.__ToolsToCopy, self.__RsDllDir)

class ChangeLogComponent(RsComponentBase):
    def __init__(self):
        super(ChangeLogComponent, self).__init__()

        self.DisplayName = 'ChangeLog'
        self.Description = 'История изменений'
        self.Name = 'com.rs.fmt.workfmt.changelog'
        self.ForcedUpdate = True
        self.ForcedInstallation = True
        self.Default = None
        self.Virtual = True

    def getVersion(self):
        return "1"

    def makeData(self, datadir):
        fmtdir = WorkFmtConfig.inst().getWorkFmtSourceDir()
        mask = os.path.join(fmtdir, '**/com.rs.fmt*.xml')
        
        for filename in glob.glob(mask, recursive=True):
            basedbfiletoolname = os.path.basename(filename)
            dstexefile = os.path.join(self.DataPath, 'changelog/' + os.path.basename(basedbfiletoolname))

            try:
                driverdir = os.path.join(self.DataPath, 'changelog')
                os.mkdir(driverdir)
                copyfile(filename, dstexefile)
            except FileExistsError:
                copyfile(filename, dstexefile)
            except Exception as e:
                print('Fail: ' + str(e))
            
class WorkFmtInstaller(InstallCreator):
    def __init__(self):
        super(WorkFmtInstaller, self).__init__(name = 'WorkFmt', 
            version = '1.0.2', 
            title = 'WorkFmt', 
            publisher = 'Serpkov Nikita')
        
        self.MaintenanceToolName = 'WorkFmtMaintenanceTool'
        self.MaintenanceToolIniFile = 'WorkFmtMaintenanceTool.ini'
        self.WizardDefaultWidth = 1000

        self.addRepository('file://Serpkovn/workfmtrepo/', 'Network Repo')
        self.addRepository('file:///d:/Work/workfmtrepo/', 'Local Repo')

        self.__WorkFmtMainPackage = WorkFmtMainPackage()
        self.__QtPackage = QtPackage()
        self.__DBFileTool = DBFileTool()
        self.__RsdDriver = RsdDriver()
        self.__RsdTools = RsTools()
        self.__ChangeLog = ChangeLogComponent()

        self.addPage(self.__WorkFmtMainPackage)
        self.addPage(self.__DBFileTool)
        self.addPage(self.__QtPackage)
        self.addPage(self.__RsdDriver)
        self.addPage(self.__RsdTools)
        self.addPage(self.__ChangeLog)
