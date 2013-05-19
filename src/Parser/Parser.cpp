#include "Parser.hpp"
// EVERYBODY STAND BACK
// I know regular expressions

// just kidding

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

std::string
Parser::parse_string()
{
    std::string ret;
    consume("\"");
    int oldcur = cur;
    while (!(source[cur - 1] != '\\' && peek('"'))) {
        cur++;
    }
    ret.append(source + oldcur, cur - oldcur);
    consume("\"");
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

std::pair<Element::Type, double>
Parser::parse_numeric()
{
    Element::Type type = Element::Type::INT;
    double num = (double)parse_int();
    if (peek('.')) {
        consume(".");
        int val = parse_int();
        type = Element::Type::NUM;
        // FIXME: This will fuck up on something like 3.014 (becomes 3.14)
        // Let's hope nobody notices :>
        num = num + val * 0.01;
    }
    return std::make_pair(type, num);
}

Element *
Parser::element()
{
    // constrained element, for input()
    Element::Type type;
    bool constrained = false;
    if (peek("integer")) {
        consume("integer");
        consume(":");
        constrained = true;
        type = Element::Type::INT;
    } else if (peek("float")) {
        consume("float");
        consume(":");
        constrained = true;
        type = Element::Type::NUM;
    } else if (peek("string")) {
        consume("string");
        consume(":");
        constrained = true;
        type = Element::Type::STR;
    }
    if (constrained) {
        Element::Constraint constraint;
        if (peek('*')) {
            consume("*");
            constraint = Element::Constraint::ANY;
        } else if (peek('<')) {
            consume("<");
            constraint = Element::Constraint::LT;
        } else if (peek('>')) {
            consume(">");
            constraint = Element::Constraint::GT;
        } else {
            constraint = Element::Constraint::EQ;
        }
        Element *ret = new Element;
        ret->type = type;
        ret->constraint = constraint;
        std::pair<Element::Type, double> num_pair;
        try {
            if (constraint != Element::Constraint::ANY) {
                switch (type) {
                case Element::Type::INT:
                    ret->int_value = parse_int();
                    break;
                case Element::Type::NUM:
                    num_pair = parse_numeric();
                    ret->num_value = num_pair.second;
                    break;
                case Element::Type::STR:
                    ret->string_value = parse_string();
                    break;
                }
            }
        } catch (Parser::Exception *p) {
            delete ret;
            throw p;
        }
        return ret;
    }
    // concrete element, for output()
    if (peek('"')) {
        std::string str = parse_string();
        Element *ret = new Element;
        ret->type = Element::Type::STR;
        ret->string_value = str;
        return ret;
    }
    auto num_pair = parse_numeric();
    Element *ret = new Element;
    ret->type = num_pair.first;
    if (ret->type == Element::Type::INT) {
        ret->int_value = (int)num_pair.second;
    } else {
        ret->num_value = num_pair.second;
    }
    return ret;
}
