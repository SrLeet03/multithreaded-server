#include "common.h"
#include <bits/stdc++.h>

using namespace std;

void error_and_kill(const char *fmt, ...);

char *bin2hex(const unsigned char *input, size_t len);

int main(int identity, char **argv)
{

    int listenfd, connfd, n;
    struct sockaddr_in server_address;
    uint8_t buff[MAX_LENGHT + 1];
    uint8_t recvline[MAX_LENGHT + 1];

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

    if (listen(listenfd, 10) < 0)
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
        connfd = accept(listenfd, (SA *) &client_addr, &client_addr_len);
        
        inet_ntop(AF_INET , &client_addr , client_address ,MAX_LENGHT) ; 
        printf("\n\nclient connection : %s\n" , client_address) ;
        //inet_ntop function takes the address into network (bytes) format and convert it into the
        //presentation format.


        // zero out the recive buffer to make it NULL terminated

        memset(recvline, 0, MAX_LENGHT);

        // now read the client's message

        n = read(connfd, recvline, MAX_LENGHT - 1) ;
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


        if (n < 0)
            error_and_kill("read error");
        printf("below is from client--------------\n\n");
        printf("%s\n", recvline);
        // sending the response now
        snprintf((char *)buff, sizeof(buff), "HTTP/1.0 200 OK \r\n\r\nHello there,Client!");

        write(connfd, (char *)buff, strlen((char *)buff));
        close(connfd);
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

void handle_connection(int client_socket){
    char buffer[MAX_LENGHT+1]  ; 
    
}