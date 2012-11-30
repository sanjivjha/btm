CC=g++
CFLAGS=-c -pipe -O3
#SRC=$(wildcard *.cpp)
MODEL_OBJ=model.o infer.o main.o
MODEL_EXE=btm

all:$(MODEL_EXE) 

$(MODEL_EXE):$(MODEL_OBJ)
	$(CC) $(MODEL_OBJ) -o $@ 

main.o:model.h
model.o:model.h biterm.h
infer.o:infer.h biterm.h

%.o:%.cpp
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(MODEL_OBJ) $(MODEL_EXE)
