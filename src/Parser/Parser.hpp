#ifndef _PARSER_HPP_
#define _PARSER_HPP_

#include <string>
#include <vector>
// yes, I'm mixing C with C++
#include <cstring>
#include <cctype>

struct Element {
  enum Type { INT, NUM, STR };
  enum Constraint { ANY, GT, LT, EQ };
  Type type;
  Constraint constraint;
  int int_value;
  double num_value;
  std::string str_value;
};

struct Operation {
  enum Type { INPUT, OUTPUT, READ };
  Type type;
  std::vector<Element*>* elements = nullptr;

  Element& element(int n) { return *((*elements)[n]); }
  int arity() { return elements == nullptr ? 0 : elements->size(); }

  ~Operation()
  {
    if (elements != nullptr) //todo make this check unnecessary
      for (Element* element : *elements) 
        delete element;
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

  void die(std::string m);
  void die(const char *m) { die(std::string(m)); }

  Operation * parse(const char *s) { source = s; cur = 0; return TOP(); }
  void consume(const char *);
  bool peek(char c) { return source[cur] == c; }
  bool peek(const char *s) { return !strncmp(s, source + cur, strlen(s)); }
  void skipws() { while (isblank(source[cur])) cur++; }
  Operation* TOP();
  Operation::Type operation();
  std::vector<Element*>* elements();
  Element * element();
  std::string parse_string();
  std::pair<Element::Type, double> parse_numeric();
  int parse_int();
};

#endif
