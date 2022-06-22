//
// Created by os on 17.1.2022.
//

#include <cstdio>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <chrono>
#include <sys/time.h>
#include <fstream>

#define WRQ_OPCODE 2
#define DATA_OPCODE 3
#define ACK_OPCODE 4
#define ERROR_OPCODE 5
#define BUFFER_MAX_SIZE 1500
#define OCTET "octet"
#define MAX_PACKET_SIZE 1500
#define MAX_DATA_SIZE 512
#define MAX_DATA_PACKET_SIZE 516
#define UNKNOWN_USER_ERR_CODE 7
#define UNEXPECTED_PCKT_ERR_CODE 4
#define FILE_ALREADY_EXIST_ERR_CODE 6
#define BAD_BLOCK_NUM_ERR_CODE 0
#define ABANDONING_TRANS -1


using namespace std;

struct ACK {
    unsigned short int opcode;
    unsigned short int block_number;
} __attribute__((packed));


struct WRQ {
    unsigned short int opcode;
    char strings[MAX_PACKET_SIZE];
} __attribute__((packed));


struct DATA {
    unsigned short int opcode;
    unsigned short int block_number;
    char data[MAX_DATA_SIZE];

} __attribute__((packed));

struct ERROR {
    unsigned short int opcode;
    unsigned short int ErrorCode;
    char strings[MAX_PACKET_SIZE];
} __attribute__((packed));

struct PACKET {
    unsigned short int opcode;
    char strings[MAX_PACKET_SIZE];
} __attribute__((packed));



class Client {
public:
    Client(int socket, int num);

    struct sockaddr_in client_address;
    unsigned int client_address_len;

    int socket;

    int num;
    char *file_name;
    char *mode;
    int mode_size;
    short int block_counter;
    int file_d;

    struct timeval last_ack_time;

    int timeout_expired_counter;

    bool operator == (const Client& s) const;
    bool operator != (const Client& s) const;


};



























