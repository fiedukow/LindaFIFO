#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

#include <Server/Database.h>
#include <Server/Common/Field.h>
#include <Server/Common/QueryStructures.h>

BOOST_AUTO_TEST_SUITE( Database_test )

BOOST_AUTO_TEST_CASE( Database_insertion_test )
{
  using namespace Server;
  Database database;

  { // insert 5,"foo" to DB
    Common::Tuple tuple = {
        new Common::IntField(5),
        new Common::StringField("foo")
      };

    Common::InsertDescription description(tuple);

    database.output(description);
  }

  BOOST_REQUIRE_EQUAL(database.getTuplesCount(),1);
}

BOOST_AUTO_TEST_SUITE_END()

