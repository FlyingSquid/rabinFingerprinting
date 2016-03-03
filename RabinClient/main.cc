#include "rabinclient.h"

int main (int argc, char *argv[]) {

        if (argc < 2) {
                cout << "Too few arguments" << endl;
                exit(0);
        }

        RabinClient *r = new RabinClient();

        return 0;
}
