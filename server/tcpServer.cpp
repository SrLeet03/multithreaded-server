
// compilation : g++ -o ./server/tcpServer ./server/tcpServer.cpp
// g++ -pthread -o ./server/tcpServer ./server/tcpServer.cpp (when there are threads using in the programme)

#include "common.h"
#include <bits/stdc++.h>
#include <pthread.h>

#define SERVER_BACKLOG 1 // maxm connections allowable in the server  waiting  queue.
#define THREAD_POOL_SIZE 20

pthread_t thread_pool[THREAD_POOL_SIZE];
pthread_mutex_t mutex_lock;
pthread_cond_t thread_condition_var = PTHREAD_COND_INITIALIZER;

using namespace std;

void error_and_kill(const char *fmt, ...);

struct ClientRequest
{
    string method;
    string filePath;
    string protocol;
    vector<string> data;
};

string get_route_function();
string get_http_method();
ClientRequest parseRequestLine(string line);
ClientRequest parseRequestMessage(string message);
string buildResponseMessage(string status_code, string status_response, string filePathData = "");
string handleGetRequest(ClientRequest request);
string handlePostRequest(ClientRequest request);
void writeDataToFile(vector<string> data, string filePath);

void send_fav_icon_as_response(int socket);

void *handle_connection(int *connfd);
void *thread_function(void *arg);
queue<int *> client_connections;

char *bin2hex(const unsigned char *input, size_t len);

uint8_t buff[MAX_LENGHT + 1];
uint8_t recvline[MAX_LENGHT + 1];

int main(int identity, char **argv)
{

    int listenfd, connfd, n, client_socket;
    struct sockaddr_in server_address;

    int opt = 1;

    // creating the multiple threads to handle the connections

    for (int i = 0; i < THREAD_POOL_SIZE; i++)
    {
        if (pthread_create(&thread_pool[i], NULL, &thread_function, NULL) != 0)
        {
            error_and_kill("falied to create thread:(");
        }
    }
    // for(int i=0 ; i<THREAD_POOL_SIZE ; i++){
    //     if(pthread_create(&thread_pool[i] , NULL , &thread_function , NULL ) != 0){
    //         error_and_kill("falied to create thread:(");
    //     }
    // }

    if (pthread_mutex_init(&mutex_lock, NULL) != 0)
    {
        error_and_kill("mutex init has failed");
    }
    if (pthread_cond_init(&thread_condition_var, NULL) != 0)
    {
        error_and_kill("condition variable init has failed");
    }

    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        error_and_kill("Error while creating socket");
    }

    // Forcefully attaching socket to the port 8080 although the port is already in use

    if (setsockopt(listenfd, SOL_SOCKET,
                   SO_REUSEADDR | SO_REUSEPORT, &opt,
                   sizeof(opt)))
    {
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
        char client_address[MAX_LENGHT + 1];

        // accept block untill the new connection arives
        // it returns the "file descriptor to the connection"

        //   cout<<"waiting for conection on PORT"<<SERVER_PORT<<endl;
        printf("server is runnning on PORT : %d\n", SERVER_PORT);

        fflush(stdout);
        client_socket = accept(listenfd, (SA *)&client_addr, &client_addr_len);

        inet_ntop(AF_INET, &client_addr, client_address, MAX_LENGHT);
        // inet_ntop function takes the address into network (bytes) format and convert it into the
        // presentation format.

        printf("\nclient connection : %s\n", client_address);
        int *ptr = &client_socket;

        //    handle_connection(ptr ) ;

        // creating the threads
        //    pthread_t t ;

        pthread_mutex_lock(&mutex_lock);
        int *pclient = new int();

        *pclient = client_socket;
        client_connections.push(pclient);

        // pthread_cond_signal(&thread_condition_var) ; // this will give signal that we have a task to exucute so dont wait

        pthread_mutex_unlock(&mutex_lock);

        //    pthread_create(&t , NULL , handle_connection , pclient) ;
    }

    shutdown(listenfd, SHUT_RDWR);

    pthread_mutex_destroy(&mutex_lock);
    pthread_cond_destroy(&thread_condition_var);

    return 0;
}

