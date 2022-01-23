from xml.dom import minidom
import os

class LicenseElement:
    def __init__(self, _name : str, _file : str):
        self.name = _name
        self.file = _file

#https://doc.qt.io/qtinstallerframework/ifw-component-description.html#package-information-file-syntax
class InstallerPackageInfoBase:
    __SortingPriority = 0

    def __init__(self):
        InstallerPackageInfoBase.__SortingPriority += 1

        self.DisplayName = 'test name'
        self.Description = None
        self.Version = self.getVersion()
        self.ReleaseDate = None
        self.Name = None
        self.Dependencies = []
        self.Virtual = None
        self.Licenses = []
        self.Script = None
        self.UserInterfaces = []
        self.Translations = []
        self.DownloadableArchives = []
        self.AutoDependOn = []
        self.SortingPriority = InstallerPackageInfoBase.__SortingPriority
        self.UpdateText = None
        self.Default = True
        self.ForcedInstallation = False
        self.Replaces = []
        self.RequiresAdminRights = False
        self.Checkable = True
        self.ExpandedByDefault = True

        self.VendorPath = None
        self.DataPath = None
        self.MetaPath = None

    def getVersion(self):
        return ''

    def addLicense(self, _name, _file):
        self.Licenses.append(LicenseElement(_name, _file))

    def makeData(self, datadir):
        pass

    def make(self, _path):
        self.VendorPath = os.path.join(_path, self.Name)
        self.DataPath = os.path.join(self.VendorPath, 'data')
        self.MetaPath = os.path.join(self.VendorPath, 'meta')

        try:
            os.mkdir(self.VendorPath)
        except FileExistsError:
            pass
            
        try:
            os.mkdir(self.DataPath)
        except FileExistsError:
            pass

        try:
            os.mkdir(self.MetaPath)
        except FileExistsError:
            pass

        self.__saveConfigFile(self.MetaPath)

        if self.VendorPath is None:
            raise ValueError("'Vendor path' is not defined")

        if self.DataPath is None:
            raise ValueError("'Data path' is not defined")

        if self.MetaPath is None:
            raise ValueError("'Meta path' is not defined")

        self.makeData(self.DataPath)

    def __addTextElement(self, doc, parent, tag, value):
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

    def __addArray(self, doc, parent, tag, value):
        for elem in value:
            self.__addTextElement(doc, parent, tag, elem)

    def __makeXml(self):
        doc = minidom.Document()
        doc.createProcessingInstruction('xml', 'version=''1.0'' encoding=''UTF-8''')
        package = doc.createElement('Package')
        doc.appendChild(package)

        if self.DisplayName == None:
            raise ValueError('Empty field ''DisplayName''')

        if self.Name == None:
            raise ValueError('Empty field ''Name''')

        self.__addTextElement(doc, package, 'DisplayName', self.DisplayName)
        self.__addTextElement(doc, package, 'Description', self.Description)
        self.__addTextElement(doc, package, 'Version', self.Version)
        self.__addTextElement(doc, package, 'ReleaseDate', self.ReleaseDate)
        self.__addTextElement(doc, package, 'Name', self.Name)

        if len(self.Dependencies) != 0:
            depstr = ','.join(self.Dependencies)
            self.__addTextElement(doc, package, 'Dependencies', depstr)

        self.__addTextElement(doc, package, 'Virtual', self.Virtual)

        if len(self.Licenses) != 0:
            _licenses = doc.createElement('Licenses')
            for _license in self.Licenses:
                _licenseElement = doc.createElement('License')
                _licenseElement.setAttribute('name', _license.name)
                _licenseElement.setAttribute('file', _license.file)
                _licenses.appendChild(_licenseElement)

            package.appendChild(_licenses)

        self.__addTextElement(doc, package, 'Script', self.Script)

        if len(self.UserInterfaces) != 0:
            _ui = doc.createElement('UserInterfaces')
            self.__addArray(doc, _ui, 'UserInterface', self.UserInterfaces)
            package.appendChild(_ui)

        if len(self.Translations) != 0:
            _tr = doc.createElement('Translations')
            self.__addArray(doc, _tr, 'Translation', self.Translations)
            package.appendChild(_tr)

        if len(self.AutoDependOn) != 0:
            dependstr = ','.join(self.AutoDependOn)
            self.__addTextElement(doc, package, 'AutoDependOn', dependstr)

        self.__addTextElement(doc, package, 'SortingPriority', self.SortingPriority)
        self.__addTextElement(doc, package, 'UpdateText', self.UpdateText)
        self.__addTextElement(doc, package, 'Default', self.Default)
        self.__addTextElement(doc, package, 'ForcedInstallation', self.ForcedInstallation)

        if len(self.AutoDependOn) != 0:
            replacestr = ','.join(self.Replaces)
            self.__addTextElement(doc, package, 'Replaces', replacestr)

        if len(self.DownloadableArchives) != 0:
            archstr = ','.join(self.DownloadableArchives)
            self.__addTextElement(doc, package, 'DownloadableArchives', archstr)

        self.__addTextElement(doc, package, 'RequiresAdminRights', self.RequiresAdminRights)
        
        if self.Default == None:
            self.__addTextElement(doc, package, 'Checkable', self.Checkable)
            
        self.__addTextElement(doc, package, 'ExpandedByDefault', self.ExpandedByDefault)

        return doc.toprettyxml(indent='  ', newl='\n', encoding="utf-8")

    def __saveConfigFile(self, path):
        confiffilename = os.path.join(path, 'package.xml') 
        f = open(confiffilename, "xb")
        f.write(self.__makeXml())
        f.close()

    def __str__(self):
        return __makeXml()
