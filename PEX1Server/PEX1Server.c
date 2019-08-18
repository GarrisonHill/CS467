/** PEX1Server.c
 * ===========================================================
 * Name: CS468, Fall 2019
 * Project: PEX1
 * Purpose: Implementation of Server Server Music Streaming
 *          Communication
 * ===========================================================
 */

#include "PEX1Server.h"
#define LISTMESSAGE  "LIST_REQUEST"
int main(){
    printf("Waiting For Connection\n");
    char *LISTREPLY = "LIST_REPLY\nBilly Joel - We Didn't Start the Fire.mp3\nSuzanne Vega - Toms Diner.mp3\n\0";
    char *STARTSTREAMBILLY = "START_STREAM\nBilly Joel - We Didn't Start the Fire.mp3";
    char *STARTSTREAMVEGA = "START_STREAM\nSuzanne Vega - Toms Diner.mp3";

    int socketfd;  //Socket descriptor, like a file-handle
    struct sockaddr_in srvaddr, cliaddr; //Stores IP address, address family (ipv4), and port
    char buffer[MAXLINE]; //buffer to store message from client
    char *hello = "Hello from server"; //message to respond to client

    // Creating socket file descriptor
    // AF_INET = "domain" = IPv4
    // SOCK_DGRAM = "type" = UDP, unreliable
    // protocol=0, specifies UDP within SOCK_DGRAM type.
    if ((socketfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    srvaddr.sin_family = AF_INET;           // IPv4
    srvaddr.sin_addr.s_addr = INADDR_ANY;   // All available interfaces
    srvaddr.sin_port = htons( PORT );       // port, converted to network byte order (prevents little/big endian confusion between hosts)



    // Forcefully attaching socket to the port 8080
    // Bind expects a sockaddr, we created a sockaddr_in.  (struct sockaddr *) casts pointer type to sockaddr.
    if (bind(socketfd, (const struct sockaddr *)&srvaddr, sizeof(srvaddr))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    while(1){
        // Receive message from client
        int len=sizeof(cliaddr), n; //must initialize len to size of buffer
        if((n = recvfrom(socketfd, (char *)buffer, MAXLINE, 0, ( struct sockaddr *) &cliaddr, &len))<0)
        {
            perror("ERROR");
            printf("Errno: %d. ",errno);
            exit(EXIT_FAILURE);
        }
        buffer[n] = '\0';  //terminate message
        printf("Client :%s\n", buffer);
        if(strcmp(buffer,LISTMESSAGE) == 0){
            sendto(socketfd, LISTREPLY, strlen(LISTREPLY), 0, (const struct sockaddr *) &cliaddr, len);
            printf("Reply Sent\n%s", LISTREPLY);
        }

        if(strcmp(buffer, STARTSTREAMBILLY) == 0){
            char* fileInfo = malloc(sizeof(fileInfo));
            fileRead("./Billy Joel - We Didn't Start the Fire.mp3", fileInfo);
            printf("%s", fileInfo);
        }
        if(strcmp(buffer, STARTSTREAMVEGA) == 0){

        }

//        // Respond to client
//        sendto(socketfd, (const char *)hello, strlen(hello), 0, (const struct sockaddr *) &cliaddr, len);
//        printf("Hello message sent.\n");

    }
    close(socketfd); // Close socket
    return 0;
}

int fileRead(char* FILENAME, char* fileInfo){
    int BUFFER_SIZE = 512;
    FILE *fptr;

    fptr = fopen(FILENAME, "r");
    if (fptr == NULL) {
        printf("Could not open file\n");
    }
    char buffer[BUFFER_SIZE];
    while (fscanf(fptr, "%s", buffer) == 1)
    {
        strcpy(fileInfo, buffer);
        if (feof(fptr))
        {
            break;
        }
    }




}