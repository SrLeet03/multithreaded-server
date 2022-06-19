# multithreaded-server

Description - This is a simple TCP client server A web server implemented in C++ on a UNIX based platform so that it can serve multiple incoming client requests concurrently with minimum CPU's usage and thread pool. Fow now it is just simple server which can post your text file on server and with get request can get you your uploaded file.

---

   **_SERVER FUNCTIONING_**
   
   ``` 1.  Using socket(), create TCP socket.```<br/>
   ```2.  Using bind(), bind the socket to server address.```<br/>
    ``` 3.  Using listen(), put the server socket in a passive mode, where it waits for the client to approach the server to   make a connection.```<br/>
    ``` 4.  Using accept(), connection is established between client and server, and they are ready to transfer data.```<br/>
    ``` 5.  Then this connection is passed to request queue of the server```<br/>
    ``` 6.  One of the thread from thread pool which is not working at the time will pick the front element of the    Req Queue and proceed the request.```<br/>
    ``` 7. Handle's cpu usage problem with thread condition variable and CPU critical section memory sharing problem with mutex lock```<br/>

---


   **_HOW TO RUN the project _**
   
   ``` For establishing a connection between client and server, first compile server.cpp in one terminal and run the programme. Now to access the server either use curl from your terminal or run the bash script of root folder```<br/>

   ``` 1. Compile server :g++ -pthread -o ./server/tcpServer ./server/tcpServer.cpp ```<br/>
   ``` 2. Run server : ./server/tcpServer ```<br/>
   ``` 3. For client  : you can make get request from your web browser itself. eg. localhost:18000/{filename.txt}/ ```<br/>
   ``` And post request: eg. curl -i -X POST localhost:18000/{file_name_to_upload_on_server}.txt/ \ -H "Content-Type: text/xml" \ --data-binary "@{file_path_from_your_machine}" ```<br/>
   ``` 4. Or you can run the shell script also. Before running it make it exucatable.  i.e                         sudo chmod u+x multiClients.bash```<br/>

---

   **_TECH STACK USED - C++ , Socket Programming and Computer Networks and multi-threading using:**
   

   ``` <sys/types.h>-//Defines a collection of typedef symbols and structures```<br>                           ```<sys/socket.h>---//Declarations of socket constants, types, and   functions```<br>                                  
   ```<netinet/in.h>---//Definitions for the internet protocol family   ```<br>                                          
   ```<arpa/inet.h>----//Definitions for internet operations    ```<br>                                                  
   ```<netdb.h>--------//Definitions for network database  operations```<br>                                             
   ```<unistd.h>-------//Definitions for standard symbolic constants and types ```<br>                                  
  ``` <pthread.h> -----//With help of this we have implemented the threads and it's functionality ```<br/>

---


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
