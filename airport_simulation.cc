/*************************************************************************
Implementation File : airport_simulation.cc
Author - Date       : Efstathios Siatras - 18/12/2017
Purpose             : Simulates an airport
*************************************************************************/
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>

#include "airport.h"

using namespace std;

Airport *create_airport(int&, char**&); // Creates airport based on command line parameters
int destroy_airport(Airport*&); // Destroys airport_simulation

int insert_flights(ifstream&, Airport&); // Inserts flights from file
int insert_applications(ifstream&, Airport&); // Inserts applications from file

int main(int argc, char** argv) {
	Airport *a = NULL;
	a = create_airport(argc, argv);
	if (a == NULL) { // Checks create_airport() failure
		cout << "Bad input from command line parameters. Exiting..." << endl;

		return 1;
	}

	ifstream flights;
	flights.open("./input/FlightsLinuxANSI.txt");
	if (!flights) { // Checks open() failure
		cout << "Cannot open file of flights. Exiting..." << endl;

		return 2;
	}

	if (!insert_flights(flights, *a)) { // Checks insert_flights() failure
		cout << "Bad input from file of flights. Exiting..." << endl;

		return 3;
	}
	flights.close();

	ifstream applications;
	applications.open("./input/ApplicationsLinuxANSI.txt");
	if (!applications) { // Checks open() failure
		cout << "Cannot open file of applications. Exiting..." << endl;

		return 2;
	}

	if (!insert_applications(applications, *a)) {  // Checks insert_applications() failure
		cout << "Bad input from file of applications. Exiting..." << endl;

		return 3;
	}
	applications.close();

	a->show_timetable();
	a->show_people_waiting();

	a->flow_time(5);

	a->show_timetable();
	a->show_people_waiting();

	if(!destroy_airport(a)) { // Checks destroy_airport() failure
		cout << "There was no airport to destroy. Exiting..." << endl;

		return 4;
	}
}

Airport *create_airport(int& argc, char**& argv) {
	Airport *a = NULL;

	if (argc == 1) { // No parameters
		
		a = new Airport;
	}

	else if (argc == 2) { // One parameter
		
		a = new Airport(atoi(argv[1]));
	}

	else if (argc == 3) { // Two parameters
		
		a = new Airport(atoi(argv[1]), atoi(argv[2]));
	}

	else { // exec() called or too many parameters

		return NULL;
	}

	return a;
}

int destroy_airport(Airport*& a) {
	if (a == NULL) { // Checks if there is no airport to destroy

		return 0;
	}

	delete a;
	a = NULL;

	return 1;
}

int insert_flights(ifstream& in, Airport& a) {
	if(!in) {

		return 0;
	}

	string line;
	int check = 0; // Return value of add_flight() ; check = -1 if terminals are filled

	while ((getline(in, line)) && (check != -1)) { // Gets a line of file as string
		istringstream iline(line); // An input stream to operate on string

		string destination;
		if (!getline(iline, destination, ';')) { // Gets destination from istringstream

			return 0;
		}

		string departure;
		if (!getline(iline, departure, ';')) { // Gets departure from istringstream

			return 0;
		}

		string duration;
		if (!getline(iline, duration, ';')) { // Gets duration from istringstream

			return 0;
		}

		string maxCapacityA;
		if (!getline(iline, maxCapacityA, ';')) { // Gets maxCapacityA from istringstream

			return 0;
		}

		string maxCapacityB;
		if (!getline(iline, maxCapacityB)) { // Gets maxCapacityB from istringstream

			return 0;
		}
		if (atoi(departure.c_str()) >= a.get_time()) { // Ignores flights that the current time has passed the departure time of the flight
			check = a.add_flight(destination, atoi(departure.c_str()), atoi(duration.c_str()), atoi(maxCapacityA.c_str()), atoi(maxCapacityB.c_str()));
			// Adds flight
		}
	}

	return 1;
}

int insert_applications(ifstream& in, Airport& a) {
	if(!in) {
		return 0;
	}

	string line;

	while ((getline(in, line))) {  // Gets a line of file as string
		istringstream iline(line); // An input stream to operate on string

		string id;
		if (!getline(iline, id, ';')) { // Gets id from istringstream

			return 0;
		}

		string firstName;
		if (!getline(iline, firstName, ';')) { // Gets firstName from istringstream

			return 0;
		}

		string lastName;
		if (!getline(iline, lastName, ';')) { // Gets lastName from istringstream

			return 0;
		}

		string destination;
		if (!getline(iline, destination, ';')) { // Gets destination from istringstream

			return 0;
		}

		string arrivalAtAirport;
		if (!getline(iline, arrivalAtAirport, ';')) { // Gets arrivalAtAirport from istringstream

			return 0;
		}

		string arrivalAtDestination;
		if (!getline(iline, arrivalAtDestination, ';')) { // Gets arrivalAtDestination from istringstream

			return 0;
		}

		string seat;
		if (!getline(iline, seat)) { // Gets seat from istringstream

			return 0;
		}

		if (atoi(arrivalAtDestination.c_str()) >= a.get_time()) {// Ignores applications that the current time has passed the desirable arrival time of the application
			a.add_application(atoi(id.c_str()), firstName, lastName, destination, atoi(arrivalAtAirport.c_str()), atoi(arrivalAtDestination.c_str()), seat[0]);
			// Adds application
		}
	}

	return 1;
}
