/*************************************************************************
Header File   : airport.h
Author - Date : Efstathios Siatras - 18/12/2017
Purpose       : Header file of class Airport
*************************************************************************/
#ifndef AIRPORT_H
#define AIRPORT_H

#include <string>
#include <ctime>
#include <list>

class Flight;
class Application;

class Airport {
	private:
		time_t current; // Current time of the airport

		int numberOfTerminals; // Number of terminals
		Flight **terminals; // Terminals of the airport ; Array of pointers to Flights

		std::list<Application *> waitingList; // Waiting list of the airport ; List of pointers to Applications

		int failedApplications; // Number of failed and cancelled applications

	public:
		Airport(const time_t& = 0, const int& = 25); // Constructor

		~Airport(); // Destructor

		time_t get_time() const; // Gets current time

		int add_application(const int &, const std::string &, const std::string &, const std::string &, const time_t &, const time_t &, const char &); // Adds application
		int cancel_applications(const int &); // Cancels all the applications and reservations of a person, using the ID

		int add_flight(const std::string &, const time_t &, const time_t &, const int &, const int &); // Adds flight
		int cancel_flight(const int &); // Cancels flight

		int failed_applications() const; // Gets failed applications

		void show_timetable() const; // Shows timetable in ascending order by departure time
		void show_people_waiting() const; // Shows people waiting in waiting list (all applications)

		void flow_time(const time_t &); // Flows time by specific amount of time

		int get_numberOfTerminals() const; // Gets number of terminals
};

#endif