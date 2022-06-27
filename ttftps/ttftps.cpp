#include "ttftps.h"
#include <iostream>
#include <cstring>
#include <vector>
#include <list>

using namespace std;

bool Client::operator!=(const Client &s) const {
    return !( (client_address.sin_addr.s_addr == s.client_address.sin_addr.s_addr) && (client_address.sin_port == s.client_address.sin_port) );
}

bool Client::operator==(const Client &s) const {
    return ( (client_address.sin_addr.s_addr == s.client_address.sin_addr.s_addr) && (client_address.sin_port == s.client_address.sin_port) );
}

Client::Client(int socket, int num) {
    client_address_len = (socklen_t)sizeof(client_address);
    this->socket = socket;
    timeout_counter = 0;
    block_counter = 0;

    this->num = num;
    file_name = nullptr;
    mode = nullptr;
    mode_size = 0;
    file_d = 0;
}

void parsing_wrq(Client *client, struct WRQ *wrq){

    if(wrq == nullptr || client == nullptr) return;

    client->file_name = new char[(strlen(wrq->strings)) + 1];
    client->file_name[strlen(wrq->strings)] = '\0';
    strcpy(client->file_name, wrq->strings);

    client->mode_size = strlen(wrq->strings + strlen(client->file_name)) + 1;
    client->mode = new char[client->mode_size + 1];
    client->mode[client->mode_size] = '\0';
    strcpy(client->mode, wrq->strings + 1 + strlen(client->file_name));

}

void send_error_message(Client *client, int error_code){
    struct ERROR error_packet = {0};
    error_packet.opcode = htons(ERROR_OPCODE);

    switch (error_code) {

        case UNKNOWN_USER_ERR_CODE: { //OPCODE = 7 
            error_packet.ErrorCode = htons(UNKNOWN_USER_ERR_CODE);
            strcpy(error_packet.strings, "Unknown user");
            int size = 4 + strlen(error_packet.strings) + 1;
            if (sendto(client->socket, &error_packet, size, 0,
                       (struct sockaddr *)&client->client_address, client->client_address_len) != size) {
                perror("TTFTP_ERROR:");
                exit(1);
            }

            break;
        }

        case UNEXPECTED_PCKT_ERR_CODE: {
            error_packet.ErrorCode = htons(UNEXPECTED_PCKT_ERR_CODE);
            strcpy(error_packet.strings, "Unexpected packet");
            int size = 4 + strlen(error_packet.strings) + 1;
            if (sendto(client->socket, &error_packet, size, 0,
                       (struct sockaddr *)&client->client_address, client->client_address_len) != size) {
                perror("TTFTP_ERROR:");
                exit(1);
            }

            break;
        }

        case FILE_ALREADY_EXIST_ERR_CODE: { // OPCODE = 6 
            error_packet.ErrorCode = htons(FILE_ALREADY_EXIST_ERR_CODE);
            strcpy(error_packet.strings, "File already exists");
            int size = 4 + strlen(error_packet.strings) + 1;
            if (sendto(client->socket, &error_packet, size, 0,
                       (struct sockaddr *)&client->client_address, client->client_address_len) != size) {
                perror("TTFTP_ERROR:");
                exit(1);
            }

            break;
        }

        case BAD_BLOCK_NUM_ERR_CODE: { // OPCODE = 0
            error_packet.ErrorCode = htons(0);
            strcpy(error_packet.strings, "Bad block number");
            int size = 4 + strlen(error_packet.strings) + 1;
            if (sendto(client->socket, &error_packet, size, 0,
                       (struct sockaddr *)&client->client_address, client->client_address_len) != size) {
                perror("TTFTP_ERROR:");
                exit(1);
            }

            break;
        }

        case ABANDONING_TRANS: { // OPCODE = 0
            error_packet.ErrorCode = htons(0);
            strcpy(error_packet.strings, "Abandoning file transmission");
            int size = 4 + strlen(error_packet.strings) + 1;
            if (sendto(client->socket, &error_packet, size, 0,
                       (struct sockaddr *)&client->client_address, client->client_address_len) != size) {
                perror("TTFTP_ERROR:");
                exit(1);
            }

            break;
        }

        default: ;
            //not supposed to reach here
    }
}


