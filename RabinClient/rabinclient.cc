#include "rabinclient.h"
#include <assert.h>
#include <netinet/in.h>
#include <netdb.h>

/* Server name and port number*/
RabinClient::RabinClient(char * hostname, int port_) {

   portno = port_;

   sockfd = socket(AF_INET, SOCK_STREAM, 0);

   server = gethostbyname(hostname); 

   if(server == NULL) {
        error("Could not connect to host\n");
   }

   bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr,
        server->h_length);

    serv_addr.sin_port = htons(portno);


}

/* destructor */
RabinClient::~RabinClient() {

}

/* Receives a file from the server
 *
 * Does this by calling request_block
 *
 * Returns a pointer to null if nothing is received
 */
char *RabinClient::receive_file(char *file_name, size_t s) {


}

/* Receives a block from the block cache */
char *RabinClient::receive_block(int i) {

    block_desc bd;
    char *buf;
    int n = read(sockfd, &bd, sizeof(block_desc));
    unsigned s = bd.data_size;
    buf = new char[size]; 
    if(n <= 0) {
        return NULL;
    } else {
        n = read(sockfd, buf, s);
    }
    
    if(!bd.old)
        insert_block(buf, s, bd.block_num);


    return buf;
}

/* Establishes a connection to the server */
int RabinClient::connect_to_server() {

  return connect(sockfd,(struct sockaddr *)&serv_addr, sizeof(serv_addr));

}

/* This should probably eventually be a prvate function */
unsigned RabinClient::insert_block (char *b, int size, int bno) {

}

char *RabinClient::get_block(unsigned b) {
    /* Copy from rabinserver */
}

/* Hash function to insert blocks to <blocks> */ 
unsigned int RabinClient::hash_function(char *b, int size) {
    /* Just copy hash_function from rabinserver*/
}


void RabinClient::error(const char* msg)
{
    perror(msg);
    exit(1);
}

