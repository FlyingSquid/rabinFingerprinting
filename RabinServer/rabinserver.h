
#ifndef RabinServer_H
#define RabinServer_H


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
    char data[2048];
    size_t data_size;
} block;


class RabinServer
{

    public:
        /* Port at which to be open*/
        RabinServer(int port_);
        ~RabinServer();

        /* Adds a fles blocks to the hash table */
        int add_blocks(char *file, size_t s); 

        /* Writes a given block to the user */
        int write_to_client(int i);

        /* Listens for the client and accepts a connection 
        * Must be called before write_to_client is
        * called  */
        int connect_to_client();


        /* Inserts a block into the local cache */
        unsigned insert_block (char *b, int size);
        
        char *get_block(unsigned b);

    private:
        
        /* Hash function to insert blocks to <blocks> */ 
        unsigned int hash_function(char *b, int size);
        /* Rabin function */
        int rabin_func (char byte, int i);
       
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

