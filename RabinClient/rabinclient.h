
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
        /* Server name and port number*/
        RabinClient(char * hostname, int port_);
        ~RabinServer();

        /* Requests a file from the server
         *
         * When it receives blocks back from the server,
         * it stores the blocks where they need to be stored.
         *
         * Then recombines and returns a file
         *
         *
         * Requests to server are made in terms of blocks only
         *
         * Server caches only on content here, probably need to cache
         * on file name toooo
         *
         * */
        char *request_file(char *file_name, size_t s); 

        /* Requests a block from the block cache */
        int request_block(int i);

        /* Establishes a connection to the server */
        int connect_to_server();

        /* This should probably eventually be a prvate function */
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

        struct hostent* server;
        sockaddr_in serv_addr;

        unsigned max_size;
        /***********************************/
};

#endif

