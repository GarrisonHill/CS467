//
// Created by David on 8/17/2019.
//

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
int fileRead(char* FILENAME, char* fileInfo);

#endif //PEX1_PEX1_H
