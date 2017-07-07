TARGET =printer
IDIR =include
ODIR =build
SRC_DIR =src
CXX=g++-4.9
CXXFLAGS=-std=c++11 -Wall -I$(IDIR)
LIBS=-pthread -lwiringPi -lrt
_DEPS = ConfigParser.h A4988.h Axis.h DataTypes.h Extruder.h Heater.h FileReader.h Interface.h stringParser.h timer.h Interpreter.h Planner.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = ConfigParser.o A4988.o Axis.o Extruder.o Heater.o FileReader.o Interface.o stringParser.o timer.o Interpreter.o Planner.o cnc.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: src/%.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

printer: $(OBJ)
	$(CXX) -o $@ $^ $(LIBS) $(CXXFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ exec
