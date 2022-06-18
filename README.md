# multithreaded-server

INTRODUCTION - This is a simple TCP client server A web server implemented in C++ on a UNIX based platform so that it can serve multiple incoming client requests concurrently with minimum CPU's usage and thread pool.

   SERVER FUNCTIONING
   
   1.  Using socket(), create TCP socket. 
   2.  Using bind(), bind the socket to server address.
   3.  Using listen(), put the server socket in a passive mode, where it waits for the client to approach the server to   make a connection.
   4.  Using accept(), connection is established between client and server, and they are ready to transfer data.
   5.  Then this connection is passed to request queue of the server
   6.  One of the thread from thread pool which is not working at the time will pick the front element of the    Req Queue and proceed the request.
   7. Handle's cpu usage problem with thread condition variable and CPU critical section memory sharing problem with mutex lock



##### change the permission bash script to make it exucutable
###### sudo chmod u+x multiClients.bash


###### Linux command to see the CPU's usage of the machine
######## https://www.ibm.com/docs/en/linux-on-systems?topic=tools-top



Server

curl -i -X POST localhost:18000/file-name.txt/ \
  -H "Content-Type: text/xml" \
  --data-binary "@/root/intern/garbage/multithreaded_server/static/cfiles/commands.txt"


Connects to the server over TCP/IP
Retrieves and validates user input
Displays results of each move in the console
Very small memory footprint because the game logic is handled server-side
