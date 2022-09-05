
import tempfile, os, subprocess

class ProjectBulder:
    def __init__(self, config) -> None:
        self.__config = config
        self.__cmd_file_name = ''

    def __makeCommandFile(self, tmpdirname):
        self.__cmd_file_name = os.path.join(tmpdirname, 'build.cmd')
        qt_vars_bat = os.path.join(self.__config.getQtComplect().getPath(), 'bin\\qtenv2.bat')
        qt_complect = self.__config.getQtComplect().getComplect()
        qt_qmake = os.path.join(self.__config.getQtComplect().getPath(), 'bin\\qmake.exe')
        project_file = os.path.join(self.__config.getWorkFmtSourceDir(), 'WorkFMT.pro')
        
        compiler_spec = ' -spec win32-g++'
        compiler = 'mingw32-make.exe'
        compiler_make = 'mingw32-make.exe qmake_all'
        compiler_install = 'mingw32-make.exe install'
        config_param = ' "CONFIG+=' + self.__config.getBinaryType() + '"'
        with open(self.__cmd_file_name, 'wt') as f:
            print('call "' + qt_vars_bat + '"', end='\n', file=f)

            if (qt_complect.startswith('msvc')):
                compiler_spec = ' -spec win32-msvc'
                compiler = 'nmake'
                compiler_make = 'nmake ' + self.__config.getBinaryType()
                compiler_install = 'nmake install'
                print('call "' + self.__config.getMsvcBat() + '"', end='\n', file=f)

            print('cd "' + self.__config.getWorkFmtSourceDir() + '"', end='\n', file=f)

            build_cmd = qt_qmake + ' ' + project_file + compiler_spec + config_param
            print(build_cmd, end='\n', file=f)
            print(compiler + ' clean' , end='\n', file=f)
            print(compiler_make, end='\n', file=f)
            print(compiler_install, end='\n', file=f)

    def compile(self):
        with tempfile.TemporaryDirectory() as tmpdirname:
            self.__makeCommandFile(tmpdirname)
            print(self.__cmd_file_name)
            subprocess.call([self.__cmd_file_name],
                            cwd=self.__config.getWorkFmtSourceDir())
