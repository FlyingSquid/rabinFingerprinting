#include "rabinserver.h"

int main (int argc, char* argv[]) {

    RabinServer *r = new RabinServer(0);

    delete (r);
    return 1;

}



