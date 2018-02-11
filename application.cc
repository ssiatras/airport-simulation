/*************************************************************************
Implementation File : application.cc
Author - Date       : Efstathios Siatras - 18/12/2017
Purpose             : Implementation file of class Application
*************************************************************************/
#include <iostream>

#include "application.h"
#include "flight.h"

using namespace std;

Application::Application(const int& idd, const string& first, const string& last, const string& dest, const time_t& arrAir, const time_t& arrDest, const char& seatt)
: id(idd), firstName(first), lastName(last), destination(dest), arrivalAtAirport(arrAir), arrivalAtDestination(arrDest), seat(seatt) {}

Application::~Application() {}

int Application::matches(const Flight& check) const {
	if (this->destination == check.get_destination()) { // Checks destination
		int numA, numB;

		if (check.get_available(numA, numB) != 0) { // Checks if there is any seat available
			if (this->arrivalAtAirport <= check.departs_at()) { // Checks if person will have arrived by the time flight departs
				if (this->arrivalAtDestination >= check.arrives_at()) { // Checks if flight will have arrived to destination by the time the person wants to
					if (this->is_luxury_class()) { // If seat == 'A'
						if (numA != 0) { // Checks availability of A class seats

							return 1;
						}
					}

					else { // If seat == 'B' 
						if (numB != 0) { // Checks availability of B class seats

							return 1;
						}
					}
				}
			}
		}
	}

	return 0;
}

int Application::is_luxury_class() const {
	if (seat == 'A') {
		
		return 1;
	}

	else { // seat == 'B'
		
		return 0;
	}
}

int Application::get_id() const {

	return id;
}

string Application::get_name() const {
	string fullName(firstName);
	fullName += ' ';
	fullName += lastName; // 'fullName' is 'firstName lastName'

	return fullName;
}

time_t Application::arrived_by() const {

	return arrivalAtDestination;
}