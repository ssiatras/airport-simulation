CC= g++
CFLAGS= -std=c++98

OBJECTS= airport_simulation.o airport.o flight.o application.o

airport_simulation: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o airport_simulation

airport_simulation.o: airport_simulation.cc
	$(CC) $(CFLAGS) -c airport_simulation.cc

airport.o: airport.cc
	$(CC) $(CFLAGS) -c airport.cc

flight.o: flight.cc
	$(CC) $(CFLAGS) -c flight.cc

application.o: application.cc
	$(CC) $(CFLAGS) -c application.cc


.PHONY: clean

clean:
	rm -f airport_simulation $(OBJECTS)