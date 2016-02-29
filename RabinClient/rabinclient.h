
#ifndef RabinClient_H
#define RabinClient_H


#include <vector>
#include <functional>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

using namespace std;

typedef struct __attribute__((__packed__)) block_desc {

    unsigned block_num;
    size_t data_size;

} block_desc;

typedef struct __attribute__((__packed__)) block {

    unsigned block_num;
    char *data;
    size_t data_size;
} block;


class RabinClient
{

    public:
        /* Port at which to be open*/
        RabinClient(char * hostname, int port_);
        ~RabinServer();

        int request_file(char *file, size_t s); 

        int request_block(int i);

        int connect_to_server();


        unsigned insert_block (char *b, int size);
        
        char *get_block(unsigned b);

    private:
        
        /* Hash function to insert blocks to <blocks> */ 
        unsigned int hash_function(char *b, int size);
        /* Rabin function */
        unsigned rabin_func (char b0, char b1, char b2, int i);
       
        void error(const char* msg);
        /* Private class variables go here */  

        /* Simulates a local cache (in memory) */
        std::vector<block *> blocks;
        int portno, sockfd, newsockfd;
        sockaddr_in serv_addr, cli_addr;
        socklen_t clilen;

        unsigned max_size;
        /***********************************/
};

#endif