// helper function to handle errors and exit the programme.
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

void *handle_connection(int *ptr_connfd)
{
    // zero out the recive buffer to make it NULL terminated

    int connfd = *(ptr_connfd);
    // conv(ptr_connfd , connfd) ;

    // We are casting it before deleting it.
    //  delete[] (int*)ptr_connfd;
    printf("in handle connection connfd %d\n", connfd);
    memset(recvline, 0, MAX_LENGHT);

    // now read the client's message

    int n = read(connfd, recvline, MAX_LENGHT - 1);
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
    {
        error_and_kill("read error");
        return NULL;
    }

    printf("below is from client--------------\n\n");
    printf("%s\n", recvline);
    // sending the response now

    snprintf((char *)buff, sizeof(buff),
            "HTTP/1.0 200 OK \r\n\r\n<h1>Hello there,Client!</h1>"
            "<title>Hi</title>"
            "<link rel = \"icon\" type = \"image/x-icon\" href = \"/root/intern/garbage/multithreaded_server/static/favicon.ico\"></link>"
            "<p style=\"color:red\">To read your file from server enter the correct file name as route.</p>"
            "<p style=\"color:red\">Eg.localhost:18000/myfile.txt/");


    // testing the multi - threadness of the server
    string route = get_route_function();

    if (route == "favicon.ico")
    {
        send_fav_icon_as_response(connfd);
        return NULL;
    }
    string method = get_http_method();
    string parsed;
    for (int i = 0; i < MAX_LENGHT; i++)
        parsed += recvline[i];
    ClientRequest request = parseRequestMessage(parsed);

    string response;
    if (request.method == "GET")

    {
        if (route.size())
            route.pop_back();
        request.filePath = route;
        response = handleGetRequest(request);
    }
    else if (request.method == "POST")
    {
        route = route.substr(1, (int)route.size() - 2);
        request.filePath = route;

        response = handlePostRequest(request);
    }

    // sleep(5) ;

    write(connfd, (char *)buff, strlen((char *)buff));
    // send(connfd, response.c_str(), response.size(), 0);
    close(connfd);
    printf("\nclosing connection!\n");
    return NULL;
}

void *thread_function(void *arg)
{

    // following will burn the cpu power caz although there is no any new connection
    // for the thread still ask if there any continuosl. To handle this problem we have to introduce condition variables.

    while (true)
    {

        int *front_client;
        bool found = false;

        pthread_mutex_lock(&mutex_lock);

        //  pthread_cond_wait(&thread_condition_var , &mutex_lock) ;
        // here we passed the lock since since condition varables and lock work closely. asa thread wait
        // it will releases the lock

        if (client_connections.size() > 0)
        {
            front_client = client_connections.front();
            client_connections.pop();
            found = true;
        }

        pthread_mutex_unlock(&mutex_lock);

        if (found)
        {
            printf("front_client :  %d\n", *front_client);
            handle_connection(front_client);
        }
    }
}

bool isFileExists(string filePath)
{

    ifstream f(filePath);
    bool status = f.good();
    if (f.is_open())
    {
        status = true;
    }

    f.close();
    return status;
}

string get_route_function()
{
    int i = 5;
    string s = "";
    while (i < MAX_LENGHT)
    {
        if (recvline[i] == ' ')
            break;

        s += recvline[i];
        i++;
    }
    // cout<<"SS"<<" "<<s<<endl;
    return s;
}

string get_http_method()
{
    int i = 0;
    string s = "";
    while (i < MAX_LENGHT)
    {
        if (recvline[i] == ' ')
            break;

        s += recvline[i];
        i++;
    }
    cout << "SS"
         << " " << s << endl;
    return s;
}

