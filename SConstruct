#env = Environment()
env=Environment(CPPPATH='',
                CPPDEFINES=[],
                LIBS=[],
                CXXFLAGS="-std=c++0x -std=gnu++11 ",
                )
#GCC_VERSION = commands.getoutput(env['CXX'] + ' -dumpversion')
env.Append(LIBS = ["libBox2D"]);
env.Append(LIBS = ["sfml-graphics","sfml-window","sfml-system"]);
env.Append(CCFLAGS = '-g -std=c++0x')

env.Program("my_game", Glob("source/*.cpp") + Glob("source/pugiXml/*.cpp") )
