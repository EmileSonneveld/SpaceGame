
env=Environment()

env.Append(LIBS = ["libBox2D"]);
env.Append(LIBS = ["sfml-graphics","sfml-window","sfml-system"]);
env.Append(CXXFLAGS = '-std=gnu++11')

env.Program("SpaceGame", Glob("source/*.cpp") + Glob("source/pugiXml/*.cpp") )
