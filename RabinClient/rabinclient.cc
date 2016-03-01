#include "rabinclient.h"
#include <assert.h>

/* Server name and port number*/
RabinClient::RabinClient(char * hostname, int port_) {

}

/* destructor */
RabinClient::~RabinClient() {

}

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
 */
char *RabinClient::request_file(char *file_name, size_t s) {


}

/* Requests a block from the block cache */
int RabinClient::request_block(int i) {

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

