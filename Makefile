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

valgrind:
	G_SLICE=always-malloc G_DEBUG=gc-friendly valgrind --tool=memcheck --leak-check=full --leak-resolution=high --suppressions=gtk.suppression --num-callers=20 --log-file=vgdump ./${EXEC}

# valgrind --tool=memcheck --leak-check=full --leak-resolution=high --track-origins=yes --suppressions=gtk.suppression --num-callers=20 --log-file=vgdump ./${EXEC}