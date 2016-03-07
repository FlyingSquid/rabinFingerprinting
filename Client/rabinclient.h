/*
 * Flying Squid
 *--------------------
 * Client-side library for partial value-based web caching.
 *
 */
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
    bool old;

} block_desc;


/* Copy this from rabins server */
typedef struct __attribute__((__packed__)) block {

    unsigned block_num;
    size_t data_size;
    bool old;
    char *data;

} block;


class RabinClient
{

    public:
        /* Server name and port number*/
        RabinClient(char * hostname, int port_);
        ~RabinClient();

        /*
        * Receives a file from the server into the open, write enabled
        * file pointer 'file'.
        * Returns the number of blocks added to the file
        */
        unsigned receive_file(FILE *file); 

        /* Requests a block from the server.
        * 
        *   If an 'old' block_desc is required, it just fishes the block from
        *   the local cache.
        *
        *   Else, it adds the received block to the local cache and returns it
        */
        block *receive_block();
    
        /* Establishes a connection to the server */
        int connect_to_server();

        unsigned insert_block (char *b, unsigned size, unsigned bno);
        
        block *get_block(unsigned b);

    private:
        
        /* Hash function to insert blocks to <blocks> */ 
        unsigned int hash_function(char *b, int size);
       
        void error(const char* msg);
        
        /* Simulates a local cache (in memory) */
        std::vector<block *> blocks;
        int portno, sockfd, newsockfd;
        struct hostent* server;
        sockaddr_in serv_addr;

        /* Max size of local store */
        unsigned max_size;
      /***********************************/
};

#endif

