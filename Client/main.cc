#include "rabinclient.h"

int main (int argc, char *argv[]) {

        if(argc < 4) {
            cout << "Usage requires hostname, port no, file addr" << endl;
        }
        int port = atoi(argv[2]);
        RabinClient *r = new RabinClient(argv[1], port);
        
        int a = r->connect_to_server();

        FILE *f = fopen(argv[3], "w+");
        string s1 = argv[3];
        int go=1;
        while(go){        
            a = r->  receive_file(f);
            fflush(f);
            //cerr << "Received " << a <<" blocks.\n\n"<<endl;
            //
            string s = "firefox 'pwd'/" + s1;
            system(s1.c_str());
            cin >> a;
        }
        delete (r);

        return 0;
}
