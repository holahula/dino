CXX = g++
CXXFLAGS = -g -std=c++14 -Wall -MMD
#CXXFLAGS = -std=c++14 -Wall -MMD
EXEC = td
OBJECTS = main.o models/enemy/enemy.o models/enemy/basic/basic.o models/enemy/invisible/invisible.o models/enemy/regenerative/regenerative.o models/map/map.o models/observer/observer.o models/shop/shop.o models/state/state.o models/subject/subject.o models/tile/tile.o models/tile/land/landTile.o models/tile/path/pathTile.o models/tower/tower.o models/tower/damage/damage.o models/tower/freeze/freeze.o models/tower/money/money.o exceptions/land-tile-occupied-exception/landTileOccupiedException.o exceptions/no-tower-exception/noTowerException.o
DEPENDS = ${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC}

-include ${DEPENDS}

.PHONY: clean

clean:
	rm ${OBJECTS} ${EXEC} ${DEPENDS}