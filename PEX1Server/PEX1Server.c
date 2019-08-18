/** PEX1Server.c
 * ===========================================================
 * Name: CS468, Fall 2019
 * Project: PEX1
 * Purpose: Implementation of Server Server Music Streaming
 *          Communication
 * ===========================================================
 */

#include <w32api/rpcndr.h>
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
            char* FILENAME = "C:/Users/lionc/CLionProjects/CS467/cmake-build-debug/Billy Joel - We Didn't Start the Fire.mp3";
            int numbytes = numBytes(FILENAME);
            if(numbytes == -1){
                break;
            }
            char* fileInfo = (char*) calloc(numbytes, sizeof(char));
            fileRead(FILENAME, fileInfo, numbytes);
            printf("File Read\n");
        }
        if(strcmp(buffer, STARTSTREAMVEGA) == 0){

        }


    }
    close(socketfd); // Close socket
    return 0;
}

int fileRead(char* FILENAME, char* fileInfo, int numbytes){
    FILE *fptr;
    char *buffer;

    fptr = fopen(FILENAME, "r");
    if (fptr == NULL) {
        printf("Could not open file\n");
    }

    char c;
    do{
        char c;
    }

//    fread(buffer, sizeof(char), numbytes, fptr);
//    fclose(fptr);
//    for (int i = 0; i < numbytes; ++i) {
//        fileInfo[i] = buffer[i];
//        printf("%c", buffer[i]);
//    }
//    free(buffer);
    return 1;
}

int numBytes(char* FILENAME){
    FILE *fptr;
    fptr = fopen(FILENAME, "r");
    if (fptr == NULL) {
        printf("Could not open file\n");
        return -1;
    }
    fseek(fptr, 0L, SEEK_END);
    int numbytes = ftell(fptr);
    printf("%d", numbytes);
    fseek(fptr, 0L, SEEK_SET);
    return numbytes;
}

//TODO Document this if it works
//http://www.fundza.com/c4serious/fileIO_reading_all/