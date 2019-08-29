/** PEX1Server.c
 * ===========================================================
 * Name: CS467, Fall 2019
 * Project: PEX1_Server
 * Purpose: Implementation of Server Server Music Streaming
 *          Communication
 * Of note: Dr. Coffman said to never include a name so
 * ===========================================================
 */

#include "PEX1Server.h"
//These are all the cases possible for the list request
#define LISTMESSAGEALLUPPER  "LIST_REQUEST"
#define LISTMESSAGEFIRSTUPPER  "LIST_request"
#define LISTMESSAGESECONDUPPER  "list_REQUEST"


int main() {
    printf("Waiting For Connection\n");
    //The following are static replys that can be sent by the server to the client
    char *STREAMDATA = "STREAM_DATA\n";
    char *LISTREPLY = "LIST_REPLY\nBilly Joel - We Didn't Start the Fire.mp3\nSuzanne Vega - Toms Diner.mp3\n\0";
    char *STARTSTREAMBILLY = "START_STREAM\nBilly Joel - We Didn't Start the Fire.mp3";
    char *STARTSTREAMVEGA = "START_STREAM\nSuzanne Vega - Toms Diner.mp3";
    //End of static replys

    int socketfd;  //Socket descriptor, like a file-handle
    struct sockaddr_in srvaddr, cliaddr; //Stores IP address, address family (ipv4), and port
    char buffer[MAXLINE]; //buffer to store message from client

    // Creating socket file descriptor
    // AF_INET = "domain" = IPv4
    // SOCK_DGRAM = "type" = UDP, unreliable
    // protocol=0, specifies UDP within SOCK_DGRAM type.
    if ((socketfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    //This is literal magic and im afraid to delete it
    srvaddr.sin_family = AF_INET;           // IPv4
    srvaddr.sin_addr.s_addr = INADDR_ANY;   // All available interfaces
    srvaddr.sin_port = htons(
            PORT);       // port, converted to network byte order (prevents little/big endian confusion between hosts)



    // Forcefully attaching socket to the port 8080
    // Bind expects a sockaddr, we created a sockaddr_in.  (struct sockaddr *) casts pointer type to sockaddr.
    if (bind(socketfd, (const struct sockaddr *) &srvaddr, sizeof(srvaddr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    //Loop FOREVER
    while (1) {
        // Receive message from client
        int len = sizeof(cliaddr), n; //must initialize len to size of buffer

        //If we recieve a size of bytes thats 0 or less then show a error and exit the socket
        if ((n = recvfrom(socketfd, (char *) buffer, MAXLINE, 0, (struct sockaddr *) &cliaddr, &len)) < 0) {
            perror("ERROR");
            printf("Errno: %d. ", errno);
            exit(EXIT_FAILURE);
        }
        buffer[n] = '\0';  //terminate message
        printf("Client :%s\n", buffer);

        //IF client requests list of songs that the server can send
        if (strcmp(buffer, LISTMESSAGEALLUPPER) == 0 || strcmp(buffer, LISTMESSAGEFIRSTUPPER) == 0 ||
            strcmp(buffer, LISTMESSAGESECONDUPPER) == 0) {
            //send to the client the list of songs
            sendto(socketfd, LISTREPLY, strlen(LISTREPLY), 0, (const struct sockaddr *) &cliaddr, len);
            //print out that the list of songs that was sent
            printf("Reply Sent\n%s", LISTREPLY);
        }

            //If the user requests the song by Billy Joel this executes
        else if (strcmp(buffer, STARTSTREAMBILLY) == 0) {
            //Change this to a relative path
            char *FILENAME = "C:/Users/lionc/CLionProjects/CS467/cmake-build-debug/Billy Joel - We Didn't Start the Fire.mp3";
            //get the number of bytes in the file
            int numbytes = numBytes(FILENAME);
            //if numbytes has not returned a -1 then the file at least opened so the program should continue
            if (numbytes != -1) {
                //this array provides lookups for the samplerate of a mp3 file
                int samplerate_lookup[] = {44100, 48000, 32000, -1};
                //this array provides lookups for the bitrate of a mp3 file
                int bitrate_lookup[] = {0, 32, 40, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320, -1};
                //create a buffer pointer to the read in file from fileRead()
                int *fileBuffer = fileRead(FILENAME, numbytes);

                //initalize the index for the buffer to zero
                int counter = 0;
                int frameNumber = 1;
                /* This will send loop through the buffer until it finds a 0xff followed by a 0xfb if it finds one
                 * it will calculate the bitrate, samplerate, and padding bits and generate the frame size accordingly
                 * if that sequence is not detected it will increment the byte counter and compare the next two
                 */
                while (counter != numbytes) {
                    //If a character is 0xff then check to see if the next is 0xfb
                    if (fileBuffer[counter] == 0xff) {
                        counter++;
                        if (fileBuffer[counter] == 0xfb) {
                            //if 0xfb calculate the frame to
                            counter++;
                            //Find the first 4 bits in the byte LSR 4 then & it with 1111 to isolate the first 4
                            int bitrate = (fileBuffer[counter] >> 4) & 15;
                            //Find the 5th and 6th bits in the byte LSR 2 then & it with 11 to isolate the 5th and 6th bits
                            int samplerate = (fileBuffer[counter] >> 2) & 3;
                            //Find the 7th bit in the byte LSR 1 then & it with 1 to isolate the 7th bit
                            int padding = (fileBuffer[counter] >> 1) & 1;

                            //initalize the frameSize so it doesn't freak out
                            int frameSize = 0;
                            //check the padding formula, mult the bitrate by 1000 to get frame size in bytes
                            if (padding == 0) {
                                frameSize = (144 * bitrate_lookup[bitrate] * 1000) / samplerate_lookup[samplerate];
                            } else {
                                //this bothers me that this is just slightly to long
                                frameSize = (1 +
                                             (144 * bitrate_lookup[bitrate] * 1000) / samplerate_lookup[samplerate]);
                            }

                            //create a temp buffer to append the stream header and the data into
                            //this is frameSize + 12 to accommodate the size of the stream header
                            char *tempBuffer = calloc(frameSize + 12, sizeof(char));
                            //copy the stream header into the tempBuffer
                            strcpy(tempBuffer, STREAMDATA);
                            //Decrease the counter by two to include the first two bytes of the stream 0xff and 0xfb
                            counter = counter - 2;
                            /*loop through each byte of the buffer and insert them at 12+ so not to overwrite the
                             * stream header, go until you are at frame size + 12 to get all the data in the buffer
                             */
                            for (int i = 12; i < frameSize + 12; i++) {
                                tempBuffer[i] = fileBuffer[counter];
                                counter++;
                            }

                            //print the frame to console
                            printf("Sending frame %d size of %d\n", frameNumber, frameSize);
                            //Actually send all this data in, plus 12 to accommodate for stream header
                            sendto(socketfd, tempBuffer, frameSize + 12, 0, (const struct sockaddr *) &cliaddr, len);
                            //increment the frame number
                            frameNumber++;
                            //sleep for a hot second so we don't overwhelm the poor socket
                            usleep(1);

                        } else {
                            //rewind the counter to see if the next one is 0xfb
                            counter++;

                        }
                    } else {
                        //advance the counter cause its neither
                        counter++;
                    }
                }
                //A STREAM_DONE message is sent by the server when the stream is complete.
                char *STREAMDONE = "STREAM_DONE\n";
                sendto(socketfd, STREAMDONE, 13, 0, (const struct sockaddr *) &cliaddr, len);
                printf("File Read\n");
            }

        } else if (strcmp(buffer, STARTSTREAMVEGA) == 0) {
            //Change this to a relative path
            char *FILENAME = "C:/Users/lionc/CLionProjects/CS467/cmake-build-debug/Suzanne Vega - Toms Diner.mp3";
            //get the number of bytes in the file
            int numbytes = numBytes(FILENAME);
            //if numbytes has not returned a -1 then the file at least opened so the program should continue
            if (numbytes != -1) {
                //this array provides lookups for the samplerate of a mp3 file
                int samplerate_lookup[] = {44100, 48000, 32000, -1};
                //this array provides lookups for the bitrate of a mp3 file
                int bitrate_lookup[] = {0, 32, 40, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320, -1};
                //create a buffer pointer to the read in file from fileRead()
                int *fileBuffer = fileRead(FILENAME, numbytes);

                //initalize the index for the buffer to zero
                int counter = 0;
                int frameNumber = 1;
                /* This will send loop through the buffer until it finds a 0xff followed by a 0xfb if it finds one
                 * it will calculate the bitrate, samplerate, and padding bits and generate the frame size accordingly
                 * if that sequence is not detected it will increment the byte counter and compare the next two
                 */
                while (counter != numbytes) {
                    //If a character is 0xff then check to see if the next is 0xfb
                    if (fileBuffer[counter] == 0xff) {
                        counter++;
                        if (fileBuffer[counter] == 0xfb) {
                            //if 0xfb calculate the frame to
                            counter++;
                            //Find the first 4 bits in the byte LSR 4 then & it with 1111 to isolate the first 4
                            int bitrate = (fileBuffer[counter] >> 4) & 15;
                            //Find the 5th and 6th bits in the byte LSR 2 then & it with 11 to isolate the 5th and 6th bits
                            int samplerate = (fileBuffer[counter] >> 2) & 3;
                            //Find the 7th bit in the byte LSR 1 then & it with 1 to isolate the 7th bit
                            int padding = (fileBuffer[counter] >> 1) & 1;

                            //initalize the frameSize so it doesn't freak out
                            int frameSize = 0;
                            //check the padding formula, mult the bitrate by 1000 to get frame size in bytes
                            if (padding == 0) {
                                frameSize = (144 * bitrate_lookup[bitrate] * 1000) / samplerate_lookup[samplerate];
                            } else {
                                //this bothers me that this is just slightly to long
                                frameSize = (1 +
                                             (144 * bitrate_lookup[bitrate] * 1000) / samplerate_lookup[samplerate]);
                            }

                            //create a temp buffer to append the stream header and the data into
                            //this is frameSize + 12 to accommodate the size of the stream header
                            char *tempBuffer = calloc(frameSize + 12, sizeof(char));
                            //copy the stream header into the tempBuffer
                            strcpy(tempBuffer, STREAMDATA);
                            //Decrease the counter by two to include the first two bytes of the stream 0xff and 0xfb
                            counter = counter - 2;
                            /*loop through each byte of the buffer and insert them at 12+ so not to overwrite the
                             * stream header, go until you are at frame size + 12 to get all the data in the buffer
                             */
                            for (int i = 12; i < frameSize + 12; i++) {
                                tempBuffer[i] = fileBuffer[counter];
                                counter++;
                            }

                            //print the frame to console
                            printf("Sending frame %d size of %d\n", frameNumber, frameSize);
                            //Actually send all this data in, plus 12 to accommodate for stream header
                            sendto(socketfd, tempBuffer, frameSize + 12, 0, (const struct sockaddr *) &cliaddr, len);
                            //increment the frame number
                            frameNumber++;
                            //sleep for a hot second so we don't overwhelm the poor socket
                            usleep(1);

                        } else {
                            //rewind the counter to see if the next one is 0xfb
                            counter++;

                        }
                    } else {
                        //advance the counter cause its neither
                        counter++;
                    }
                }
                //A STREAM_DONE message is sent by the server when the stream is complete.
                char *STREAMDONE = "STREAM_DONE\n";
                sendto(socketfd, STREAMDONE, 13, 0, (const struct sockaddr *) &cliaddr, len);
                printf("File Read\n");
            }
        }
            //This handles if the user doesn't request a song list or a song that doesn't exist or is misspelled
        else {
            /**
             * A COMMAND_ERROR is sent by the server to the client when the client sends an
                invalid message. It may be sent when the client sends an unrecognized message,
                or attempts to stream an MP3 that does not exist.
             */
            char *COMMANDERROR = "COMMAND_ERROR\n";
            sendto(socketfd, COMMANDERROR, 15, 0, (const struct sockaddr *) &cliaddr, len);
            printf("The Server has recieved an unexpected command.\nSending Error Message.\n");
        }
        printf("Waiting For Command.\n");
    }
    //This should never be reached cause the server should never close except for catastrophic errors
    close(socketfd); // Close socket
    return 0;
}

/** -------------------------------------------------------------------
 * Read the values of a file into a integer array
 * @param FILENAME the name of the file from which the data will be read
 * @param numBytes the total number of bytes in the file
 * @return a integer array containing all the bytes of the file
 */
int *fileRead(char *FILENAME, int numbytes) {
    FILE *fptr;
    //create a buffer with everything cleared out the size of a int with numBytes space allocated
    int *buffer = calloc(numbytes, sizeof(int));

    //open the file only to read from
    fptr = fopen(FILENAME, "r");
    //if the file pointer is nothing return -1 to be handled above
    if (fptr == NULL) {
        printf("Could not open file\n");
        return -1;
    }

    //Loop from the start of the file till the end
    for (int i = 0; i < numbytes; ++i) {
        //read into buffer[i] the size of 1 byte one at a time from the file
        fread(&buffer[i], 1, 1, fptr);
    }

    //close the file
    fclose(fptr);
    return buffer;
}

/** -------------------------------------------------------------------
 * Read the number of bytes in a file
 * @param FILENAME the name of the file from which the data will be read
 * @return the total number of bytes in the file or a -1 if the file can not be opened
 */
int numBytes(char *FILENAME) {
    FILE *fptr;
    //open the file only to read from
    fptr = fopen(FILENAME, "r");
    //if the file pointer is nothing return -1 to be handled above
    if (fptr == NULL) {
        printf("Could not open file\n");
        return -1;
    }
    //seek to the end of the file
    fseek(fptr, 0L, SEEK_END);
    //take the total number of bytes passed over
    int numbytes = ftell(fptr);
    //Go back to the beginning of the file
    fseek(fptr, 0L, SEEK_SET);
    //close the file
    fclose(fptr);
    return numbytes;
}

//TODO Document this if it works
//http://www.fundza.com/c4serious/fileIO_reading_all/