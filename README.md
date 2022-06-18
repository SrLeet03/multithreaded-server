# multithreaded-server

##### change the permission bash script to make it exucutable
###### sudo chmod u+x multiClients.bash


###### Linux command to see the CPU's usage of the machine
######## https://www.ibm.com/docs/en/linux-on-systems?topic=tools-top


This project consists of both client and server software to handle playing any number of Tic-Tac-Toe games simultaneously. This game was developed in C++ and utilized Linux sockets programming (TCP/IP) to provide for a networked gaming experience.

Server

Developed to handle multiple games at the same time
Kicks off a new thread for each client that connects
Linked list of games is used to keep track of which players are playing who
Mutexes provide inter-thread atomic data access
Object-oriented programming was utilized to keep track of tic-tac-toe boards, players, and games between players
Client

Connects to the server over TCP/IP
Retrieves and validates user input
Displays results of each move in the console
Very small memory footprint because the game logic is handled server-side
