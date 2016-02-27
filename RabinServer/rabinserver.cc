#include "rabinserver.h"
#include <assert.h>


/* Initially written for exactly one user
 *
 * Rabin Fingerprinting has some issues with multiple users.
 *
 *
 *
 *
 *
 * */


void RabinServer::error(const char* msg)
{
    perror(msg);
    exit(1);
}


RabinServer::RabinServer(int port_) {

    portno = port_;

    /* These might have to be moved to class members */

    socklen_t clilen;
    char buffer[256];
    int n;
    /*************************************************/

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);


    if(bind(sockfd, (struct sockaddr *)&serv_addr,
                    sizeof(serv_addr)) < 0) {
            error("ERROR on binding");
    }

    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr,
                                    &clilen);

    if(newsockfd < 0)
        error("Error on accept");


    /* READING FROM SERVER */

    bzero(buffer, 256);
    n = read(newsockfd, buffer, 255);
    
    if (n < 0) 
       error("Error reading from socket");
    printf("message: %s\n", buffer);
    /***********************/

}

RabinServer::~RabinServer () {

    close(newsockfd);
    close(sockfd);

    /* Need to destroy blocks here*/

}



int RabinServer::add_blocks(char *file, size_t s) {

    /* Break file into blocks, then for each of these
     * blocks call insert_block */

    assert (file != NULL);

    unsigned i;

    char *prev = file;

    for(i = 1; i < s; i++) {
        
        char c = file[i];
        if(rabin_func(c,i) == 0) {
            /* This is some pointer addition that gives the
             * size of the block */
            size_t width = (file + i - prev);
            insert_block(prev, width);
            
            prev = file + i;
        } 

    } 


    /* Returning one indicates success */
    return 1;
}


int RabinServer::rabin_func(char byte, int i) {

    /* will implement a Rabin function here 
     * Till then, this creates 2KB blocks */

    (void) byte;

    return (i % 2048);

}

int RabinServer::write_to_client(int i) {

    int n;

    block *block_i = blocks.at(i);
    
    block_desc descriptor;

    descriptor.block_num = block_i -> block_num;
    descriptor.data_size = block_i -> data_size;

    char *data = (char *) block_i -> data;

    n = write(newsockfd, &descriptor, sizeof(block_desc));

    if( n < 0) {

        cerr << "Error writing to client" << endl;
    } else {

        n = write(newsockfd, data , block_i -> data_size);

    }

    return n;
} 


/* For now a simple djb2 hash, we should replace this 
 * when we can */
unsigned int RabinServer::hash_function (char *b) {

    unsigned int hash = 5381;
    int c;

    while ((c = (*b++)))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c*/
    
    return hash;

}

void RabinServer::insert_block(char *b, int size) {

    block *new_block = new block;
    int n = hash_function(b);

    new_block -> block_num = n;
    new_block -> data_size = size;
    memcpy(new_block -> data, b, size);
    
    blocks.at(n) = new_block;

}




