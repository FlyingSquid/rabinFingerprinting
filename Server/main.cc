#include "rabinserver.h"

/*
 * Example use of rabinserver.h
 *
 * Transfers a file in command line arguments over a network.
 */

int main (int argc, char* argv[]) {

    if(argc < 3) {
        cout << "Too few arguments" <<endl;
        exit(0);
    }

    /* Creating an instance of the server, open at port 1024 */
    RabinServer *r = new RabinServer(1024);


    /******** Storing the input file in a char array   */ 
    FILE *t = fopen(argv[1], "r");

    fseek(t, 0L, SEEK_END);
    int n = ftell(t);
    fseek(t, 0L, SEEK_SET);

    char c[n];
    fread(c, n, 1, t);
    /****************************************************/
  
    FILE *t_ = fopen(argv[2], "r");

    fseek(t_, 0L, SEEK_END);
    n = ftell(t);
    fseek(t_, 0L, SEEK_SET);

    char c_[n];
    fread(c_, n, 1, t_);




    /****************************************************/
 
    /* Connectng to the client */ 
    r -> connect_to_client();


    printf("Sending %s\n",argv[1]);
    int a = r->send_file((char *)c, n);
    cout <<  a << " blocks sent." << endl;

    printf("Sending %s\n",argv[2]);
    a = r -> send_file((char *)c_, n);
    cout <<  a << " blocks sent." << endl;


    /* Deleting the class, this should invoke the destructor */
    delete (r);

    return 0;

}



