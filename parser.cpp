// EVERYBODY STAND BACK
// I know regular expressions

// just kidding

#include <string>
#include <iostream>
#include <list>
// yes, I'm mixing C with C++
#include <cstring>
#include <cctype>

#define MIN(a, b) ((a < b) ? a : b)

struct Element {
    enum Type { INT, NUM, STRING };
    Type type;
    int int_value;
    double num_value;
    std::string string_value;
};

struct Operation {
    enum Type { INPUT, OUTPUT, READ };
    Type type;
    std::list<Element*>* elements;

    ~Operation()
    {
        for (auto i = elements->begin(); i != elements->end(); i++) {
            delete *i;
        }
        delete elements;
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

    void die(const char *m)
    {
        std::string exp = m;
        die(exp);
    }

    void die(std::string& m)
    {
        std::string msg = m;
        msg.append(" near '");
        // no more than 10 characters from source
        msg.append(source + cur, MIN(10, strlen(source + cur)));
        if (source[cur] == '\0') {
            msg.append("(end of string)");
        }
        msg.append("'");
        throw new Exception(msg.c_str());
    }

    Parser(const char *src) : source(src), cur(0) { }

    void consume(const char *pattern)
    {
        if (!strncmp(pattern, source + cur, strlen(pattern))) {
            cur += strlen(pattern);
        } else {
            std::string exp = "Expected ";
            exp.append(pattern);
            die(exp);
        }
    }

    bool peek(char c) { return source[cur] == c; }

    void skipws()
    {
        while (isblank(source[cur])) cur++;
    }

    Operation* TOP()
    {
        puts("parsing top");
        Operation *ret = new Operation;
        try {
            ret->type = operation();
            printf("operation is %d\n", ret->type);
            consume("(");
            ret->elements = elements();
            consume(")");
        } catch (Parser::Exception *p) {
            delete ret;
            throw p;
        }
        return ret;
    }

    Operation::Type operation()
    {
        if (!strncmp("input", source + cur, strlen("input"))) {
            cur += strlen("input");
            return Operation::Type::INPUT;
        }
        if (!strncmp("output", source + cur, strlen("output"))) {
            cur += strlen("output");
            return Operation::Type::OUTPUT;
        }
        if (!strncmp("read", source + cur, strlen("read"))) {
            cur += strlen("read");
            return Operation::Type::READ;
        }
        die("unrecognized operation");
        return Operation::Type::INPUT; // fuck off, clang
    }

    std::list<Element*>* elements()
    {
        std::list<Element*> *ret = new std::list<Element*>();
        Element *e;
        try {
            e = element();
            if (e) ret->push_back(e);
            skipws();
            while (peek(',')) {
                consume(",");
                skipws();
                ret->push_back(element());
            }
        } catch (Parser::Exception *p) {
            delete ret;
            throw p;
        }
        return ret; 
    }

    Element * element()
    {
        std::string val;
        int oldcur = cur;
        while (isdigit(source[cur])) {
            cur++;
        }
        if (oldcur == cur) return nullptr;
        val.append(source + oldcur, cur - oldcur);
        Element *ret = new Element;
        ret->type = Element::Type::INT;
        ret->int_value = atoi(val.c_str());
        printf("New value: %d\n", ret->int_value);
        return ret;
    }
};

int main(void)
{
    Operation *op = nullptr;
    Parser p("input(55, 5, 0)");
    try {
        op = p.TOP();
    } catch (Parser::Exception* ex) {
        std::cout << "Exception: " << ex->what() << std::endl;
        delete ex;
    }

    delete op;

    return 0;
}
