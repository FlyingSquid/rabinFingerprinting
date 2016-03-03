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

        close(newsockfd);
        close(sockfd);
        /* Need to destroy blocks here
         * 
         * Iterate through the vector block, free each element.
         *
         */


}


/* Receives a file from the server
 *
 * Does this by calling receive_block
 *
 * Returns a pointer to null if nothing is received
 * Returns the whole file if file is received
 */
char *RabinClient::receive_file() {

        char *block_data;
        char *whole_file;
        do {
                block_data = receive_block();
                /* concatenate block data to whole file */ 
        } while (receive_block() != NULL) 
                 
        return whole_file;
} 


/* Receives a block from the block cache */
char *RabinClient::receive_block() {

        block_desc bd;
        char *buf;
        int n = read(sockfd, &bd, sizeof(block_desc));
        unsigned s = bd.data_size;
        if(n <= 0) {
                return NULL;
        } else {
                n = read(sockfd, buf, s);
        }

        if(!bd.old) {
                buf = new char[size]; 
                insert_block(buf, s, bd.block_num);
        } else {
                buf = get_block(bd.block_num);
        }

        return buf;
}

/* Establishes a connection to the server */
int RabinClient::connect_to_server() {

        return connect(sockfd,(struct sockaddr *)&serv_addr, sizeof(serv_addr));

}

/* This should probably eventually be a prvate function */
unsigned RabinClient::insert_block (char *b, int size, int bno) {
        block *new_block = new block;
        unsigned n = hash_function(b, size);


        new_block -> block_num = n;
        new_block -> data_size = size;
        new_block -> old = false;
        new_block->data = new char[size];
        memcpy(new_block -> data, b, size);

        if(blocks.size() <= n) {
                blocks.resize(2*n);
        }

        if(blocks.at(n) == NULL) {
                blocks.at(n) = new_block;
        } else {
                blocks.at(n)-> old = true;
        }
        return n;

}

char *RabinClient::get_block(unsigned b) {
        block *block_i = blocks.at(b);
        char *to_return = new char[block_i -> data_size];
        memcpy(to_return, block_i -> data, block_i -> data_size);
        return to_return; 
}

/* Hash function to insert blocks to <blocks> */ 
unsigned int RabinClient::hash_function(char *b, int size) {

        //cout << "Inserting block of width ";
        //cout << size <<endl;


        char *str = new char[size+1];
        memcpy(str, b, size);
        str[size] = 0;

        unsigned int hash = 5381;
        int c;

        while ((c = (*str))) {
                str++;
                hash = ((hash << 5) + hash) + c; /* hash * 33 + c*/

        }
        //  delete str;

        return (hash % max_size);

}

void RabinClient::error(const char* msg)
{
        perror(msg);
        exit(1);
}

