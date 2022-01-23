import sys, installer

from config.WorkFmtConfig import *
from tools.qtdestrib import *
#from installtools.workfmtinstaller import WorkFmtInstaller

dstrlist = QtDestribList()

print(WorkFmtConfig.inst().getInstallerPath())
print('Select Qt version: ')
for index, element in enumerate(dstrlist.getQtList(), start=1):
    print(str(index) + ': ' + element.getVersion() + ' ' + element.getComplect() + ' (' + element.getPath() + ')')

print(' > ', end='')

try:
    qtselected = int(input())

    if qtselected <= 0 or qtselected > dstrlist.size():
        raise ValueError()

    WorkFmtConfig.inst().setQtComplect(dstrlist.getElement(qtselected - 1))
except ValueError:
    print('Wrong version selected')
    sys.exit()

print('')
print('Select QtInstallerFramework version: ')
for index, element in enumerate(dstrlist.getQtInstallerList(), start=1):
    print(str(index) + ': ' + element.getVersion() + ' (' + element.getPath() + ')')

print(' > ', end='')

try:
    qtselected = int(input())

    if qtselected <= 0 or qtselected > dstrlist.installerCount():
        raise ValueError()

    WorkFmtConfig.inst().setQtInstallerFramework(dstrlist.getInstallerElement(qtselected - 1))
except ValueError:
    print('Wrong installer framework version selected')
    sys.exit()

installerCreator = installer.workfmtinstaller.WorkFmtInstaller()
installerCreator.make(WorkFmtConfig.inst().getInstallerPath())
