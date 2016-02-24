
#ifndef RabinServer_H
#define RabinServer_H

#include <functional>
using namespace std;

/* Opens a connection to a machine, helps maintain a rabin fingerprinting
 * approach
 *
 *
 *
 */

class RabinServer
{

    public:
    /* Port to be open at */
    RabinServer(int port_);
    /* Adds a fles blocks to the hash table */
    void add_blocks(char *file); 

    /* Writes a given block to the user */
    int write_to_client(int client, int block);


    private:

    std::vector<char *> blocks;
   
    /* Rabin hash function here char -> int */
 


    /* Private class variables go here */  

    int port;

    /***********************************/

}



