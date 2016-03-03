#include "rabinclient.h"
#include <assert.h>

/* Server name and port number*/
RabinClient::RabinClient(char *hostname, int port_) {
        
        portno = port_;

        sockfd = socket(AF_INT, SOCK_STREAM, 0);
        if (sockfd < 0) 
                error("ERROR opening socket");
        server = gethostbyname(hostname);
        if (server == NULL) {
                fprintf(stderr,"ERROR, no such host\n");
                exit(0);
        }

        bzero((char *) &serv_addr, sizeof(serv_addr)); 
        serv_addr.sin_family = AF_INET;
        bcopy((char *)server->h_addr, 
             (char *)&serv_addr.sin_addr.s_addr,
             server->h_length);
        serv_addr.sin_port = htons(portno); 

        max_size = (blocks.max_size()) / 10000;
}

/* destructor */
RabinClient::~RabinClient() {

        close(newsockfd);
        close(sockfd);
        /* Need to destroy blocks here
         * 
         * Iterate through the vector block, free each element.
         *
         */
        

}

/* Receives a file (as blocks) from the server
 * Does this by calling request_block
 * Returns a pointer to null if nothing is received
 */
char *RabinClient::receive_file() {

} 


/* Requests a block from the server.
 * If an 'old' block_desc is required, it just fishes the block from the local cache.
 * Else, it adds the received block to the local cache and returns it.
 */
char *RabinClient::request_block(int i) {

}

/* Establishes a connection to the server */
int RabinClient::connect_to_server() {

}

/* This should probably eventually be a prvate function */
unsigned RabinClient::insert_block (char *b, int size) {

}

char *RabinClient::get_block(unsigned b) {

}

/* Hash function to insert blocks to <blocks> */ 
unsigned int RabinClient::hash_function(char *b, int size) {

}

/* Rabin function */
unsigned RabinClient::rabin_func (char b0, char b1, char b2, int i) {

}

void RabinClient::error(const char* msg)
{
    perror(msg);
    exit(1);
}

