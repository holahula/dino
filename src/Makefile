CXX = g++-5
CXXFLAGS = -g -std=c++14 -Wall -MMD
#CXXFLAGS = -std=c++14 -Wall -MMD
EXEC = td
OBJECTS = main.o enemy.o basic.o invisible.o regenerative.o map.o observer.o shop.o state.o subject.o tile.o landTile.o pathTile.o tower.o damage.o freeze.o money.o
DEPENDS = ${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC}

-include ${DEPENDS}

.PHONY: clean

clean:
	rm ${OBJECTS} ${EXEC} ${DEPENDS}
