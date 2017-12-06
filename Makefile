#Makefile

CC= /opt/ohpc/pub/mpi/openmpi-gnu/1.10.6/bin/mpic++
OBJECTS = objects
SOURCES = sources
BIN = bin
INCLUDES = -I headers
CFLAGS = -g

all: $(BIN)/xy_model

$(BIN)/xy_model: $(OBJECTS)/main.o $(OBJECTS)/Thread.o $(OBJECTS)/ran1.o
	${CC} ${CFLAGS}  $^ -o $@

$(OBJECTS)/main.o: $(SOURCES)/main.cpp
	${CC} -c ${CFLAGS} ${INCLUDES} $^ -o $@

$(OBJECTS)/Thread.o: $(SOURCES)/Thread.cpp
	${CC} -c ${CFLAGS} ${INCLUDES} $^ -o $@

$(OBJECTS)/ran1.o: $(SOURCES)/ran1.cpp
	${CC} -c ${CFLAGS} ${INCLUDES} $^ -o $@

clean: 
	rm $(OBJECTS)/*.o

