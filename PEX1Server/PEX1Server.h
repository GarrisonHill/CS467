/** PEX1Server.h
 * ===========================================================
 * Name: CS467, Fall 2019
 * Project: PEX1_Server
 * Purpose: Implementation of Server Server Music Streaming
 *          Communication
 * Of note: Dr. Coffman said to never include a name so
 * ===========================================================
 */

#ifndef PEX1_PEX1_H
#define PEX1_PEX1_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>

#define PORT 4240
#define MAXLINE 1024
/** -------------------------------------------------------------------
 * Read the values of a file into a integer array
 * @param FILENAME the name of the file from which the data will be read
 * @param numBytes the total number of bytes in the file
 * @return a integer array containing all the bytes of the file
 */
int* fileRead(char* FILENAME, int numbytes);
/** -------------------------------------------------------------------
 * Read the number of bytes in a file
 * @param FILENAME the name of the file from which the data will be read
 * @return the total number of bytes in the file or a -1 if the file can not be opened
 */
int numBytes(char* FILENAME);

#endif //PEX1_PEX1_H