string handleGetRequest(ClientRequest request)
{
    string response;
    //  cout << "file Path: " << request.filePath << endl;
    string filePath = SERVER_FILE_ROOT + request.filePath;
    if (isFileExists(filePath))
    {
        response = buildResponseMessage("200", "OK", filePath);
    }
    else
    {
        response = buildResponseMessage("404", "Not Found", filePath);
    }
    return response;
}

string handlePostRequest(ClientRequest request)
{
    string response;
    writeDataToFile(request.data, SERVER_FILE_ROOT + request.filePath);
    response = buildResponseMessage("200", "OK", SERVER_FILE_ROOT + request.filePath);
    return response;
}
void writeDataToFile(vector<string> data, string filePath)
{
    ofstream file(filePath);

    // Write to the file
    for (auto line : data)
    {
        file << line;
        file << "\n";
    }
    // Close the file
    file.close();
}

string buildResponseMessage(string status_code, string status_response, string filePathData)
{
    string response = "HTTP/1.1 " + status_code + ' ' + status_response + "\n\n";
    if (filePathData != "")
    {
        string line;

        ifstream dataFile(filePathData);

        while (getline(dataFile, line))
        {
            response += (line + "\n");
        }
        dataFile.close();
    }
    cout << "Response message: \n";
    cout << "----------------------------" << endl;
    cout << response;
    return response;
}

ClientRequest parseRequestLine(string line)
{
    // cout << "Request Line: " << line << endl;
    istringstream R(line);
    string part;
    vector<string> requestParts;
    while (getline(R, part, ' '))
    {
        requestParts.push_back(part);
        // cout << part << endl;
    }
    ClientRequest newRequest;
    if (requestParts.size() > 2)
    {
        newRequest.method = requestParts[0];
        newRequest.filePath = requestParts[1];
        newRequest.protocol = requestParts[2];
    }
    return newRequest;
}

ClientRequest parseRequestMessage(string message)
{
    istringstream f(message);
    string line;
    getline(f, line);
    ClientRequest newRequest = parseRequestLine(line);
    bool headerFinished = false;
    while (getline(f, line))
    {
        if (headerFinished)
        {
            newRequest.data.push_back(line);
        }
        if (line.empty() || line.size() < 2)
        {
            headerFinished = true;
        }
    }
    return newRequest;
}

void send_fav_icon_as_response(int socket)
{
    FILE *picture;
    int size, read_size, stat, packet_index;
    char send_buffer[10240], read_buffer[256];
    packet_index = 1;

    picture = fopen("./static/favicon.ico", "r");
    printf("Getting Picture Size\n");

    if (picture == NULL)
    {
        printf("Error Opening Image File");
    }

    fseek(picture, 0, SEEK_END);
    size = ftell(picture);
    fseek(picture, 0, SEEK_SET);
    printf("Total Picture size: %i\n", size);

    // Send Picture Size
    printf("Sending Picture Size\n");
    write(socket, (void *)&size, sizeof(int));

    // Send Picture as Byte Array
    printf("Sending Picture as Byte Array\n");

    //    do { //Read while we get errors that are due to signals.
    //       stat=read(socket, &read_buffer , 255);
    //       printf("Bytes read: %i\n",stat);
    //    } while (stat < 0);

    printf("Received data in socket\n");
    printf("Socket data: %s\n", read_buffer);

    while (!feof(picture))
    {
        // while(packet_index = 1){
        // Read from the file into our send buffer
        read_size = fread(send_buffer, 1, sizeof(send_buffer) - 1, picture);

        // Send data through our socket
        do
        {
            stat = write(socket, send_buffer, read_size);
        } while (stat < 0);

        printf("Packet Number: %i\n", packet_index);
        printf("Packet Size Sent: %i\n", read_size);
        printf(" \n");
        printf(" \n");

        packet_index++;

        // Zero out our send buffer
        bzero(send_buffer, sizeof(send_buffer));
    }
}