#include "rabinclient.h"

int main (int argc, char *argv[]) {


        string server = "dell24";

        RabinClient *r = new RabinClient((char *)server.c_str(), 9260);
        
        int a = r->receive_file(stdout);
        cout << "Read " << a <<" blocks"<<endl;
        return 0;
}
