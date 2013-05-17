#include "Parser.hpp"
// EVERYBODY STAND BACK
// I know regular expressions

// just kidding

#include <iostream>

#define MIN(a, b) ((a < b) ? a : b)


void
Parser::die(std::string m)
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

Operation*
Parser::TOP()
{
    Operation *ret = new Operation;
    try {
        ret->type = operation();
        consume("(");
        ret->elements = elements();
        consume(")");
    } catch (Parser::Exception *p) {
        delete ret;
        throw p;
    }
    return ret;
}

Operation::Type Parser::operation()
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

std::list<Element*>*
Parser::elements()
{
    std::list<Element*> *ret = new std::list<Element*>();
    Element *e;
    try {
        e = element();
        if (e) ret->push_back(e);
        else die("Zero elements not allowed");
        skipws();
        while (peek(',')) {
            consume(",");
            skipws();
            e = element();
            if (e != nullptr)
                ret->push_back(element());
            else die("Expected an element after a comma");
        }
    } catch (Parser::Exception *p) {
        for (auto i = ret->begin(); i != ret->end(); i++) {
            delete *i;
        }
        delete ret;
        throw p;
    }
    return ret; 
}

Element *
Parser::element()
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
    if (peek('.')) {
        consume(".");
        std::string val;
        int oldcur = cur;
        while (isdigit(source[cur])) {
            cur++;
        }
        if (oldcur == cur) {
            delete ret;
            die("Malformed floating point value");
        }
        val.append(source + oldcur, cur - oldcur);
        ret->type = Element::Type::NUM;
        // FIXME: This will fuck up on something like 3.014 (becomes 3.14)
        // Let's hope nobody notices :>
        ret->num_value = ret->int_value + atoi(val.c_str()) * 0.01;
    }
    return ret;
}
