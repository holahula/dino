CXX = g++
CXXFLAGS = -c -g -std=c++14 -Wall -MMD
SOURCES = main.cc models/view/view.cc models/view/tileView/tileView.cc models/enemy/enemy.cc models/enemy/basic/basic.cc models/enemy/invisible/invisible.cc models/enemy/regenerative/regenerative.cc models/map/map.cc models/map/iterator.cc models/observer/observer.cc models/shop/shop.cc models/state/state.cc models/subject/subject.cc models/tile/tile.cc models/tile/land/landTile.cc models/tile/path/pathTile.cc models/tower/tower.cc models/tower/damage/damage.cc models/tower/freeze/freeze.cc models/tower/money/money.cc exceptions/land-tile-occupied-exception/landTileOccupiedException.cc exceptions/no-tower-exception/noTowerException.cc
OBJECTS = ${SOURCES:.cc=.o}
EXEC = td

all: ${SOURCES} ${EXEC} 

${EXEC}: ${OBJECTS} 
	${CXX} ${OBJECTS} -o $@ `pkg-config gtkmm-3.0 --libs`

.cc.o:
	${CXX} ${CXXFLAGS} $< -o $@ `pkg-config gtkmm-3.0 --cflags`

clean:
	rm -rf *.o ${EXEC}

