import os, re, ntpath, shutil, zipfile, glob
from ctypes import windll

DestribFolder = os.path.realpath('destrib')
ScriptPath = os.path.realpath(__file__)
ProjectPath = os.path.abspath(os.path.join(os.path.dirname( __file__ ), '..'))
QtToolSet = ''

print('ScriptPath    =', ScriptPath)
print('DestribFolder =', DestribFolder)
print('ProjectPath   =', ProjectPath)

QtDllsToCopy  = [
    'Qt{}Core.dll', 
    'Qt{}Gui.dll', 
    'Qt{}Script.dll', 
    'Qt{}ScriptTools.dll', 
    'Qt{}Sql.dll', 
    'Qt{}Widgets.dll', 
    'Qt{}Xml.dll'
]

QtFoldersToCopy  = [
    'platforms',
    'imageformats',
    'iconengines',
    'sqldrivers',
    'styles'
]

WorkFmtToCopy = [
    'DBFileTool\\release\\DBFileTool.exe',
    'FmtDbgHelp\\release\\FmtDbgHelp.dll',
    'FmtLib\\release\\FmtLib.dll',
    'FmtScriptTest\\release\\FmtScriptTest.exe',
    'WorkFMT\\release\\WorkFMT.exe',
    ['FmtScript\\release\\FmtScript.dll', 'script\\FmtScript.dll']
]

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
        except:
            pass
    return qt_bin

def select_qt_complect():
    qt = ''
    qt_dirs = find_qt_bin()

    def read_qt_version(path):
        version = None
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
            raise Exception('Can\'t read file ' + mkspecs)
        return version

    version = None
    if len(qt_dirs) != 0:
        num = 1
        print("Select version to copy files:")
        for qtdir in qt_dirs:
            build_complect = os.path.basename(os.path.normpath(qtdir))
            version = read_qt_version(qtdir)
            print(num, ') ', build_complect, ' ', version, '(', build_complect, ')', sep='')
            num += 1
        select_id = int(input('> ')) - 1
        qt = qt_dirs[select_id]
    else:
        print("Enter Qt folder to copy files:")
        qt = input('> ')
        version = read_qt_version(qt)

    return qt, version

def extract_major(version):
    dot = version.find('.')
    major = version
    if dot != -1:
        major = version[:dot]
    return major

def copy_file_destrib(fromf, tof):
    try:
        base = ntpath.basename(fromf)
        print('Copy file \'', base, '\': ', sep='', end='')
        shutil.copyfile(fromf, tof)
        print('Ok')
    except IOError:
        print('Fail. Target directory must be writable.')
    except:
        print('Fail')

def is_qt_debug_dll(path):
    is_not_qt_debug = False
    base_path = os.path.dirname(path)
    base = ntpath.basename(path)
    filename, file_extension = os.path.splitext(base)
    filename_tmp = os.path.join(base_path, filename[:-1] + file_extension)
    if (os.path.isfile(filename_tmp)):
        is_not_qt_debug = True
    return is_not_qt_debug

def copy_qt_files(src_path, dst_path, mask = '/*.dll'):
    files = glob.glob(src_path + mask)
    for file in files:
        if not is_qt_debug_dll(file):
            dst_dll = os.path.join(dst_path, ntpath.basename(file))
            copy_file_destrib(file, dst_dll)

def copy_qt_dir(qtpluginsdir, destribfolder, _dir, mask = '/*.dll'):
    print('Copy Qt ', _dir, ' : ', sep='')
    destrib_platforms = os.path.join(destribfolder, _dir)
    QtDir = os.path.join(qtpluginsdir, _dir)
    if check_dir(destrib_platforms):
        copy_qt_files(QtDir, destrib_platforms, mask)

def read_winfmt_rc(projectpath):
    winfmt_rc_path = os.path.join(projectpath, 'WorkFMT\\res\\winfmt.rc')
    ProductName = ''
    ProductVersion = ''
    try:
        f = open(winfmt_rc_path, 'r', encoding='utf16')
        lines = f.readlines()
        for line in lines:
            if 'VALUE' in line:
                tmp = line.split(', ')
                if 'ProductName' in tmp[0]:
                    ProductName = tmp[1].replace('"', '').strip()
                if 'ProductVersion' in tmp[0]:
                    ProductVersion = tmp[1].replace('"', '').strip()
        f.close()
    except:
        raise Exception('Can\'t read file ' + winfmt_rc_path)
    return ProductName, ProductVersion

print()

try:
    QtToolSet, version = select_qt_complect()
    major = extract_major(version)

    print()
    QtBinDir = os.path.join(QtToolSet, 'bin')
    QtTranslationsDir = os.path.join(QtToolSet, 'translations')
    QtPluginsDir = os.path.join(QtToolSet, 'plugins')

    print('QtToolSet         =', QtToolSet)
    print('QtBinDir          =', QtBinDir)
    print('QtTranslationsDir =', QtTranslationsDir)
    print('QtPluginsDir      =', QtPluginsDir)
    print()

    print('Copy Qt binary: ')
    for dll in QtDllsToCopy:
        qtdll = dll.format(major)
        qtdllpath = os.path.join(QtBinDir, qtdll)
        qtdlldstpath = os.path.join(DestribFolder, qtdll)
        copy_file_destrib(qtdllpath, qtdlldstpath)

    for folder in QtFoldersToCopy:
        print()
        copy_qt_dir(QtPluginsDir, DestribFolder, folder)
    
    print()
    copy_qt_dir(QtToolSet, DestribFolder, 'translations', '/*_ru.qm')
    
    print()
    print('Copy WorkFmt binary: ')
    for fmtfile in WorkFmtToCopy:
        if isinstance(fmtfile, str):
            fmtfilepath = os.path.join(ProjectPath, fmtfile)
            fmtdstfile = ntpath.basename(fmtfilepath)
            fmtdstfilepath = os.path.join(DestribFolder, fmtdstfile)
            copy_file_destrib(fmtfilepath, fmtdstfilepath)
        else:
            fmtfilepath = os.path.join(ProjectPath, fmtfile[0])
            fmtdstfilepath = os.path.join(DestribFolder, fmtfile[1])
            if check_dir(os.path.dirname(fmtdstfilepath)):
                copy_file_destrib(fmtfilepath, fmtdstfilepath)

    try:
        print()
        print('Unzipping destrib files: ', sep='', end='')
        destrib_zip_path = os.path.join(os.path.join(ProjectPath, 'Destrib'), 'Destrib.zip')
        destrib_zip = zipfile.ZipFile(destrib_zip_path, 'r')
        destrib_zip.extractall(DestribFolder)
        destrib_zip.close()
        print('Ok')
    except:
        print('Fail')

    try:
        print()
        for f in glob.glob(os.path.join(DestribFolder, "*.zip")):
            os.remove(f)

        ProductName, ProductVersion = read_winfmt_rc(ProjectPath)
        files = glob.glob(DestribFolder + '/**/*.*', recursive=True)
        pack_file_name = os.path.join(DestribFolder, ProductName + '_' + ProductVersion.replace('.','_') + '.zip')
        z = zipfile.ZipFile(pack_file_name, 'w')
        for file in files:
            tmp = file.replace(DestribFolder + '\\', DestribFolder + '\\.\\')
            print('Add file to zip: ', tmp)
            z.write(tmp)
        z.close()

    except Exception as inst:
        print(inst.args[0])

except Exception as inst:
    print(inst.args[0])
    
os.system("pause")
