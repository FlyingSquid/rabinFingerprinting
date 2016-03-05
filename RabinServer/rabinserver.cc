#include "rabinserver.h"
#include <assert.h>


/*
 * TODO:
 *  - Implement an actual Rabin Function to split blocks, current ne
 *    is tooo damn slow.
 *
 *  - Implement hash table instead of vector
 *
 *
 */



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

    //max_size = (blocks.max_size()) / 10000;
    max_size = 100; /* Change this later */


}

int RabinServer::connect_to_client() {

    cout << "Listening at port no "<< portno <<endl;

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
     */
}



/* Returns the number of files sent */
int RabinServer::send_file(char *file, size_t s) {

    assert (file != NULL);
    unsigned i;
    char *prev = file;
    int num_blocks = 0;
    unsigned block_num;

    cout << "Size of file is " << s << endl;

    for(i = 2; i < s; i++) {
        
        if(rabin_func(file[i-2], file[i-1],file[i],i) == 0) {
            /* This is some pointer addition that gives the
             * size of the block */
            size_t width = (file + i - prev);
           
            block_num = insert_block(prev, width);
            write_block_to_client(block_num); 
            prev = file + i;
            num_blocks++; 
        } 
    } 

    if(prev != file + s) {
        size_t width = file + s - prev; 
        block_num = insert_block(prev, width);
        write_block_to_client(block_num); 
        num_blocks++;
    }

    /* EOF denoted by block_desc with 0 size*/

    block_desc descriptor;
    descriptor.block_num = htonl(0);
    descriptor.data_size = htonl(0);
    descriptor.old = false;

    write(newsockfd, &descriptor, sizeof(block_desc));
    num_blocks++;

    return num_blocks;
}




// Returns the number of blocks sent
int RabinServer::add_blocks(char *file, size_t s) {

    /* Break file into blocks, then for each of these
     * blocks call insert_block */


    assert (file != NULL);
    unsigned i;
    char *prev = file;
    int num_blocks = 0;

    cout << "Size of file is " << s << endl;

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
        insert_block(prev, width);
        num_blocks++;
    }

    return num_blocks;
}


unsigned RabinServer::rabin_func(char b0, char b1, char b2, int i) {

    /* will implement a uniform random function here */

    char hash_me[3];
    hash_me[0] = b0;
    hash_me[1] = b1;
    hash_me[2] = b2;

    unsigned hashval = (hash_function(hash_me, 3) % 1024);
/*
    if(hashval == 0) {

        cout<<i<<endl;
    }*/
    return hashval;
}
/*
 *
 *  May htonl here
 *
 */


int RabinServer::write_block_to_client(unsigned i) {

    
    int n;
    block *block_i = blocks.at(i);
   
    block_desc descriptor;
    descriptor.block_num = htonl(block_i -> block_num);
    descriptor.data_size = htonl(block_i -> data_size);
    descriptor.old = block_i -> old;

    n = write(newsockfd, &descriptor, sizeof(block_desc));

    if (n < 0)
        return n;

    if(!descriptor.old) {

        char *data = (char *) block_i -> data;
        n = write(newsockfd, data , block_i -> data_size);

        cout << "Writing block "<< i <<" to the client. Size "<<block_i->data_size <<endl;
    } else {

        cout << "Writing block "<< i <<" to the client. "<< "Did not write data" <<endl;

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
        delete (new_block);
        blocks.at(n)-> old = true;
    }
    return n;
}




