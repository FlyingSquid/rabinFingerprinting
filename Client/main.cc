#include "rabinclient.h"

int main (int argc, char *argv[]) {

        if(argc < 4) {
            cout << "Usage requires hostname, port no, file addr" << endl;
        }
        int port = atoi(argv[2]);
        RabinClient *r = new RabinClient(argv[1], port);
        
        int a = r->connect_to_server();
        
        if (a < 0) {
            cerr<<"Could not connect"<<endl;
            exit(1);
        }

        string s1 = argv[3];
        int go=1;
        while(go){

            FILE *f = fopen(argv[3], "w+");

            a = r->  receive_file(f);
            fflush(f);

            cerr << "Received " << a <<" blocks.\n\n"<<endl;
            string s = "xdg-open " + s1;
            system(s.c_str());
            cin >> go;
            fclose(f);
        }
        delete (r);

        return 0;
}
