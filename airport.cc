/*************************************************************************
Implementation File : airport.cc
Author - Date       : Efstathios Siatras - 18/12/2017
Purpose             : Implementation file of class Airport
*************************************************************************/
#include <iostream>
#include <iomanip>

#include "airport.h"
#include "flight.h"
#include "application.h"

using namespace std;

Airport::Airport(const time_t& cur, const int& numberOfTerm) 
: current(cur), numberOfTerminals(numberOfTerm), terminals(NULL), waitingList(), failedApplications(0) {
	terminals = new Flight*[numberOfTerminals];

	for (int i = 0; i < numberOfTerminals; i++) { // Fills array with NULL
		
		terminals[i] = NULL;
	}

	cout << "An airport with capacity for " << numberOfTerminals << " flights was created" << endl;
}

Airport::~Airport() {
	cout << "An airport was destroyed" << endl;
	for (int i = 0; i < numberOfTerminals; i++) { // Frees flights in array
		if(terminals[i] != NULL) {

			delete terminals[i];
			terminals[i] = NULL;
		}
	}

	delete[] terminals; // Frees array
	terminals = NULL;

	if (!waitingList.empty()) {
		list<Application *>::iterator it = waitingList.begin();

		while (it != waitingList.end()) { // Frees applications in list
			delete *it;
			*it = NULL;

			it = waitingList.erase(it);			
		}
	}
}

time_t Airport::get_time() const {

	return current;
}

int Airport::add_application(const int &id, const string &firstName, const string &lastName, const string &destination, const time_t &arrivalAtAirport, const time_t &arrivalAtDestination, const char &seat) {
	Application *newApp = new Application(id, firstName, lastName, destination, arrivalAtAirport, arrivalAtDestination, seat);

	for (int i = 0; i < numberOfTerminals; i++) { // Checks all flights
		if(terminals[i] != NULL) {
			if (terminals[i]->add_passenger(*newApp)) { // Application was serviced and it got placed in a flight

				return 1;
			}
		}
	}

	waitingList.push_back(newApp); // Application was not serviced and it got placed in the waiting list

	return 0;
}

int Airport::cancel_applications(const int &id) {
	int found = 0; // Number of cancelled applications and reservations

	for (int i = 0; i < numberOfTerminals; i++) { // Checks all flights
		if(terminals[i] != NULL) {

			found += terminals[i]->cancel_reservations(id); // Cancels reservations in flights
		}
	}

	if (!waitingList.empty()) { 
		list<Application *>::iterator it = waitingList.begin();

		while (it != waitingList.end()) { // Checks all applications in waiting list
			if (id == (*it)->get_id()) { // Checks if id matches
				delete *it; // Cancels applications in waiting list
				*it = NULL;

				it = waitingList.erase(it);

				found++;
			}

			else {

				it++;
			}
		}
	}

	failedApplications += found; // Adds cancelled applications and reservations to total failed applications

	return found;
}

int Airport::add_flight(const string &destination, const time_t &departure, const time_t &duration, const int &maxCapacityA, const int &maxCapacityB) {
	for (int i = 0; i < numberOfTerminals; i++) {
		if(terminals[i] == NULL) { // Finds an empty terminal
			terminals[i] = new Flight(destination, departure, duration, maxCapacityA, maxCapacityB);
			// Adds flight to the terminal
			if (!waitingList.empty()) { 
				list<Application *>::iterator it = waitingList.begin();

				while (it != waitingList.end()) { // Checks all applications waiting list
					if (terminals[i]->add_passenger(**it)) { // Finds applications that match with the flight and adds them to the flight
						it = waitingList.erase(it); // Removes them from the waiting list
					}

					else {

						it++;
					}
				}
			}

			return i;
		}
	}

	return -1; // No empty terminal found
}

int Airport::cancel_flight(const int &indexOfTerm) {
	if(indexOfTerm >= numberOfTerminals) { // Trying to cancel a flight that does not exist

		return 0;
	}

	if (terminals[indexOfTerm] != NULL) {
		const vector<Application *> &temp = terminals[indexOfTerm]->get_bookings(); // Gets read-only access to bookings of the flight
		
		int maxCapacityA;
		int maxCapacityB;
		int maxCapacity = terminals[indexOfTerm]->get_capacity(maxCapacityA, maxCapacityB); // Maximum Capacity of the flight

		int addedToAnotherFlight = 0; // Flag if application was added to another flight
		for (int i = 0; i < maxCapacity; i++) { // Checks all bookings in cancelled flight
			if (temp[i] != NULL) {
				Application *newApp = new Application(*temp[i]); // Creates a copy of the booked application by default copy constructor

				for (int j = 0; j < indexOfTerm; j++) { // Checks if application matches to a flight in terminal from 0 till (indexOfTerm - 1)
					if(terminals[j] != NULL) {
						if(terminals[j]->add_passenger(*newApp)) {
							addedToAnotherFlight = 1; // Added to another flight

							break;
						}
					}
				}

				if (addedToAnotherFlight == 1) { // Already found a matching flight
					
					addedToAnotherFlight = 0; // Initializes flag to 0 for the next booked application
				}

				else { // Checks if application matches to a flight in terminal from (indexOfTerm + 1) till (numberOfTerminals - 1)
					for (int j = (indexOfTerm + 1); j < numberOfTerminals; j++) {
						if(terminals[j] != NULL) {
							if(terminals[j]->add_passenger(*newApp)) {
								addedToAnotherFlight = 1; // Added to another flight

								break;
							}
						}
					}

					if (addedToAnotherFlight == 0) { // Did not find any matching flight

						waitingList.push_front(newApp); // Adds to the waiting list
					}

					addedToAnotherFlight = 0; // Initializes flag to 0 for the next booked application
				}
			}
		}
		delete terminals[indexOfTerm]; // Frees flight
		terminals[indexOfTerm] = NULL;

		return 1;
	}

	else {  // Trying to cancel a flight that does not exist

		return 0;
	}
}

