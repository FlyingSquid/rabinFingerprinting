#include "rabinclient.h"

int main (int argc, char *argv[]) {

        if (argc < 2) {
                cout << "Too few arguments" << endl;
                exit(0);
        }

        RabinClient *r = new RabinClient("dell24", 9260);
        
        char *a = r->receive_file();

        cout << a << endl;

        return 0;
}