bool isValid(char number[]) {
    //checking if a negative number
    if (number[0] == '-') return false;
    int val = atoi(number);
    if (!val) return false;
    for (int i = 0; number[i] != 0; i++) {
        if (!isdigit(number[i])) {
            return false;
	}
    }
    return true;
}

void check_valid(int argc, char *argv[]){
    // Checking if valid number of arguments
    if (argc != 4) {
        cout << "TTFT_ERROR: illegal arguments" << endl;
        exit(1);
    }

    // Checking valid port number
    if (!isValid(argv[1])) {
        cout << "TTFT_ERROR: illegal arguments" << endl;
        exit(1);
    }

    // Checking valid timeout number
    if (!isValid(argv[2])) {
        cout << "TTFT_ERROR: illegal arguments" << endl;
        exit(1);
    }

    // Checking valid error number
    if (!isValid(argv[3])) {
        cout << "TTFT_ERROR: illegal arguments" << endl;
        exit(1);
    }
}

void send_ack(Client *client){
    struct ACK ack = {0};
    memset(&ack, 0, sizeof(ack));
    ack.opcode = htons(ACK_OPCODE);
    ack.block_number = htons(client->block_counter);

    if (sendto(client->socket, &ack, sizeof(ack), 0,
               (struct sockaddr*)&client->client_address, client->client_address_len) != sizeof(ack)) {
        perror("TTFTP_ERROR:");
        exit(1);
    }


    client->block_counter++;
}

void close_file(char* file_name, int fd) {

    if (close(fd) == -1) {
        perror("TTFTP_ERROR:");
        exit(1);
    }

    if (unlink(file_name) == -1) {
        perror("TTFTP_ERROR:");
        exit(1);
    }
}


