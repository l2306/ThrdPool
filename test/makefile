
all: test_win test_inx

test_win:
	gcc test0.c  -I../inc -L.. -lThrdPool -o test4c
	g++ test2.cc -I../inc -L.. -lThrdPool -o test4cc
    #dll放在同级目录可用， -Wl,-rpath不起作用
    
test_inx:
	g++ test2.cc  -o test4cc  -I../inc -L.. -lThrdPool
	gcc test0.c   -o test4c   -I../inc -Wl,-rpath=.. -L.. -lThrdPool
	#g++ test2.cc -I../inc -Wl,-rpath=lib -Llib -lThrdPool -o test4cc

