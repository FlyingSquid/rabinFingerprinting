#include "rabinserver.h"
#include <assert.h>
//#include <random>



void RabinServer::error(const char* msg)
{
    perror(msg);
    exit(1);
}


RabinServer::RabinServer(int port_) {

    portno = port_;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);


    if(bind(sockfd, (struct sockaddr *)&serv_addr,
                    sizeof(serv_addr)) < 0) {
            error("ERROR on binding");
    }

    max_size = (blocks.max_size()) / 10000;
}

int RabinServer::connect_to_client() {

    /*Accept condition */
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr,
                                    &clilen);

    if(newsockfd < 0)
        error("Error on accept");


    /* How to read from the client******

    char buffer[256];
    int n;
    bzero(buffer, 256);

    n = read(newsockfd, buffer, 255);
    
    if (n < 0) 
       error("Error reading from socket");

    printf("message: %s\n", buffer);

    ************************************/
    return newsockfd;

}

RabinServer::~RabinServer () {

    close(newsockfd);
    close(sockfd);

    /* Need to destroy blocks here
     * 
     * Iterate through the vector block, free each element.
     *
     *    */
}


// Returns the number of blocks allocated
int RabinServer::add_blocks(char *file, size_t s) {

    /* Break file into blocks, then for each of these
     * blocks call insert_block */


    assert (file != NULL);
    unsigned i;
    char *prev = file;
    int num_blocks = 0;

    cout << "Size of file is ";
    cout << s << endl;

    for(i = 2; i < s; i++) {
        
        if(rabin_func(file[i-2], file[i-1],file[i],i) == 0) {
            /* This is some pointer addition that gives the
             * size of the block */
            size_t width = (file + i - prev);
            insert_block(prev, width);
            num_blocks++; 
            prev = file + i;
        } 
    } 

    if(prev != file + s) {
        size_t width = file + s - prev; 
        cout << "Extra with width ";
        cout << width <<endl;
        insert_block(prev, width);
        num_blocks++;
    }

    return num_blocks;
}


/* Definite problem here, I can't many-one map\
 * {0..255} -> {0...2047}
 *
 * I think the wbvc paper actually maps B x B x B -> {0...2047}
 * I think the paper instantiates n to 3.
 * 
 *
 */
int RabinServer::rabin_func(char b0, char b1, char b2, int i) {

    /* will implement a uniform random function here 
     * Till then, this creates 2KB blocks */

    /* Should use std::uniform_int_distribution here 
 *  but need to seed with byte somehow*/

    (void) b0;
    (void) b1;
    (void) b2;
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


char *RabinServer::get_block (unsigned b) {

    block *block_i = blocks.at(b);
    char *to_return = new char[block_i -> data_size];
    memcpy(to_return, block_i -> data, block_i -> data_size);
    return to_return;
}



/* For now a simple djb2 hash, we should replace this 
 * when we can */
unsigned int RabinServer::hash_function (char *b, int size) {


    cout << "Inserting block of width ";
    cout << size <<endl;


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

/* Returns the block number */
unsigned RabinServer::insert_block(char *b, int size) {

    block *new_block = new block;
    unsigned n = hash_function(b, size);

    cout << "Hash was ";
    cout << n << endl;

    new_block -> block_num = n;
    new_block -> data_size = size;
    memcpy(new_block -> data, b, size);
   
    if(blocks.size() <= n) {
       blocks.resize(2*n);
    }
    
    blocks.at(n) = new_block;
    return n;
}