int main(int argc,char *argv[]) {
    check_valid(argc, argv);

    int server_socket;
    struct sockaddr_in server_address;
    struct timeval timeout_const;
    struct timeval timeout;

    unsigned short ServPort = atoi(argv[1]);
    timeout_const.tv_sec = atoi(argv[2]);
    timeout_const.tv_usec = 0;
    int number_of_error_const = atoi(argv[3]);

    if ((server_socket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
        perror("TTFTP_ERROR:");
        exit(1);
    }

    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htons(INADDR_ANY);
    server_address.sin_port = htons(ServPort);

    if (bind(server_socket, (struct sockaddr *) &server_address, sizeof(server_address)) < 0) {
        perror("TTFTP_ERROR:");
        exit(1);
    }

    struct PACKET general_packet = {0};
    int received_message_size;

    list<Client> client_list;
    int number_of_clients = 0;
    bool client_exist;

    int select_return;

    while (1) {

	bool not_empty = false;
	//cout << number_of_clients << endl;
        fd_set read_fds;
        FD_ZERO(&read_fds);
        FD_CLR(server_socket, &read_fds);
        FD_SET(server_socket, &read_fds);

        Client temp_client(server_socket, number_of_clients);
        Client *curr_client = &temp_client;

        if (number_of_clients == 0) {
            select_return = select(server_socket + 1, &read_fds, NULL, NULL, NULL);
        } 
	else {
            // get earliest timeout value
            auto next_timeout_expired_client = client_list.begin();
            struct timeval now;
            gettimeofday(&now, NULL);
            timersub(&now, &next_timeout_expired_client->last_ack_time, &timeout);
            timersub(&timeout_const,&timeout,&timeout);

            select_return = select(server_socket + 1, &read_fds, NULL, NULL, &timeout);
        }

        if (select_return > 0) {

            received_message_size = recvfrom(server_socket, &general_packet, MAX_PACKET_SIZE, 0,
                                             (struct sockaddr *) &curr_client->client_address, &curr_client->client_address_len);

            if (received_message_size < 0) {
                perror("TTFTP_ERROR:");
                exit(1);
            }
	    //cout << "client_list.size() " << client_list.size() << endl;
	    if (client_list.size() > 0) {
		    not_empty = true;
	    }

            //check if client is exist for ERROR 4
            auto it = client_list.begin();
            client_exist = false;
            for (; it != client_list.end(); it++) {
                if ((it->client_address.sin_addr.s_addr == curr_client->client_address.sin_addr.s_addr) &&
                    (it->client_address.sin_port == curr_client->client_address.sin_port)) {
                    client_exist = true;
                    break;
                }
            }

            if (!client_exist) { // client does not exist

                unsigned short int opcode = ntohs(general_packet.opcode);
		//cout << "CURR_CLIENT_NUM:!!!!!!! " << curr_client->num << endl;
	   	if (not_empty) {
                   	send_error_message(curr_client, UNEXPECTED_PCKT_ERR_CODE);
                   	continue;
         	}

                if (opcode == DATA_OPCODE) {
                    send_error_message(curr_client, UNKNOWN_USER_ERR_CODE);
                    continue;
                }


                struct WRQ wrq = {0};
                memcpy(&wrq, &general_packet, sizeof(general_packet));
                parsing_wrq(curr_client, &wrq);

                char *file_path = curr_client->file_name;

                // checking if file exists
               if(access(file_path,F_OK) == 0){
                    send_error_message(curr_client, FILE_ALREADY_EXIST_ERR_CODE);
                    continue;
                }

                // open new file
                curr_client->file_d = open(curr_client->file_name, O_CREAT | O_TRUNC | O_APPEND | O_RDWR,
                                           S_IRWXU | S_IRWXG | S_IRWXO);
                if (curr_client->file_d < 0) {
                    perror("TTFTP_ERROR:");
                    exit(1);
                }

                // send ack back
                send_ack(curr_client);
                gettimeofday(&curr_client->last_ack_time, NULL);

                // update client list
                client_list.push_back(*curr_client);
                number_of_clients++;
            } // end of client does not exist block

            else { // client does exist

                curr_client = &(*it); // attach the existing client to curr_client variable

                unsigned short int opcode = ntohs(general_packet.opcode);

                if (opcode == WRQ_OPCODE) {
                    // client is already sent WRQ, client is boiled out
                    send_error_message(curr_client, UNEXPECTED_PCKT_ERR_CODE);
                    close_file(curr_client->file_name, curr_client->file_d);
                    client_list.erase(it);
                    number_of_clients--;
                    continue;
                }

                // DATA packet
                struct DATA data_packet = {0};
                memcpy(&data_packet, &general_packet, sizeof(general_packet));

                // Bad block number
                if (ntohs(data_packet.block_number) != curr_client->block_counter) {

                    send_error_message(curr_client, BAD_BLOCK_NUM_ERR_CODE);
                    close_file(curr_client->file_name, curr_client->file_d);
                    client_list.erase(it);
                    number_of_clients--;
                    continue;
                }

                // DATA packet size is invalid, larger than 516 Bytes
                if (received_message_size > 516){
                    send_error_message(curr_client, BAD_BLOCK_NUM_ERR_CODE);
                    close_file(curr_client->file_name, curr_client->file_d);
                    client_list.erase(it);
                    number_of_clients--;
                    continue;
                }

                int data_size = received_message_size - 4;
                int write_size = write(curr_client->file_d, data_packet.data, data_size);

                if (write_size != data_size) {
                    perror("TTFTP_ERROR:"); // NEED TO CHECK
                    exit(1);
                }

                send_ack(curr_client);
                gettimeofday(&curr_client->last_ack_time, NULL);

                // move element to the back of the list
                client_list.splice( client_list.end(), client_list, it);

                // Last packet from client
                if (received_message_size < 516) {

                    if (close(curr_client->file_d) == -1) {
                        perror("TTFTP_ERROR:");
                        exit(1);
                    }

                    client_list.pop_back();
                    number_of_clients--;
                }

            }
        } // end of (select_return > 0) block

        else if (select_return == 0) { 

            auto it = client_list.begin();
            Client *next_time_expired_client = &(*it);

            next_time_expired_client->timeout_counter++;

            if (next_time_expired_client->timeout_counter > number_of_error_const) {

                send_error_message(next_time_expired_client, ABANDONING_TRANS);
                close_file(next_time_expired_client->file_name,next_time_expired_client->file_d);
                client_list.pop_front();
                number_of_clients--;
                continue;

            } else {

                next_time_expired_client->block_counter--; 
                send_ack(next_time_expired_client);
                gettimeofday(&next_time_expired_client->last_ack_time, NULL);

                client_list.splice( client_list.end(), client_list, it);
            }
        }

        if (select_return < 0) {
            perror("TTFTP_ERROR:");
            exit(1);
        }
    } //end of while(1)

    // close file
    for (auto it = client_list.begin(); it != client_list.end(); it++) {
        close_file(it->file_name,it->file_d);
    }

    return 0;

}
