/*************************************************************************
Header File   : application.h
Author - Date : Efstathios Siatras - 18/12/2017
Purpose       : Header file of class Application
*************************************************************************/
#ifndef APPLICATION_H
#define APPLICATION_H

#include <string>
#include <ctime>

class Flight;

class Application {
	private:
		int id; // ID of the person
		std::string firstName; // First name of the person
		std::string lastName; // Last name of the person
		std::string destination; // Destination

		time_t arrivalAtAirport; // The person is able to arrive to the airport at arrivalAtAirport or later
		time_t arrivalAtDestination; // The person wants to arrive to the destination at arrivalAtDestination or earlier

		char seat; // A or B class

	public:
		Application(const int&, const std::string&, const std::string&, const std::string&, const time_t&, const time_t&, const char&); // Constructor

		~Application(); // Destructor

		int matches(const Flight&) const; // Checks if application matches with a specific flight 

		int is_luxury_class() const; // Checks if application is A class
		int get_id() const; // Gets id
		std::string get_name() const; // Gets full name
		time_t arrived_by() const; // Gets arrivalAtDestination
};

#endif