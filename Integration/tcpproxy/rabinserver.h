/*
 * Flying Squid
 * ------------
 * 
 * Rabin Fingerprinting Server-side Library
 *
 *
 */


#ifndef RabinServer_H
#define RabinServer_H

#include <unordered_map>
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

typedef struct __attribute__((__packed__)) block {

    unsigned block_num;
    size_t data_size;
    bool old;
    char *data;
    
} block;


class RabinServer
{

    public:
        /* Port at which to be open*/
        RabinServer(int port_);
        ~RabinServer();

        /* Sends a file, block by block to the client*/
        int send_file(char *file, size_t s); 


        /* Writes a given block to the user */
        int write_block_to_client(unsigned i);

        /* Listens for the client and accepts a connection 
        * Must be called before write_to_client is
        * called  */
        int connect_to_client();

        int disconnect_from_client();


        /* Exists only for non-network testing. Adds file blocks
         * to the hash table */  
        int add_blocks(char *file, size_t s); 


    private:
        
        unsigned insert_block (char *b, int size);
        
        char *get_block(unsigned b);
        /* Hash function to insert blocks to <blocks> */ 
        unsigned int hash_function(char *b, int size);
        /* Rabin function */
        unsigned rabin_func (char b0, char b1, char b2, int i);
       
        void error(const char* msg);


        /* Simulates a local cache (in memory) */
        std::unordered_map<unsigned, block *> blocks;

        int portno, sockfd, newsockfd;
        sockaddr_in serv_addr, cli_addr;
        socklen_t clilen;
        unsigned const max_bytes = 64; /* Maximum size of a block */
        unsigned max_size;
        /***********************************/
};

#endif

