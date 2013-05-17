#ifndef _PARSER_HPP_
#define _PARSER_HPP_

#include <string>
#include <list>
// yes, I'm mixing C with C++
#include <cstring>
#include <cctype>

struct Element {
    enum Type { INT, NUM, STR };
    Type type;
    int int_value;
    double num_value;
    std::string string_value;
};

struct Operation {
    enum Type { INPUT, OUTPUT, READ };
    Type type;
    std::list<Element*>* elements = nullptr;

    ~Operation()
    {
        if (elements != nullptr) {
            for (auto i = elements->begin(); i != elements->end(); i++) {
                delete *i;
            }
            delete elements;
        }
    }
};

/*
 * TOP ::== operation '(' elements ')'
 * operation ::== 'input'
 * elements ::== element elements_tail
 * elements_tail ::== ',' element elements_tail | eps
 */

struct Parser {
    const char *source;
    int cur;


    struct Exception : std::exception {
        const char *msg;

        Exception(const char *m) : msg(m) { }

        const char * what() { return msg; }
    };

    void die(std::string m);
    void die(const char *m) { die(std::string(m)); }

    Parser(const char *src) : source(src), cur(0) { }
    void consume(const char *);
    bool peek(char c) { return source[cur] == c; }
    void skipws() { while (isblank(source[cur])) cur++; }
    Operation* TOP();
    Operation::Type operation();
    std::list<Element*>* elements();
    Element * element();
    Element * parse_string();
};

#endif
