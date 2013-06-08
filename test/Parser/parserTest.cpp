#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <Parser/Parser.hpp>
#include <iostream>

Parser parser;

bool parses_ok(const char *s) {
    Operation *op = nullptr;
    bool ret = false;
    try {
        op = parser.parse(s);
        ret = true;
    } catch (Parser::Exception *p) {
        std::cerr << "Test failed - " << s << ": " << p->what() << std::endl;
        delete p;
    }
    delete op;
    return ret;
}

bool throws_ok(const char *s) {
    Operation *op = nullptr;
    bool ret = true;
    try {
        op = parser.parse(s);
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
    BOOST_CHECK(parses_ok("input(float:*,int:*)"));
    BOOST_CHECK(parses_ok("output(5.0)"));
    BOOST_CHECK(parses_ok("output(4.34)"));
    BOOST_CHECK(parses_ok("read(2)"));
    BOOST_CHECK(parses_ok("input(string:==\"foo\")"));
}

BOOST_AUTO_TEST_CASE( Parser_AST )
{
    Operation *op;
    op = parser.parse("input(5)");
    BOOST_CHECK(op->arity() == 1);
    BOOST_CHECK(op->element(0).type == Element::Type::INT);
    BOOST_CHECK(op->element(0).int_value == 5);
    delete op;

    op = parser.parse("input(55)");
    BOOST_CHECK(op->arity() == 1);
    BOOST_CHECK(op->element(0).int_value == 55);
    delete op;

    op = parser.parse("input(55,5)");
    BOOST_CHECK(op->arity() == 2);
    BOOST_CHECK(op->element(0).int_value == 55);
    BOOST_CHECK(op->element(1).int_value == 5);
    delete op;

    op = parser.parse("input(55, 5)");
    BOOST_CHECK(op->arity() == 2);
    BOOST_CHECK(op->element(0).int_value == 55);
    BOOST_CHECK(op->element(1).int_value == 5);
    delete op;

    op = parser.parse("input(55 , 5)");
    BOOST_CHECK(op->arity() == 2);
    BOOST_CHECK(op->element(0).int_value == 55);
    BOOST_CHECK(op->element(1).int_value == 5);
    delete op;

    op = parser.parse("input(3.14)");
    BOOST_CHECK(op->arity() == 1);
    BOOST_CHECK(op->element(0).type == Element::Type::NUM);
    BOOST_CHECK(op->element(0).num_value == 3.14);
    delete op;

    op = parser.parse("output(5.0)");
    BOOST_CHECK(op->arity() == 1);
    BOOST_CHECK(op->element(0).type == Element::Type::NUM);
    BOOST_CHECK(op->element(0).num_value == 5.0);
    delete op;

    op = parser.parse("input(3.014)");
    BOOST_CHECK(op->arity() == 1);
    BOOST_CHECK(op->element(0).num_value == 3.014);
    delete op;

    op = parser.parse("input(\"\")");
    BOOST_CHECK(op->arity() == 1);
    BOOST_CHECK(op->element(0).type == Element::Type::STR);
    BOOST_CHECK(op->element(0).str_value == "");
    delete op;

    op = parser.parse("input(\"chuj\")");
    BOOST_CHECK(op->arity() == 1);
    BOOST_CHECK(op->element(0).type == Element::Type::STR);
    BOOST_CHECK(op->element(0).str_value == "chuj");
    delete op;

    op = parser.parse("input(\"foo\", \"bar\", \"baz\")");
    BOOST_CHECK(op->arity() == 3);
    BOOST_CHECK(op->element(0).type == Element::Type::STR);
    BOOST_CHECK(op->element(0).str_value == "foo");
    BOOST_CHECK(op->element(1).type == Element::Type::STR);
    BOOST_CHECK(op->element(1).str_value == "bar");
    BOOST_CHECK(op->element(2).type == Element::Type::STR);
    BOOST_CHECK(op->element(2).str_value == "baz");
    delete op;

    op = parser.parse("input(\"fo\\\"o\")");
    BOOST_CHECK(op->arity() == 1);
    BOOST_CHECK(op->element(0).type == Element::Type::STR);
    BOOST_CHECK(op->element(0).str_value == "fo\"o");
    delete op;

    op = parser.parse("input(integer:5)");
    BOOST_CHECK(op->arity() == 1);
    BOOST_CHECK(op->element(0).type == Element::Type::INT);
    BOOST_CHECK(op->element(0).constraint == Element::Constraint::EQ);
    BOOST_CHECK(op->element(0).int_value == 5);
    delete op;

    op = parser.parse("input(integer:*)");
    BOOST_CHECK(op->arity() == 1);
    BOOST_CHECK(op->element(0).type == Element::Type::INT);
    BOOST_CHECK(op->element(0).constraint == Element::Constraint::ANY);
    delete op;

    op = parser.parse("input(integer:>3)");
    BOOST_CHECK(op->arity() == 1);
    BOOST_CHECK(op->element(0).type == Element::Type::INT);
    BOOST_CHECK(op->element(0).constraint == Element::Constraint::GT);
    BOOST_CHECK(op->element(0).int_value == 3);
    delete op;

    op = parser.parse("input(integer:<3)");
    BOOST_CHECK(op->arity() == 1);
    BOOST_CHECK(op->element(0).type == Element::Type::INT);
    BOOST_CHECK(op->element(0).constraint == Element::Constraint::LT);
    BOOST_CHECK(op->element(0).int_value == 3);
    delete op;

    op = parser.parse("input(string:*)");
    BOOST_CHECK(op->arity() == 1);
    BOOST_CHECK(op->element(0).type == Element::Type::STR);
    BOOST_CHECK(op->element(0).constraint == Element::Constraint::ANY);
    delete op;

    op = parser.parse("input(string:\"foo\")");
    BOOST_CHECK(op->arity() == 1);
    BOOST_CHECK(op->element(0).constraint == Element::Constraint::EQ);
    BOOST_CHECK(op->element(0).str_value == "foo");
    delete op;

    op = parser.parse("input(float:>3.5)");
    BOOST_CHECK(op->arity() == 1);
    BOOST_CHECK(op->element(0).type == Element::Type::NUM);
    BOOST_CHECK(op->element(0).constraint == Element::Constraint::GT);
    BOOST_CHECK(op->element(0).num_value == 3.5);
    delete op;

    op = parser.parse("input(float:3.5)");
    BOOST_CHECK(op->arity() == 1);
    BOOST_CHECK(op->element(0).type == Element::Type::NUM);
    BOOST_CHECK(op->element(0).constraint == Element::Constraint::EQ);
    delete op;

    op = parser.parse("input(float:<3.5)");
    BOOST_CHECK(op->arity() == 1);
    BOOST_CHECK(op->element(0).type == Element::Type::NUM);
    BOOST_CHECK(op->element(0).constraint == Element::Constraint::LT);
    delete op;

    op = parser.parse("input(float:<=3.5)");
    BOOST_CHECK(op->arity() == 1);
    BOOST_CHECK(op->element(0).type == Element::Type::NUM);
    BOOST_CHECK(op->element(0).constraint == Element::Constraint::LEQ);
    delete op;

    op = parser.parse("input(float:>=3.5)");
    BOOST_CHECK(op->arity() == 1);
    BOOST_CHECK(op->element(0).type == Element::Type::NUM);
    BOOST_CHECK(op->element(0).constraint == Element::Constraint::GEQ);
    delete op;

    op = parser.parse("input(string:==\"foo\")");
    BOOST_CHECK(op->arity() == 1);
    BOOST_CHECK(op->element(0).type == Element::Type::STR);
    BOOST_CHECK(op->element(0).constraint == Element::Constraint::EQ);
    BOOST_CHECK(op->element(0).str_value == "foo");
    delete op;

    op = parser.parse("input(float:*)");
    BOOST_CHECK(op->arity() == 1);
    BOOST_CHECK(op->element(0).type == Element::Type::NUM);
    BOOST_CHECK(op->element(0).constraint == Element::Constraint::ANY);
    delete op;

    op = parser.parse("read(2)");
    BOOST_CHECK(op->arity() == 1);
    BOOST_CHECK(op->element(0).type == Element::Type::INT);
    BOOST_CHECK(op->element(0).constraint == Element::Constraint::EQ);
    BOOST_CHECK(op->element(0).int_value == 2);
    delete op;

    op = parser.parse("read(3)");
    BOOST_CHECK(op->arity() == 1);
    BOOST_CHECK(op->element(0).type == Element::Type::INT);
    BOOST_CHECK(op->element(0).constraint == Element::Constraint::EQ);
    BOOST_CHECK(op->element(0).int_value == 3);
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
