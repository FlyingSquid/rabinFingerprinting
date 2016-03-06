#include "rabinserver.h"

int main (int argc, char* argv[]) {

    if(argc < 2) {
        cout << "Too few arguments" <<endl;
        exit(0);
    }

    RabinServer *r = new RabinServer(1024);
    /******** This could be abstracted to the class itself */ 
    FILE *t = fopen(argv[1], "r");

    fseek(t, 0L, SEEK_END);
    int n = ftell(t);
    fseek(t, 0L, SEEK_SET);

    char c[n];
    fread(c, n, 1, t);
    /****************************************************/
    
    r -> connect_to_client();



    for(int i = 0; i < 2; i++) {

        int a = r->send_file((char *)c, n);
        cout <<  a << " blocks sent." << endl;
        sleep(2);


    }


/*
    string test = "ThisIsATestString\n";
    char *ctest = (char *)test.c_str();
    unsigned len = test.length();
    unsigned n1 = r->insert_block(ctest, len);
    
    for(int i = 0; i < 4; i++) {
        r->write_block_to_client(n1);
        sleep(1);
    }
*/

    delete (r);

    return 0;

}



