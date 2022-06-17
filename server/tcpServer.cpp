#include<common.h>


int main(int identity , char * argv){

    int       listenfd , connfd , n ; 
    struct sockaddr_in server_address ; 
    uint8_t buff[MAX_LENGHT+1] ; 
    uint8_t recvline[MAX_LENGHT+1] ; 

    if(( listenfd  = socket(AF_INET , SOCK_STREAM , 0)   ) < 0){
        error_and_kill("Error while creating socket");
    }

    bzero( &server_address , sizeof(server_address)  ) ; 
    server_address.sin_family = AF_INET ; 
    server_address.sin_addr.s_addr =  htonl(INADDR_ANY); // i.e we are responding to anything
    server_address.sin_port = htons(SERVER_PORT) ; 

    
}