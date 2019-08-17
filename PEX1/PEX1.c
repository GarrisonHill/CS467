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
//    char START_STREAM[150] = "START_STREAM\nBilly Joel - We Didn't Start the Fire.mp3";
    char songInput[100] = "Billy Joel - We Didn't Start the Fire.mp3";
    char START_STREAM[150] = "START_STREAM\nBilly Joel - We Didn't Start the Fire.mp3";
    struct sockaddr_in servaddr;  //we don't bind to a socket to send UDP traffic, so we only need to configure server address
    printf("Please enter one of the following commands: \n");
    int input = 0;
    bool socketOpen = false;
    do {
        fflush(stdout);
        printf("'1' = List Songs\n");
        printf("'2' = Download Song\n");
        printf("'3' = Exit\n");
        //TODO UNCOMMENT THE SCANF TO RESTORE CHOICE TO CLIENT SIDE
//        scanf("%d", &input);
        /**
         * PROGRAM AUTO DEFAULTS TO CHOOSING BILLY JOEL'S SONG AND OPENING A BRAND NEW SOCKET
         * CHANGES TO THIS INVOLVE THE ABOVE TODO
         *
         */
        input = 2;
        //Program opens socket and receives communication from server for songs stored on the server
        if (input == 1) {

            // Creating socket file descriptor
            if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
                perror("socket creation failed");
                exit(EXIT_FAILURE);
            }
//            socketOpen = true;

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

        if (input == 2 && socketOpen) {

            char *songInput;
            printf("Please input the name of the song to be streamed: ");
            scanf("%s", songInput);
            printf("%s\n", songInput);

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
            if ((n = recvfrom(sockfd, (char *) 2, MAXLINE, 0, (struct sockaddr *) &servaddr, &len)) < 0) {
                perror("ERROR");
                printf("Errno: %d. ", errno);
                exit(EXIT_FAILURE);
            }
            buffer[n] = '\0'; //terminate message
            printf("Server : %s\n", buffer);
        }

        /**
         * This section is for when no open socket already exists and the user wishes to auto enter a song
         */
        if (input == 2 && !socketOpen) {
            // Creating socket file descriptor
            if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
                perror("socket creation failed");
                exit(EXIT_FAILURE);
            }
            socketOpen = true;

            //Tells the client side which IP version it shoudl run off of
            servaddr.sin_family = AF_INET; //IPv4
            servaddr.sin_port = htons(
                    PORT); // port, converted to network byte order (prevents little/big endian confusion between hosts)
            servaddr.sin_addr.s_addr = INADDR_ANY; //localhost

            int n, len = sizeof(servaddr);


            printf("Please input the name of the song to be streamed: ");
//            //This clears the input buffer from the users choice of what to do above
//            getchar();
//            //creates the songInput variable
//            char songInput[150];
//            //gets user inptu from stdin and stores it in songinput
//            fgets(songInput, sizeof(songInput), stdin);
//            //removes the null terminator from the string which messes with the transmission to the server with the song name
//            int i = 0;
//            while (songInput[i] != '\0') {
//                i++;
//            }
//            songInput[i - 1] = '\0';

            printf("Fetching Song.\n");
//            //Concatenates the START_STREAM\n command with the chosen song name
//            strcat(START_STREAM, songInput);
//            printf("%s\n", START_STREAM);
            printf("Sending Start Response\n");
            printf("Waiting For Response.\n");
            //makes a file pointer called fptr
            FILE *fptr;
            char fileName[100];
            char path[100] = "./";
            strcat(path, songInput);
            strcpy(fileName, songInput);
            //opens or creates a file for reading or writing
            fptr = fopen(fileName, "a+");
            /*tells the server that we want to connect on socket sockfd, gives it the message to be sent and the length of
             * that message, sets any error flags to none.
             */
            sendto(sockfd, (const char *) START_STREAM, strlen(START_STREAM), 0, (const struct sockaddr *) &servaddr,
                   sizeof(servaddr));

            // Receive message from client
            while (1) {
                memset(buffer, '\0', MAXLINE);
                //recvfrom returns the number of characters in the message from the server
                n = recvfrom(sockfd, (char *) buffer, MAXLINE, 0, (struct sockaddr *) &servaddr, &len);
                //If there is nothing left to read from the server break
                if (n == -1) {
                    break;
                }
                //truncate the string so that
                char* buffRemoved = buffer + 12;
                puts(buffRemoved);
                //print each character in the buffer, printing strings could cause errors?
                for (int i = 0; i < n; ++i) {
                    fprintf(fptr, "%c", buffRemoved[i]);
                }
//                printf("%s", buffRemoved);

            }
//                for (int i = 0; i < n; ++i) {
//                    buffer[i] = 0;
//                }
            fclose(fptr);
        }


} while (input != 3);
printf("Exiting program and closing the socket");
close(sockfd);
return 0;
}