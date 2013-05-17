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
    }
    delete op;
    return ret;
}

bool throws_ok(const char *s) {
    Parser p(s);
    Operation *op = nullptr;
    bool ret = true;
    try {
        op = p.TOP();
        ret = false;
    } catch (Parser::Exception *p) {
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
}

BOOST_AUTO_TEST_SUITE_END()
