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
         *  Iterate through the vector block, free each element.
         */
}


/* Receives a file from the server
 *
 * Does this by calling receive_block
 *
 * file must be an open file pointer
 *
 */
unsigned RabinClient::receive_file(FILE *file) {

        assert (file != NULL);

        block *b; 
        unsigned num_blocks = 0;
        do {
                b = receive_block();
                if(b != NULL) {
                    fwrite(b->data, b->data_size , 1, file);
                }
                
                num_blocks++;

        } while (b != NULL);

        fflush(file);
 
        return num_blocks;
} 


/* Receives a block from the block cache */
block *RabinClient::receive_block() {

        block_desc bd;
        char *buf;
        int n = read(sockfd, &bd, sizeof(block_desc));

        if(n <= 0) {
            error("Error: Connection may have been closed.");
        }

        bd.block_num = ntohl(bd.block_num);
        bd.data_size = ntohl(bd.data_size);

        size_t s = bd.data_size;
    
        if(s <= 0) {
                cerr << "Received EOF\n"<<endl;
                return NULL;
        }

        cerr << "\n\nReceived block " << bd.block_num<<". Size " << s;
        cerr<< ". Old " <<bd.old<<endl;

        if(!bd.old) {
           
            buf = new char[s]; 

            unsigned bytesRead = 0;


            /* This will happen if there is not sufficient time
            * to read the entire data stream */
            while(bytesRead < s) {

                /* Network may be too fast */            
                n = read(sockfd, buf + bytesRead, s - bytesRead);
                bytesRead += n;
            }
            /* Used to have an error when bytesRead exceeded s*/
            
            insert_block(buf, s, bd.block_num);

        }
        
        return get_block(bd.block_num);
}

/* Establishes a connection to the server */
int RabinClient::connect_to_server() {

        return connect(sockfd,(struct sockaddr *)&serv_addr, 
                            sizeof(serv_addr));

}

/* This should probably eventually be a prvate function */
unsigned RabinClient::insert_block (char *b, unsigned size, unsigned bno) {
        

        block *new_block = new block;
        unsigned n = bno;
        new_block -> block_num = n;
        new_block -> data_size = size;
        /* Redundant here */
        new_block -> old = true;
        new_block->data = b;

        blocks[n] = new_block;
        
        return n;

}

block *RabinClient::get_block(unsigned b) {

    /* TODO: Streamline and optimize this function */

        block *block_i = blocks.at(b);
        block *to_return = new block;
        to_return->block_num = block_i -> block_num;
        to_return -> data_size = block_i -> data_size;
        to_return -> old = block_i -> old;
        to_return -> data = new char[block_i -> data_size];
        memcpy(to_return -> data, block_i -> data, block_i -> data_size);

        return to_return; 
}


void RabinClient::error(const char* msg)
{
        perror(msg);
        exit(1);
}

