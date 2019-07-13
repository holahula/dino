CXX = g++
CXXFLAGS = -c -g -std=c++14 -Wall -MMD
SOURCES = ${shell find ${SRCDIR} -type f -name *.cc}
OBJECTS = ${SOURCES:.cc=.o}
EXEC = td

all: ${SOURCES} ${EXEC} 

${EXEC}: ${OBJECTS} 
	${CXX} ${OBJECTS} -o $@ `pkg-config gtkmm-3.0 --libs`

.cc.o:
	${CXX} ${CXXFLAGS} $< -o $@ `pkg-config gtkmm-3.0 --cflags`

clean:
	rm -rf *.o ${EXEC}