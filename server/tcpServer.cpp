#include "common.h"
#include <bits/stdc++.h>
#include <pthread.h>


#define SERVER_BACKLOG 1 // maxm connections allowable in the server  waiting  queue.

using namespace std;

void error_and_kill(const char *fmt, ...);

void * handle_connection(void *connfd) ;

char *bin2hex(const unsigned char *input, size_t len);

//compilation : g++ -o ./server/tcpServer ./server/tcpServer.cpp
               //g++ -pthread -o ./server/tcpServer ./server/tcpServer.cpp (when there are threads using in the programme)
               

uint8_t buff[MAX_LENGHT + 1];
uint8_t recvline[MAX_LENGHT + 1];

int main(int identity, char **argv)
{

    int listenfd, connfd, n , client_socket;
    struct sockaddr_in server_address;

    int opt = 1;


    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        error_and_kill("Error while creating socket");
    }

    // Forcefully attaching socket to the port 8080 although the port is already in use


    if (setsockopt(listenfd, SOL_SOCKET,
                   SO_REUSEADDR | SO_REUSEPORT, &opt,
                   sizeof(opt))) {
        error_and_kill("setsocket opt error");
    }

    bzero(&server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY); // i.e we are responding to anything
    server_address.sin_port = htons(SERVER_PORT);

    // tell operating system that this socket will run on this port
    if (bind(listenfd, (struct sockaddr *)&server_address,
             sizeof(server_address)) < 0)
    {
        error_and_kill("bind error");
    }

    if (listen(listenfd, SERVER_BACKLOG) < 0)
    {
        error_and_kill("listening error");
    }

    for (;;)
    {
        struct sockaddr_in client_addr;
        socklen_t client_addr_len;
        char client_address[MAX_LENGHT+1] ; 

        
        // accept block untill the new connection arives
        // it returns the "file descriptor to the connection"

        //   cout<<"waiting for conection on PORT"<<SERVER_PORT<<endl;
        printf("waiting for conection on PORT : %d\n", SERVER_PORT);

        fflush(stdout);
        client_socket = accept(listenfd, (SA *) &client_addr, &client_addr_len);
        
        inet_ntop(AF_INET , &client_addr , client_address ,MAX_LENGHT) ; 
        //inet_ntop function takes the address into network (bytes) format and convert it into the
        //presentation format.

        printf("\nclient connection : %s\n" , client_address) ;

        // handle_connection(client_socket ) ; 

       // creating the threads
       pthread_t t ; 

       int *pclient = new int() ; 
       *pclient = client_socket ; 
       pthread_create(&t , NULL , handle_connection , pclient) ;
    }

    shutdown(listenfd, SHUT_RDWR);
}

void error_and_kill(const char *ch, ...)
{
    int error_save;
    va_list ap;

    // libary call to save the error no , now we need to save it now
    error_save = errno;

    va_start(ap, ch);
    vfprintf(stdout, ch, ap);
    fprintf(stdout, "\n");
    fflush(stdout);

    if (error_save != 0)
    {
        fprintf(stdout, "  ( error = %d ) : %s\n", error_save,
                strerror(error_save));

        fprintf(stdout, "\n");
        fflush(stdout);
    }

    va_end(ap);

    exit(1);
}


void * handle_connection(void  *ptr_connfd){
        // zero out the recive buffer to make it NULL terminated
        
        int connfd = *((int*)(ptr_connfd)) ; 
        // conv(ptr_connfd , connfd) ; 

        //We are casting it before deleting it.
        delete[] (int*)ptr_connfd;

        memset(recvline, 0, MAX_LENGHT);

        // now read the client's message

        int n = read(connfd, recvline, MAX_LENGHT - 1) ;
        // while ((n = read(connfd, recvline, MAX_LENGHT - 1)) > 0)
        // {
        //     fprintf(stdout, "\n%s\n\n%s", bin2hex(recvline, n), recvline);

        //     // detect end of message
        //     if (recvline[n - 1] == '\n')
        //     {
        //         break;
        //     }
        //     memset(recvline, 0, MAX_LENGHT);
        // }


        if (n < 0){
            error_and_kill("read error");
            return NULL ; 
         }

        printf("below is from client--------------\n\n");
        printf("%s\n", recvline);
        // sending the response now
        snprintf((char *)buff, sizeof(buff), "HTTP/1.0 200 OK \r\n\r\nHello there,Client!");
        
        //testing the multi - threadness of the server
        sleep(5) ; 

        write(connfd, (char *)buff, strlen((char *)buff));
        close(connfd);
        printf("\nclosing connection!\n");
        return NULL ;
}