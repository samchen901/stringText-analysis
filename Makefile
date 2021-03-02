CC=gcc
CFLAGS=-Wall -Werror -g
OBJS=mostFreqWords.o wordTree.o wordCutter.o

all : mostFreqWords stem

mostFreqWords : $(OBJS)

stem : stem.o wordCutter.o

mostFreqWords.o : mostFreqWords.c wordTree.h WFreq.h wordCutter.h 

wordTree.o : wordTree.c wordTree.h WFreq.h

wordCutter.o : wordCutter.c

stem.o : stem.c wordCutter.h

clean :
	rm -f mostFreqWords stem $(OBJS) stem.o core
