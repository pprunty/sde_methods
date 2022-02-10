CC	 := g++
CFLAGS 	:= -Wall -Wextra -O3 --std=c++17
LDFLAGS := -lm
EXE 	:= sde_methods
CFILES	:= sde_methods.cc myrandom.cc simulation.cc empirical.cc
OBJECTS := sde_methods.o myrandom.o simulation.o empirical.o

all: ${EXE}

# $@ = PROGS (name of target)

${EXE}: $(OBJECTS)
	$(CC) $(CFLAGS) -o $(EXE) $(OBJECTS) $(LDFLAGS)

myrandom.o: myrandom.cc
	$(CC) $(CFLAGS) -c myrandom.cc

simulation.o: simulation.cc
	$(CC) $(CFLAGS) -c simulation.cc


empirical.o: empirical.cc
	$(CC) $(CFLAGS) -c empirical.cc


sde_methods.o: sde_methods.cc
	$(CC) $(CFLAGS) -c sde_methods.cc 


.PHONY: clean
clean:
	rm -f $(EXE) $(OBJECTS) *.txt
