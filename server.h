
#ifndef Server_H
#define Server_H


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
    RabinServer(int port);
    /* Adds a fles blocks to the hash table */
    void add_blocks(char *file); 

    /* Writes a given block to the user */
    int write_to_client(int block);


    private:

    /*Need a hash table here*/


}



