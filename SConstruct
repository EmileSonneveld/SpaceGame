#env = Environment()
env=Environment(CPPPATH='',
                CPPDEFINES=[],
                LIBS=[],
                CXXFLAGS="-std=c++0x"
                )
#GCC_VERSION = commands.getoutput(env['CXX'] + ' -dumpversion')

env.Append(LIBS = ["libBox2D"]);
env.Append(LIBS = ["sfml-graphics","sfml-window","sfml-system"]);

env.Program( Glob("source/*") + Glob("source/pugiXml/*") )
