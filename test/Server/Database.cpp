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

  Common::Tuple tuple1 = {
      new Common::IntField(5),
      new Common::StringField("foo")
    };

  { // insert 5,"foo" to DB
    Common::InsertDescription description(tuple1);

    database.output(description);
  }

  BOOST_REQUIRE_EQUAL(database.getTuplesCount(),1);

  Common::Tuple tuple2 = {
      new Common::IntField(1),
      new Common::StringField("bar")
    };

  { // insert 1,"bar" to DB
    Common::InsertDescription description(tuple2);

    database.output(description);
  }

  BOOST_REQUIRE_EQUAL(database.getTuplesCount(),2);

  { 
    Common::SelectDescription::FieldConditions conditions = {
        new Common::IntFieldCondition(5,Common::FieldCondition::Equal),
        new Common::StringFieldCondition("",Common::FieldCondition::Any)
      };

    Common::SelectDescription description(conditions);
    Common::Tuple tuple = database.read(description);
    BOOST_CHECK(tuple == tuple1);
  }
}

BOOST_AUTO_TEST_SUITE_END()

