
all: exe

exe: checkPerm.h main.cpp lib
	g++ --std=c++17 -lstdc++fs main.cpp -Wl,-rpath,. -L. -lCheckperm -o checkperm

lib: checkPerm.h checkPerm.cpp
	g++ --std=c++17 -lstdc++fs -shared checkPerm.cpp -o libCheckperm.so

clean:
	-rm checkperm libCheckperm.so 2>/dev/null
