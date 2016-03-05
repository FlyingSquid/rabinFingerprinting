#include "rabinclient.h"

int main (int argc, char *argv[]) {

        if(argc < 3) {
            cout << "Usage requires hostname and port no" << endl;
        }
        int port = atoi(argv[2]);
        RabinClient *r = new RabinClient(argv[1], port);
        
        int a = r->connect_to_server();
        cerr << "Connecting returned " << a << endl;

        a = r->receive_file(stdout);

        fflush(stdout);

        /*cerr << "\n\n\nAgain\n\n\n" <<endl;

        a = r -> receive_file(stdout);*/

        cerr << "Received " << a <<" blocks."<<endl;
        delete (r);

        return 0;
}
