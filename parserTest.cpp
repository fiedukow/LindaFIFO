#include "Parser.hpp"
#include <iostream>

int testno = 0;

void parses_ok(const char *s) {
    Parser p(s);
    Operation *op = nullptr;
    try {
        op = p.TOP();
        std::cout << "ok " << testno++ <<  " - " << s << std::endl;
    } catch (Parser::Exception *p) {
        std::cout << "not ok - " << s << ": " << p->what() << std::endl;
        delete p;
    }
    delete op;
}

int main(void)
{
    parses_ok("input()");
    parses_ok("input(5)");
    parses_ok("input(55)");
    parses_ok("input(55,5)");
    parses_ok("input(55, 5)");
    parses_ok("input(55 , 5)");
    return 0;
}
