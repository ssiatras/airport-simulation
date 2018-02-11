/*************************************************************************
Implementation File : flight.cc
Author - Date       : Efstathios Siatras - 18/12/2017
Purpose             : Implementation file of class Flight
*************************************************************************/
#include <iostream>

#include "flight.h"
#include "application.h"

using namespace std;

Flight::Flight(const string& dest, const time_t& depart, const time_t& durat, const int& maxA, const int& maxB)
: destination(dest), bookingsVector(), departure(depart), duration(durat), maxCapacityA(maxA), maxCapacityB(maxB), numberOfA(0), numberOfB(0) {
	int maxCapacity = maxCapacityA + maxCapacityB;

	for (int i = 0; i < maxCapacity; i++) { // Fills vector with empty bookings

		bookingsVector.push_back(NULL);
	}

	cout << "A flight to " << destination << " with capacity for " << maxCapacity << " passengers was created" << endl;
}

Flight::~Flight() {
	for (int i = 0; i < (maxCapacityA + maxCapacityB); i++) { // Frees reserved bookings in vector
		if(bookingsVector[i] != NULL) {

			delete bookingsVector[i];
			bookingsVector[i] = NULL;
		}
	}

	cout << "A flight was destroyed" << endl;
}

int Flight::add_passenger(Application& check) {
	if (check.matches(*this)) { // Checks if application matches
		if (check.is_luxury_class()) { // If seat == 'A'
			for (int i = 0; i < maxCapacityA; i++) { // Finds empty booking for A class
				if (bookingsVector[i] == NULL) { 
					bookingsVector[i] = &check; // Fills it with application

					numberOfA++;

					return 1;
				}
			}
		}

		else { // If seat == 'B'
			for (int i = maxCapacityA; i < (maxCapacityA + maxCapacityB); i++) { // Finds empty booking for B class
				if (bookingsVector[i] == NULL) {
					bookingsVector[i] = &check; // Fills it with application

					numberOfB++;

					return 1;
				}
			}
		}
	}

	return 0;
}

int Flight::cancel_reservations(const int &id) {
	int found = 0; // Number of cancelled reservations
	for (int i = 0; i < (maxCapacityA + maxCapacityB); i++) { // Checks all bookings
		if (bookingsVector[i] != NULL) {
			if (id == bookingsVector[i]->get_id()) { // If its the ID we are looking for
				if (bookingsVector[i]->is_luxury_class()) { // If seat == 'A'
					
					numberOfA--;
				}

				else { // If seat == 'B'
					
					numberOfB--;
				}

				delete bookingsVector[i]; // Destroys application of reservation
				bookingsVector[i] = NULL;

				found++;
			}
		}
	}

	return found;
}

time_t Flight::departs_at() const {

	return departure;
}

time_t Flight::arrives_at() const {

	return (departure + duration);
}

string Flight::get_destination() const {

	return destination;
}

int Flight::get_available(int &numA, int &numB) const {
	numA = maxCapacityA - numberOfA; // Number of available seats to book in A class
	numB = maxCapacityB - numberOfB; // Number of available seats to book in B class

	return (numA + numB);
}

int Flight::get_capacity(int &maxA, int &maxB) const {
	maxA = maxCapacityA; // Maximum number of seats in A class
	maxB = maxCapacityB; // Maximum number of seats in B class

	return (maxCapacityA + maxCapacityB);
}

const vector<Application *> &Flight::get_bookings() const {

	return bookingsVector;
}