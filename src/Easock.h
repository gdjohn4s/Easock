#ifndef Easock_h
#define Easock_h

#pragma once
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>


int make_sock();
int send_msg(int sock, char* hostname, uint16_t port, char* msg);
void get_req(char *page, char *hostaddr, uint16_t port);

#endif