#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <Parser/Parser.hpp>
#include <iostream>

bool parses_ok(const char *s) {
    Parser p(s);
    Operation *op = nullptr;
    bool ret = false;
    try {
        op = p.TOP();
        ret = true;
    } catch (Parser::Exception *p) {
        std::cerr << "Test failed - " << s << ": " << p->what() << std::endl;
        delete p;
    }
    delete op;
    return ret;
}

Operation *
parse(const char *s)
{
    Parser p(s);
    return p.TOP();
}

bool throws_ok(const char *s) {
    Parser p(s);
    Operation *op = nullptr;
    bool ret = true;
    try {
        op = p.TOP();
        ret = false;
    } catch (Parser::Exception *p) {
        delete p;
    }
    delete op;
    return ret;
}

BOOST_AUTO_TEST_SUITE( Parser_test )

BOOST_AUTO_TEST_CASE( Parser_ok_test )
{
    BOOST_CHECK(parses_ok("input(5)"));
    BOOST_CHECK(parses_ok("input(55)"));
    BOOST_CHECK(parses_ok("input(55,5)"));
    BOOST_CHECK(parses_ok("input(55, 5)"));
    BOOST_CHECK(parses_ok("input(55 , 5)"));
    BOOST_CHECK(parses_ok("input(3.14)"));
    BOOST_CHECK(parses_ok("input(\"\")"));
    BOOST_CHECK(parses_ok("input(\"chuj\")"));
    BOOST_CHECK(parses_ok("input(\"foo\", \"bar\", \"baz\")"));
    BOOST_CHECK(parses_ok("input(\"fo\\\"o\")"));
    BOOST_CHECK(parses_ok("input(integer:5)"));
    BOOST_CHECK(parses_ok("input(integer:*)"));
    BOOST_CHECK(parses_ok("input(integer:>3)"));
    BOOST_CHECK(parses_ok("input(integer:<3)"));
    BOOST_CHECK(parses_ok("input(string:*)"));
    BOOST_CHECK(parses_ok("input(string:\"foo\")"));
    BOOST_CHECK(parses_ok("input(float:>3.5)"));
    BOOST_CHECK(parses_ok("input(float:3.5)"));
    BOOST_CHECK(parses_ok("input(float:<3.5)"));
    BOOST_CHECK(parses_ok("input(float:*)"));
}

