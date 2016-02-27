
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


typedef struct __attribute__((__packed__)) block_desc {

    int block_num;
    size_t data_size;

} block_desc;

typedef struct __attribute__((__packed__)) block {

    int block_num;
    char data[2048];
    size_t data_size;
} block;


class RabinServer
{

    public:
    /* Port to be open at */
    RabinServer(int port_);
    ~RabinServer();

    /* Adds a fles blocks to the hash table */
    int add_blocks(char *file, size_t s); 

    /* Writes a given block to the user */
    int write_to_client(int i);


    /* Reading requests needs to be implemented somewhere else */

    private:

    std::vector<block *> blocks;
   
    /* Rabin hash function here char -> int */
 
    unsigned int hash_function(char *b);
    void insert_block (char *b, int size);
    void error(const char* msg);
    int rabin_func (char byte, int i);
    /* Private class variables go here */  

    int portno, sockfd, newsockfd;
    sockaddr_in serv_addr, cli_addr;

    /***********************************/

};

#endif

