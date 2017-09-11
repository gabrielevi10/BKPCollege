CPP      = g++
CC       = gcc
OBJ      = main.o Jogodavida.o Cells.o
LINKOBJ  = main.o Jogodavida.o Cells.o
BIN      = jogodavida
CXXFLAGS = $(CXXINCS) -ggdb3 -std=c++11
CFLAGS   = $(INCS) -ggdb3 -std=c11
RM       = rm -f

.PHONY: all all-before all-after clean clean-custom

all: all-before $(BIN) all-after

clean: clean-custom
	${RM} $(OBJ) $(BIN)

$(BIN): $(OBJ)
	$(CPP) $(LINKOBJ) -o $(BIN) $(LIBS)

main.o: main.cpp
	$(CPP) -c main.cpp -o main.o $(CXXFLAGS)

Jogodavida.o: Jogodavida.cpp
	$(CPP) -c Jogodavida.cpp -o Jogodavida.o $(CXXFLAGS)

Cells.o: Cells.cpp
	$(CPP) -c Cells.cpp -o Cells.o $(CXXFLAGS)