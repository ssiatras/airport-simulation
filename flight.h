/*************************************************************************
Header File   : flight.h
Author - Date : Efstathios Siatras - 18/12/2017
Purpose       : Header file of class Flight
*************************************************************************/
#ifndef FLIGHT_H
#define FLIGHT_H

#include <string>
#include <vector>
#include <ctime>

class Application;

class Flight {
	private:
		std::string destination; // Destination of the flight
		std::vector<Application*> bookingsVector; // Bookings of the flight ; Vector of pointers to Applications

		time_t departure; // Departure time of the flight
		time_t duration; // Duration of the flight

		int maxCapacityA; // Maximum Capacity of A class
		int maxCapacityB; // Maximum Capacity of B class

		int numberOfA; // Current number of bookings in A class
		int numberOfB; // Current number of bookings in B class

	public:
		Flight(const std::string&, const time_t&, const time_t&, const int&, const int&); // Constructor

		~Flight(); // Destructor

		int add_passenger(Application&); // Adds application to the flight, if it matches
		int cancel_reservations(const int&); // Cancels all the reservations of a person in the flight, using the ID

		time_t departs_at() const; // Gets departure time
		time_t arrives_at() const; // Gets arrival time (departure + duration)

		std::string get_destination() const; // Gets destination

		int get_available(int&, int&) const; // Gets number of available seats to book
		int get_capacity(int&, int&) const; // Gets maximum capacity of bookings

		const std::vector<Application *>& get_bookings() const; // Gets read-only access to bookings
};

#endif