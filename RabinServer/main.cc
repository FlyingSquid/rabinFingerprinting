#include "rabinserver.h"

int main (int argc, char* argv[]) {

    if(argc < 2) {
        cout << "Too few arguments" <<endl;
        exit(0);
    }

    RabinServer *r = new RabinServer(0);

    FILE *t = fopen(argv[1], "r");

    fseek(t, 0L, SEEK_END);
    int n = ftell(t);
    fseek(t, 0L, SEEK_SET);


    char c[n];

    fread(c, n, 1, t);

    int a = r->add_blocks((char *)c, n);
    cout << a <<endl;

    string test = "ThisIsATestString";

    char *ctest = (char *)test.c_str();

    unsigned len = test.length();

    unsigned n1 = r->insert_block(ctest, len);

    char *x = r->get_block(n1);
    cout << x <<endl;
    delete (r);
    return 1;

}



