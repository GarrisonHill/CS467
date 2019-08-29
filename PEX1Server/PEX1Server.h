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
int* fileRead(char* FILENAME, int numbytes);
int numBytes(char* FILENAME);

#endif //PEX1_PEX1_H
