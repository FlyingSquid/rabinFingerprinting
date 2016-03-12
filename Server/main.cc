#include "rabinserver.h"

/*
 * Example use of rabinserver.h
 *
 * Transfers a file in command line arguments over a network.
 */

int main (int argc, char* argv[]) {

    if(argc < 4) {
        cout << "Too few arguments" <<endl;
        exit(0);
    }

    /* Creating an instance of the server, open at port 1024 */
    RabinServer *r = new RabinServer(atoi(argv[1]));

    /* Connectng to the client */ 
    r -> connect_to_client();

    FILE *t = NULL;


    for (int i = 2; i < argc; i++) {
        
    /******** Storing the input file in a char array **/ 
        t = fopen(argv[i], "r");

        fseek(t, 0L, SEEK_END);
        int n = ftell(t);
        fseek(t, 0L, SEEK_SET);

        char c[n];
        bzero((char *)c, n);
        fread(c, n, 1, t);
    /**************************************************/
        printf("Sending %s\n",argv[i]);
        int a = r->send_file((char *)c, n);
        cout <<  a << " blocks sent.\n" << endl;
        fclose(t);
    /*************************************************/
    }

    /* Deleting the class, this should invoke the destructor */
    delete (r);

    return 0;

}



