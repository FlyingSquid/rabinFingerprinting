#include "rabinserver.h"

/*
 * Example use of rabinserver.h
 *
 * Transfers a file in command line arguments over a network.
 */

int main (int argc, char* argv[]) {

   if(argc < 2) {
            cout << "Too few arguments" <<endl;
            exit(0);
   }

   /* Creating an instance of the server, open at port 1024 */
    RabinServer *r = new RabinServer(atoi(argv[1]));

    /* Connectng to the client */ 
    r -> connect_to_client();
    FILE *t = NULL;

  if(argc == 2) {
    string s;

    while(1) {
        cout << "What file would you like to send: ";
        fflush(stdout);
        cin >> s;
        
        t = fopen(s.c_str(), "r");

        if(t== NULL) {
            cerr<<"File could not be opened"<<endl;
            continue;
        }

        fseek(t, 0L, SEEK_END);
        int n = ftell(t);
        fseek(t, 0L, SEEK_SET);

        char c[n];
        bzero((char *)c, n);
        fread(c, n, 1, t);
        
        cout<<"Sending"<< s <<endl;;
        int a = r->send_file((char *)c, n);
        cerr <<  a << " blocks sent.\n" << endl;

        fclose(t);
        t = NULL;
    }


   } else {
 
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
            
            cerr <<  a << " blocks sent.\n" << endl;
            fclose(t);
            /*************************************************/
        }
    }
    /* Deleting the class, this should invoke the destructor */
    delete (r);

    return 0;

}



