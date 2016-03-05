#include "rabinclient.h"

int main (int argc, char *argv[]) {

        if(argc < 3) {
            cout << "Usage requires hostname and port no" << endl;
        }
        int port = atoi(argv[2]);
        RabinClient *r = new RabinClient(argv[1], port);
        
        int a = r->connect_to_server();
        cout << "Connecting returned " << a << endl;
        return 0;
}
