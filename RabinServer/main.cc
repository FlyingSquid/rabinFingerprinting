#include "rabinserver.h"

int main (int argc, char* argv[]) {

    RabinServer *r = new RabinServer(0);

    FILE *t = fopen("testfile", "r");
    char c[2048];

    fread(c, 2048, 1, t);

    int a = r->add_blocks((char *)c, 2048);
    cout << a <<endl;

    delete (r);
    return 1;

}



