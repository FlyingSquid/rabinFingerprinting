#include "rabinserver.h"
#include <assert.h>


/*
 * Flying Squid
 * ------------------
 *Implementation of RabinServer
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

    max_size = (blocks.max_size()) / 10000;


}

int RabinServer::connect_to_client() {

    cerr << "Listening at port no "<< portno <<endl;

    listen(sockfd, 5);
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr,
                                    &clilen);

    /*Accept condition */
    if(newsockfd < 0)
        error("Error on accept");

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

    cerr << "Size of file is " << s << endl;
    for(i = 2; i < s; i++) {
       
        /* Establishes a max size of a block*/

        bool too_large = (((file + i) - prev) >= max_bytes);

        /*************************/

 
        if(rabin_func(file[i-2], file[i-1],file[i],i) == 0
            || too_large) {
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

/*
 * This function hardly gives any 0s
 *
 */


unsigned RabinServer::rabin_func(char b0, char b1, char b2, int i) {

    /* will implement a uniform random function here */

    char hash_me[3];
    hash_me[0] = b0;
    hash_me[1] = b1;
    hash_me[2] = b2;

    unsigned hashval = (hash_function(hash_me, 3) % (max_bytes/2));

    return hashval;
}


int RabinServer::write_block_to_client(unsigned i) {

    
    int n;
    block *block_i = blocks.at(i);

    //Get block_i
    block_desc descriptor;
    descriptor.block_num = htonl(block_i -> block_num);
    descriptor.data_size = htonl(block_i -> data_size);
    descriptor.old = block_i -> old;

    n = write(newsockfd, &descriptor, sizeof(block_desc));

    if (n < 0)
        return n;

    string forlog = descriptor.old ? " Did not write data.":"";
        cerr << "Writing block "<< i <<" to the client. Size "<<block_i->data_size << "." <<forlog<<endl;

    if(!descriptor.old) {

        char *data = (char *) block_i -> data;
        n = write(newsockfd, data , block_i -> data_size);
        block_i -> old = true;
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
  
    try {
        block *cur = blocks.at(n);
        if(cur->data_size != (unsigned)size || memcmp(cur->data, b, size)) {
            cerr<<"Overwriting current block"<<endl;
            blocks[n] = new_block;
            delete cur;
        } else {
            blocks.at(n) -> old = true;
        
            delete (new_block);
        }
    }
    catch (const std::out_of_range & oor) {
        blocks[n] = new_block;
        
    } 
    return n;
}