int Airport::failed_applications() const {

	return failedApplications;
}

void Airport::show_timetable() const {
	cout << endl;
	cout << " _______________________________________________________________________________________" << endl;
	cout << "|                                                                                       |" << endl;
	cout << "|-------------------------------------| TIMETABLE |-------------------------------------|" << endl;
	cout << "|_______________________________________________________________________________________|" << endl;

	int toPrint = 0; // Number of flights to be printed

	for (int i = 0; i < numberOfTerminals; i++) { // Calculates toPrint
		if (terminals[i] != NULL) {
			toPrint++;
		}
	}

	if (toPrint == 0) { // No flights to print
		cout << "|                                                                                       |" << endl;
		cout << "|                                  * Empty timetable *                                  |" << endl;
		cout << "|_______________________________________________________________________________________|" << endl << endl << endl;
	}

	else {
		cout << "|                                           |           |         |           |         |" << endl;
		cout << "|                DESTINATION                | DEPARTURE | ARRIVAL | AVAILABLE | MAXIMUM |" << endl;
		cout << "|                                           |   TIME    |  TIME   |   SEATS   |  SEATS  |" << endl;
		cout << "|___________________________________________|___________|_________|___________|_________|" << endl;
		cout << "|                                           |           |         |           |         |" << endl;
		Flight **sorted = new Flight*[toPrint]; // A temporary array of filled terminals (no NULL terminals) in ascending order by departure time

		int j = 0;
		for (int i = 0; i < numberOfTerminals; i++) { // Fills the copy
			if (terminals[i] != NULL) {

				sorted[j] = terminals[i];
				j++;
			}
		}

		Flight *temp; // Temporary pointer to swap
		for (int i = 0; i < (toPrint-1); i++) { // Bubble sort algorithm to sort by departure time
			for (j = 0; j < (toPrint - i - 1); j++) {
				if(sorted[j]->departs_at() > sorted[j+1]->departs_at()) {
					temp = sorted[j];
					sorted[j] = sorted[j+1];
					sorted[j+1] = temp;
				}
			}
		}

		int aivA, aivB, maxA, maxB;
		for (int i = 0; i < toPrint; i++) {
			cout << "| " << left << setw(41) << sorted[i]->get_destination();
			cout << " | " << left << setw(9) << sorted[i]->departs_at();
			cout << " | " << left << setw(7) << sorted[i]->arrives_at();
			cout << " | " << left << setw(9) << sorted[i]->get_available(aivA, aivB);
			cout << " | " << left << setw(7) << sorted[i]->get_capacity(maxA, maxB) << " |" << endl;

			if (i != (toPrint-1)) {
				cout << "|-------------------------------------------|-----------|---------|-----------|---------|" << endl;
			}
		}
		cout << "|___________________________________________|___________|_________|___________|_________|" << endl << endl << endl;

		delete[] sorted; // Deletes temporary array
		sorted = NULL;
	}
}

void Airport::show_people_waiting() const {
	cout << endl;
	cout << " ________________________________" << endl;
	cout << "|                                |" << endl;
	cout << "|--------| WAITING LIST |--------|" << endl;
	cout << "|________________________________|" << endl;

	if (waitingList.empty()) { // No applications in waiting list to print
		cout << "|                                |" << endl;
		cout << "|     * Empty waiting list *     |" << endl;
	}

	else {
		for (list<Application *>::const_iterator it = waitingList.begin(); it != waitingList.end(); it++) {
			cout << "|                                |" << endl;
			cout << "| " << left << setw(30) << (*it)->get_name() << " |" << endl;
		}
	}

	cout << "|________________________________|" << endl << endl << endl;
}

void Airport::flow_time(const time_t &dt) {
	cout << "Current time is " << current << endl; // Current time before flowing
	current += dt;

	for (int i = 0; i < numberOfTerminals; i++) { // Checks all flights
		if(terminals[i] != NULL) {
			if (terminals[i]->departs_at() <= current) { // Checks if flight has departed during dt
				cout << "Flight in terminal " << i+1 << " departing to " << terminals[i]->get_destination() << endl;

				delete terminals[i]; // Frees flight
				terminals[i] = NULL;
			}
		}	
	}

	if (!waitingList.empty()) {
		list<Application *>::iterator it = waitingList.begin();

		while (it != waitingList.end()) { // Checks all applications in waiting list
			if ((*it)->arrived_by() <= current) { // Checks if application's desirable arrival time has passed current time
				delete *it; // Frees application
				*it = NULL;

				it = waitingList.erase(it); // Removes it from the waiting list
				failedApplications++; // Adds it to failed applications
			}

			else {

				it++;
			}
		}
	}

	cout << "Current time is " << current << endl; // Current time after flowing
}

int Airport::get_numberOfTerminals() const {

	return numberOfTerminals;
}