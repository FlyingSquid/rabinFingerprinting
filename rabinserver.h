
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

/* Opens a connection to a machine, helps maintain a rabin fingerprinting
 * approach
 *
 *
 *
 */


typedef struct __attribute__((__packed__)) block {

    int block_num;
    char *hash;
    char *data;
} block;


class RabinServer
{

    public:
    /* Port to be open at */
    RabinServer(int port_);
    ~RabinServer();

    /* Adds a fles blocks to the hash table */
    int add_blocks(char *file); 

    /* Writes a given block to the user */
    int write_to_client(int i);


    /* Reading requestsneeds to be implemented somewhere else */

    private:

    std::vector<block *> blocks;
   
    /* Rabin hash function here char -> int */
 
    int hash_function(char *b);
    void insert_block (char *b);
    void error(const char* msg);
    /* Private class variables go here */  

    int portno, sockfd, newsockfd;
    sockaddr_in serv_addr, cli_addr;

    /***********************************/

};

#endif

