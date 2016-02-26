#include "rabinserver.h"



/* Initially written for exactly one user
 *
 * Rabin Fingerprinting has some issues with multiple users.
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
}



int RabinServer::add_blocks(char *file) {

     

    /* Returning one indicates success */
    return 1;
}


int RabinServer::write_to_client(int i) {

    int n;

    block *to_write = blocks.at(i);
    n = write(newsockfd, to_write, sizeof(to_write));

    if( n < 0) {

        cerr << "Error writing to client" << endl;
    }

    return n;
} 



int RabinServer::hash_function (char *b) {

    (void) b;
    return 1;

}

void RabinServer::insert_block(char *b) {

    block *new_block = new block;
    int n = hash_function(b);

    new_block -> block_num = n;
    new_block -> data = b;
    
    /* Have to do something with "hash" */
    new_block -> hash = NULL;
    /************************************/
    
    blocks.at(n) = new_block;

}




