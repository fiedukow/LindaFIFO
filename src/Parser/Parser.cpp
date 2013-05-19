#include "Parser.hpp"
// EVERYBODY STAND BACK
// I know regular expressions

// just kidding

#include <iostream>

#define MIN(a, b) ((a < b) ? a : b)

void
Parser::consume(const char *pattern)
{
    if (!strncmp(pattern, source + cur, strlen(pattern))) {
        cur += strlen(pattern);
    } else {
        std::string exp = "Expected ";
        exp.append(pattern);
        die(exp);
    }
}

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
                ret->push_back(e);
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
Parser::parse_string()
{
    consume("\"");
    int oldcur = cur;
    while (!(source[cur - 1] != '\\' && peek('"'))) {
        cur++;
    }
    Element *ret = new Element;
    ret->type = Element::Type::STR;
    ret->string_value.append(source + oldcur, cur - oldcur);
    try {
        consume("\"");
    } catch (Parser::Exception *p) {
        delete ret;
        throw p;
    }
    return ret;
}

int
Parser::parse_int()
{
    std::string val;
    int oldcur = cur;
    while (isdigit(source[cur])) {
        cur++;
    }
    if (oldcur == cur) die("Null element found");
    val.append(source + oldcur, cur - oldcur);
    return atoi(val.c_str());
}

Element *
Parser::element()
{
    // constrained element, for input()
    if (peek("integer")) {
        consume("integer");
        consume(":");
        int val = parse_int();
        Element *ret = new Element;
        ret->type = Element::Type::INT;
        ret->constraint = Element::Constraint::EQ;
        ret->int_value = val;
        return ret;
    }
    // concrete element, for output()
    if (peek('"')) {
        return parse_string();
    }
    int val = parse_int();
    Element *ret = new Element;
    ret->type = Element::Type::INT;
    ret->int_value = val;
    if (peek('.')) {
        consume(".");
        try {
            val = parse_int();
        } catch (Parser::Exception *p) {
            delete p;
            delete ret;
            die("Malformed floating point value");
        }
        ret->type = Element::Type::NUM;
        // FIXME: This will fuck up on something like 3.014 (becomes 3.14)
        // Let's hope nobody notices :>
        ret->num_value = ret->int_value + val * 0.01;
    }
    return ret;
}
