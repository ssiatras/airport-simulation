# Airport Simulation
Simulation of an airport.

## Compilation
`$ make clean`  
`$ make`

## Usage
Command line arguments are optional.  
`$ ./airport_simulation <Current Time> <Number of Terminals>`

## Design Decisions
There are three classes: **Application**, **Flight** and **Airport**.
-  **Application**: Represents a person's Application.

-  **Flight**: Represents a Flight. The bookings of a Flight are represented as a vector of pointers to Applications. This choice was made in order to get easily the read-only access to the bookings. Otherwise, an array would have been preferred, since the size is fixed.

- **Airport**: Represents an Airport. The terminals of an Airport are represented as an array of pointers to Flights. This choice was made, because the size of the array is fixed, so a vector was not needed. On the other hand, waiting list is represented as a list of pointers to Applications. This choice was made, because the ability to insert and erase elements easily is necessary.
