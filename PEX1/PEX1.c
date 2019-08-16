/** PEX1.c
 * ===========================================================
 * Name: CS468, Fall 2019
 * Project: PEX1
 * Purpose: Implementation of Client Server Music Streaming
 *          Communication
 * ===========================================================
 */

#include "PEX1.h"

int main() {
    int sockfd; //Socket descriptor, like a file-handle
    char buffer[MAXLINE]; //buffer to store message from server
    char *LIST_REQUEST = "LIST_REQUEST"; //message to send to server
    char *START_STREAM = "START_STREAM\n";
    struct sockaddr_in servaddr;  //we don't bind to a socket to send UDP traffic, so we only need to configure server address
    printf("Please enter one of the following commands: \n");
    int input = 0;
    bool socketOpen = false;
    do {
        printf("'1' = List Songs\n");
        printf("'2' = Download Song\n");
        printf("'3' = Exit\n");

        scanf("%d", &input);
        //Program opens socket and receives communication from server for songs stored on the server
        if (input == 1) {

            // Creating socket file descriptor
            if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
                perror("socket creation failed");
                exit(EXIT_FAILURE);
            }
            socketOpen = true;

            // Filling server information
            servaddr.sin_family = AF_INET; //IPv4
            servaddr.sin_port = htons(
                    PORT); // port, converted to network byte order (prevents little/big endian confusion between hosts)
            servaddr.sin_addr.s_addr = INADDR_ANY; //localhost

            int n, len = sizeof(servaddr);
            //Sending message to server
            sendto(sockfd, (const char *) LIST_REQUEST, strlen(LIST_REQUEST), 0, (const struct sockaddr *) &servaddr,
                   sizeof(servaddr));
            printf("Requesting Song List.\n");

            // Receive message from client
            if ((n = recvfrom(sockfd, (char *) buffer, MAXLINE, 0, (struct sockaddr *) &servaddr, &len)) < 0) {
                perror("ERROR");
                printf("Errno: %d. ", errno);
                exit(EXIT_FAILURE);
            }
            buffer[n] = '\0'; //terminate message
            printf("Server : %s\n", buffer);
        }
        //TODO Stream a song

        if(input == 2 && socketOpen){

            char* songInput;
            printf("Please input the name of the song to be streamed: ");
            scanf("%s", songInput);
            printf("%s", songInput);

            int n, len = sizeof(servaddr);
            //Sending message to server
            //Server needs to receive the START_STEAM command followed by the name of the song to be streamed
            printf("%s", START_STREAM);
            sendto(sockfd, (const char *) START_STREAM, strlen(START_STREAM), 0, (const struct sockaddr *) &servaddr,
                   sizeof(servaddr));
            printf("Fetching Song.\n");

            sendto(sockfd, (const char *) songInput, strlen(songInput), 0, (const struct sockaddr *) &servaddr,
                   sizeof(servaddr));

            // Receive message from client
            if ((n = recvfrom(sockfd, (char *) buffer, MAXLINE, 0, (struct sockaddr *) &servaddr, &len)) < 0) {
                perror("ERROR");
                printf("Errno: %d. ", errno);
                exit(EXIT_FAILURE);
            }
            buffer[n] = '\0'; //terminate message
            printf("Server : %s\n", buffer);
        }


        if(input == 2 && !socketOpen){
            // Creating socket file descriptor
            if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
                perror("socket creation failed");
                exit(EXIT_FAILURE);
            }
            socketOpen = true;

            // Filling server information
            servaddr.sin_family = AF_INET; //IPv4
            servaddr.sin_port = htons(
                    PORT); // port, converted to network byte order (prevents little/big endian confusion between hosts)
            servaddr.sin_addr.s_addr = INADDR_ANY; //localhost

            int n, len = sizeof(servaddr);
            //Sending message to server
            //Server needs to receive the START_STEAM command followed by the name of the song to be streamed
            printf("%s", START_STREAM);
            sendto(sockfd, (const char *) START_STREAM, strlen(START_STREAM), 0, (const struct sockaddr *) &servaddr,
                   sizeof(servaddr));
            printf("Fetching Song.\n");
            char* songInput;
            printf("Please input the name of the song to be streamed: ");
            scanf("%s", songInput);
            printf("%s", songInput);
            sendto(sockfd, (const char *) songInput, strlen(songInput), 0, (const struct sockaddr *) &servaddr,
                   sizeof(servaddr));

            // Receive message from client
            if ((n = recvfrom(sockfd, (char *) buffer, MAXLINE, 0, (struct sockaddr *) &servaddr, &len)) < 0) {
                perror("ERROR");
                printf("Errno: %d. ", errno);
                exit(EXIT_FAILURE);
            }
            buffer[n] = '\0'; //terminate message
            printf("Server : %s\n", buffer);
        }


    } while (input != 3);
    printf("Exiting program and closing the socket");
    close(sockfd);
    return 0;
}