BOOST_AUTO_TEST_CASE( Parser_AST )
{
    Operation *op;
    op = parse("input(5)");
    BOOST_CHECK(op->arity() == 1);
    BOOST_CHECK(op->element(0).type == Element::Type::INT);
    BOOST_CHECK(op->element(0).int_value == 5);
    delete op;

    op = parse("input(55)");
    BOOST_CHECK(op->arity() == 1);
    BOOST_CHECK(op->element(0).int_value == 55);
    delete op;

    op = parse("input(55,5)");
    BOOST_CHECK(op->arity() == 2);
    BOOST_CHECK(op->element(0).int_value == 55);
    BOOST_CHECK(op->element(1).int_value == 5);
    delete op;

    op = parse("input(55, 5)");
    BOOST_CHECK(op->arity() == 2);
    BOOST_CHECK(op->element(0).int_value == 55);
    BOOST_CHECK(op->element(1).int_value == 5);
    delete op;

    op = parse("input(55 , 5)");
    BOOST_CHECK(op->arity() == 2);
    BOOST_CHECK(op->element(0).int_value == 55);
    BOOST_CHECK(op->element(1).int_value == 5);
    delete op;

    op = parse("input(3.14)");
    BOOST_CHECK(op->arity() == 1);
    BOOST_CHECK(op->element(0).type == Element::Type::NUM);
    BOOST_CHECK(op->element(0).num_value == 3.14);
    delete op;

    op = parse("input(3.014)");
    BOOST_CHECK(op->arity() == 1);
    //BOOST_CHECK(op->element(0).num_value == 3.014); FIXME
    delete op;

    op = parse("input(\"\")");
    BOOST_CHECK(op->arity() == 1);
    BOOST_CHECK(op->element(0).type == Element::Type::STR);
    BOOST_CHECK(op->element(0).str_value == "");
    delete op;

    op = parse("input(\"chuj\")");
    BOOST_CHECK(op->arity() == 1);
    BOOST_CHECK(op->element(0).type == Element::Type::STR);
    BOOST_CHECK(op->element(0).str_value == "chuj");
    delete op;

    op = parse("input(\"foo\", \"bar\", \"baz\")");
    BOOST_CHECK(op->arity() == 3);
    BOOST_CHECK(op->element(0).type == Element::Type::STR);
    BOOST_CHECK(op->element(0).str_value == "foo");
    BOOST_CHECK(op->element(1).type == Element::Type::STR);
    BOOST_CHECK(op->element(1).str_value == "bar");
    BOOST_CHECK(op->element(2).type == Element::Type::STR);
    BOOST_CHECK(op->element(2).str_value == "baz");
    delete op;

    op = parse("input(\"fo\\\"o\")");
    BOOST_CHECK(op->arity() == 1);
    BOOST_CHECK(op->element(0).type == Element::Type::STR);
    // BOOST_CHECK(op->element(0).str_value == "fo\"o"); FIXME -- \ not removed
    delete op;

    op = parse("input(integer:5)");
    BOOST_CHECK(op->arity() == 1);
    BOOST_CHECK(op->element(0).type == Element::Type::INT);
    BOOST_CHECK(op->element(0).constraint == Element::Constraint::EQ);
    BOOST_CHECK(op->element(0).int_value == 5);
    delete op;

    op = parse("input(integer:*)");
    BOOST_CHECK(op->arity() == 1);
    BOOST_CHECK(op->element(0).type == Element::Type::INT);
    BOOST_CHECK(op->element(0).constraint == Element::Constraint::ANY);
    delete op;

    op = parse("input(integer:>3)");
    BOOST_CHECK(op->arity() == 1);
    BOOST_CHECK(op->element(0).type == Element::Type::INT);
    BOOST_CHECK(op->element(0).constraint == Element::Constraint::GT);
    BOOST_CHECK(op->element(0).int_value == 3);
    delete op;

    op = parse("input(integer:<3)");
    BOOST_CHECK(op->arity() == 1);
    BOOST_CHECK(op->element(0).type == Element::Type::INT);
    BOOST_CHECK(op->element(0).constraint == Element::Constraint::LT);
    BOOST_CHECK(op->element(0).int_value == 3);
    delete op;

    op = parse("input(string:*)");
    BOOST_CHECK(op->arity() == 1);
    BOOST_CHECK(op->element(0).type == Element::Type::STR);
    BOOST_CHECK(op->element(0).constraint == Element::Constraint::ANY);
    delete op;

    op = parse("input(string:\"foo\")");
    BOOST_CHECK(op->arity() == 1);
    BOOST_CHECK(op->element(0).constraint == Element::Constraint::EQ);
    BOOST_CHECK(op->element(0).str_value == "foo");
    delete op;

    op = parse("input(float:>3.5)");
    BOOST_CHECK(op->arity() == 1);
    BOOST_CHECK(op->element(0).type == Element::Type::NUM);
    BOOST_CHECK(op->element(0).constraint == Element::Constraint::GT);
    //BOOST_CHECK(op->element(0).num_value == 3.5); FIXME
    delete op;

    op = parse("input(float:3.5)");
    BOOST_CHECK(op->arity() == 1);
    BOOST_CHECK(op->element(0).type == Element::Type::NUM);
    BOOST_CHECK(op->element(0).constraint == Element::Constraint::EQ);
    delete op;

    op = parse("input(float:<3.5)");
    BOOST_CHECK(op->arity() == 1);
    BOOST_CHECK(op->element(0).type == Element::Type::NUM);
    BOOST_CHECK(op->element(0).constraint == Element::Constraint::LT);
    delete op;

    op = parse("input(float:*)");
    BOOST_CHECK(op->arity() == 1);
    BOOST_CHECK(op->element(0).type == Element::Type::NUM);
    BOOST_CHECK(op->element(0).constraint == Element::Constraint::ANY);
    delete op;
}

BOOST_AUTO_TEST_CASE( Parser_fail_test )
{
    BOOST_CHECK(throws_ok("input"));
    BOOST_CHECK(throws_ok("input("));
    BOOST_CHECK(throws_ok("inputt"));
    BOOST_CHECK(throws_ok("input()"));
    BOOST_CHECK(throws_ok("input(5"));
    BOOST_CHECK(throws_ok("input(55,)"));
    BOOST_CHECK(throws_ok("input( , 5)"));
    BOOST_CHECK(throws_ok("input(3.)"));
    BOOST_CHECK(throws_ok("input(\")"));
    BOOST_CHECK(throws_ok("input(integer:)"));
    BOOST_CHECK(throws_ok("input(integer5)"));
    BOOST_CHECK(throws_ok("input(integer:>)"));
    BOOST_CHECK(throws_ok("input(integer:<)"));
}

BOOST_AUTO_TEST_SUITE_END()
