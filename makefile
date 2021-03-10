
UNAME= $(shell uname)

ifeq ($(UNAME), Linux)
  suffix=so
endif
ifeq ($(UNAME), CYGWIN_NT-10.0)
  suffix=dll
endif


INCFLAGS    =-I./inc
CFLAGS      =-w -shared
LDFLAGS     =-lpthread
CC          =g++
tgt_dyn     =libThrdPool.$(suffix)
tgt_sta     =libThrdPool.a
src         =$(wildcard ./src/*.c)
objs        =$(patsubst %.c,%.o,$(src))

VPATH	= ./src

INST	=/usr/local/lib

all: $(tgt_sta) $(tgt_dyn)

$(tgt_dyn):$(objs)
	$(CC) $^ -o $@  -shared $(LDFLAGS) 
    
$(tgt_sta):$(objs)
	ar -cr $@ $^ 

%.o:%.c
	$(CC) $< -c -fpic -o $@

.PHONY:clean
clean:
	-rm -f $(objs) $(tgt_sta) $(tgt_dyn)

test:
	gcc test/test0.c  -I../inc -lThrdPool -o test4c
	g++ test/test2.cc -I../inc -lThrdPool -o test4cc
	
unst:
	rm $(INST)/$(target)
	ldconfig
inst:
	cp $(target) $(INST)
	ldconfig

