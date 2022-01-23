from xml.dom import minidom
import os, shutil, subprocess
from installer.instpackageinfo import InstallerPackageInfoBase
from config.WorkFmtConfig import WorkFmtConfig
from typing import List
from enum import Enum

def addTextElement(doc, parent, tag, value):
    if value != None:
        element = doc.createElement(tag)

        text = ''
        if type(value) == type(True):
            text = doc.createTextNode(str(value).lower())
        elif type(value) != type(str):
            text = doc.createTextNode(str(value))
        else:
            text = doc.createTextNode(value)

        element.appendChild(text)
        parent.appendChild(element)


class Repository:
    def __init__(self, Url : str, DisplayName : str, Enabled = 1, Username = None, Password = None):
        self.Url = Url
        self.DisplayName = DisplayName
        self.Enabled = Enabled
        self.Username = Username
        self.Password = Password

    def getXmlElement(self, doc, parent):
        Repository = doc.createElement('Repository')
        addTextElement(doc, Repository, 'Url', self.Url)
        addTextElement(doc, Repository, 'DisplayName', self.DisplayName)
        addTextElement(doc, Repository, 'Enabled', self.Enabled)
        addTextElement(doc, Repository, 'Username', self.Username)
        addTextElement(doc, Repository, 'Password', self.Password)

        parent.appendChild(Repository)

class InstallCreator:
    def __init__(self, name : str, version : str, title : str, publisher : str, startmenudir : str = None, targetdir : str = None):
        self.Name = name
        self.Version = version
        self.Title = title
        self.Publisher = publisher
        self.ProductUrl = None
        self.Icon = None
        self.WizardDefaultWidth = 800
        self.WizardDefaultHeight = 600
        self.WizardMinimumWidth = None
        self.WizardMinimumHeight = None
        self.MaintenanceToolName = None
        self.MaintenanceToolIniFile = None
        self.AllowNonAsciiCharacters = True
        
        self.Pages = []
        self.RemoteRepositories = []

        if startmenudir != None:
            self.StartMenuDir = startmenudir
        else:
            self.StartMenuDir = self.Name

        if targetdir != None:
            self.TargetDir = targetdir
        else:
            self.TargetDir = '@HomeDir@/' + self.Name

    def addPage(self, page : InstallerPackageInfoBase):
        self.Pages.append(page)

    def addRepository(self, Url : str, DisplayName : str, Enabled = 1, Username = None, Password = None):
        self.RemoteRepositories.append(Repository(Url, DisplayName, Enabled, Username, Password))
    
    def __makeXml(self):
        doc = minidom.Document()
        doc.createProcessingInstruction('xml', 'version=''1.0'' encoding=''UTF-8''')

        installer = doc.createElement('Installer')
        doc.appendChild(installer)

        addTextElement(doc, installer, 'Name', self.Name)
        addTextElement(doc, installer, 'Version', self.Version)
        addTextElement(doc, installer, 'Title', self.Title)
        addTextElement(doc, installer, 'Publisher', self.Publisher)
        addTextElement(doc, installer, 'StartMenuDir', self.StartMenuDir)
        addTextElement(doc, installer, 'TargetDir', self.TargetDir)

        addTextElement(doc, installer, 'ProductUrl', self.ProductUrl)
        addTextElement(doc, installer, 'Icon', self.Icon)
        addTextElement(doc, installer, 'WizardDefaultWidth', self.WizardDefaultWidth)
        addTextElement(doc, installer, 'WizardDefaultHeight', self.WizardDefaultHeight)
        addTextElement(doc, installer, 'WizardMinimumWidth', self.WizardMinimumWidth)
        addTextElement(doc, installer, 'WizardMinimumHeight', self.WizardMinimumHeight)
        addTextElement(doc, installer, 'MaintenanceToolName', self.MaintenanceToolName)
        addTextElement(doc, installer, 'MaintenanceToolIniFile', self.MaintenanceToolIniFile)
        addTextElement(doc, installer, 'AllowNonAsciiCharacters', self.AllowNonAsciiCharacters)

        if len(self.RemoteRepositories):
            RemoteRepositories = doc.createElement('RemoteRepositories')

            for Repo in self.RemoteRepositories:
                Repo.getXmlElement(doc, RemoteRepositories)

            installer.appendChild(RemoteRepositories)

        return doc.toprettyxml(indent='  ', newl='\n', encoding="utf-8")

    def __str__(self):
        return self.__makeXml()

    def __saveConfigFile(self, path):
        confiffilename = os.path.join(path, 'config.xml') 
        f = open(confiffilename, "xb")
        f.write(self.__makeXml())
        f.close()

    def __getInstallerBinaryPath(self):
        qtinstallerframework = WorkFmtConfig.inst().getQtInstallerFramework()
        binpath = os.path.join(qtinstallerframework.getPath(), 'bin')
        return binpath

    def __binarycreator(self, sourcepath):
        saveworkdir = os.getcwd()
        os.chdir(sourcepath)

        binarycreatorpath = os.path.join(self.__getInstallerBinaryPath(), 'binarycreator.exe')
        repogen = os.path.join(self.__getInstallerBinaryPath(), 'repogen.exe')

        args = '{} -c config\config.xml --online-only -p packages FmtInstaller.exe'.format(binarycreatorpath)
        proc = subprocess.Popen(args, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

        print()
        print("BinaryCreator Output: ")
        for line in proc.stdout:
            line = line.rstrip()
            print(line.decode('utf-8'))

        print()
        print("BinaryCreator Errors: ")
        for line in proc.stderr:
            line = line.rstrip()
            print(line.decode('utf-8'))
        
        if len(self.RemoteRepositories):
            for repo in self.RemoteRepositories:
                url = repo.Url.replace('file:///', '')
                args = '{} -p packages {}'.format(repogen, url)
                proc = subprocess.Popen(args, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

                print()
                print("RepoGen Output: ")
                for line in proc.stdout:
                    line = line.rstrip()
                    print(line.decode('utf-8'))

                print()
                print("RepoGen Errors: ")
                for line in proc.stderr:
                    line = line.rstrip()
                    print(line.decode('utf-8'))

    def __removeSubdirs(self, path, subdirs):
        for element in subdirs:
            elementpath = os.path.join(path, element) 
            if os.path.isfile(elementpath):
                os.remove(elementpath)
            elif os.path.isdir(elementpath):
                shutil.rmtree(elementpath)

    def make(self, path : str):
        subdirs = os.listdir(path)
        confifdir = os.path.join(path, 'config') 
        packagesdir = os.path.join(path, 'packages') 

        # clear old files
        self.__removeSubdirs(path, subdirs)

        #clear old repositoryes
        for repo in self.RemoteRepositories:
            url = repo.Url.replace('file:///', '')
            subdirs = os.listdir(url)
            self.__removeSubdirs(url, subdirs)

        try:
            os.mkdir(confifdir)
        except FileExistsError:
            pass

        self.__saveConfigFile(confifdir)
        
        try:
            os.mkdir(packagesdir)
        except FileExistsError:
            pass

        for obj in self.Pages:
            obj.make(packagesdir)

        self.__binarycreator(path)