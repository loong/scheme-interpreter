SRCS    = $(shell /bin/ls *.cc)
CFLAGS   = -DOP_ASSIGN
DEBUG = # -DL_DEBUG

.SUFFIXES: $(SUFFIXES) .cpp

%.o: %.cpp
#	g++ -c $(CFLAGS) $<
	g++ -c $(CFLAGS) -fno-elide-constructors $<

OBJS = main.o parse.o eval.o functions.o Cell.o FunctionManager.o DefinitionManager.o

main: $(OBJS)
	g++ -g $(CFLAGS) -o $@ $(OBJS) -lm

main.o: Cell.hpp cons.hpp parse.hpp eval.hpp main.cpp
	g++ -c -g main.cpp

parse.o: Cell.hpp cons.hpp parse.hpp parse.cpp
	g++ -c -g parse.cpp

eval.o: Cell.hpp cons.hpp eval.hpp eval.cpp
	g++ $(DEBUG) -c -g eval.cpp

function.o: Cell.hpp eval.hpp functions.hpp functions.cpp
	g++ -c -g functions.cpp

Cell.o: functions.hpp Cell.hpp Cell.cpp
	g++ -c -g Cell.cpp

FunctionManager.o: Cell.hpp FunctionManager.hpp FunctionManager.cpp
	g++ -c -g FunctionManager.cpp

DefinitionManager.o: Cell.hpp bstmap.hpp DefinitionManager.hpp DefinitionManager.cpp
	g++ -c -g DefinitionManager.cpp


doc:
	doxygen doxygen.config

showdoc:
	firefox html/index.html &

test:
	rm -f testoutput.txt
	./main testinput.dev.easy.txt > testoutput.txt
	diff testinput.dev.easy.ref.txt testoutput.txt

clean:
	rm -f core *~ $(OBJS) main main.exe testoutput.txt

cleanall:
	rm -f core *~ $(OBJS) main main.exe testoutput.txt
	rm -rf html/
