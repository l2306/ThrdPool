
INCFLAGS	=-I./inc
CFLAGS		=-g -w -shared
LDFLAGS		=-lpthread
CC			=g++
target		=libThrdPool.so
src			=$(wildcard ./src/*.c)
objs		=$(patsubst %.c,%.o,$(src))

VPATH	= ./src

INST	=/usr/local/lib

$(target):$(objs)
	$(CC) $^ -shared $(LDFLAGS) -o $@
%.o:%.c
	$(CC) $< -c -fpic -o $@

.PHONY:clean
clean:
	-rm -f $(objs) $(target)

test:
	gcc test/test0.c  -I../inc -lThrdPool -o test4c
	g++ test/test2.cc -I../inc -lThrdPool -o test4cc
	
unst:
	rm $(INST)/$(target)
	ldconfig
inst:
	cp $(target) $(INST)
	ldconfig

