CXX = g++
CXXFLAGS = -std=c++14 -Wall -g -MMD
EXEC = vm
OBJECTS = main.o window.o keylistener.o command.o colormanager.o store.o parser.o highlighter.o highlightergroup.o event.o saver.o
DEPENDS = ${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC} -lncurses

-include ${DEPENDS}

.PHONY: clean

clean:
	rm ${OBJECTS} ${EXEC} ${DEPENDS}
