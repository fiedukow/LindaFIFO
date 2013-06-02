#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

#include <Server/Database.h>
#include <Server/Exceptions.h>
#include <Server/Common/Field.h>
#include <Server/Common/QueryStructures.h>

BOOST_AUTO_TEST_SUITE(Database_test)

using namespace Server;

struct DatabaseWithTwoTuples
{
  DatabaseWithTwoTuples()
  {
    createTuples();
    insertTuplesIntoDB();
  }

  Database database;
  std::vector<Common::Tuple> tuples;

  private:
    void createTuples()
    {
      tuples.push_back({
          new Common::IntField(5),
          new Common::StringField("foo")
        });
      tuples.push_back({
          new Common::StringField("bar"),
          new Common::IntField(1)
        });
    }

    void insertTuplesIntoDB()
    {
      for (const Common::Tuple& tuple : tuples)
      {
        Common::InsertDescription description(tuple);
        database.output(description);
      }
    }
};

BOOST_FIXTURE_TEST_CASE(InsertionIncreasesTupleCounter, DatabaseWithTwoTuples)
{
  BOOST_CHECK_EQUAL(database.getTuplesCount(),2);

  Common::Tuple tuple3 = {
      new Common::StringField("foo"),
      new Common::StringField("bar"),
      new Common::StringField("baz")
    };

  { // insert ("foo","bar","baz") into DB
    Common::InsertDescription description(tuple3);

    database.output(description);
  }

  BOOST_CHECK_EQUAL(database.getTuplesCount(),3);
}

BOOST_FIXTURE_TEST_CASE(InputWithMatchDecreasesTupleCounter, DatabaseWithTwoTuples)
{
  int tuplesCount = database.getTuplesCount();
  { 
    Common::SelectDescription::FieldConditions conditions = {
        new Common::IntFieldCondition(0,Common::FieldCondition::Any),
        new Common::StringFieldCondition("foo",Common::FieldCondition::Equal)
      };

    Common::SelectDescription description(conditions);
    BOOST_REQUIRE_NO_THROW(database.input(description)); // proper tuple returned
  }

  BOOST_CHECK_EQUAL(database.getTuplesCount(),tuplesCount-1);
}

BOOST_FIXTURE_TEST_CASE(ReadWithMatchDoesNotDecreaseTupleCounter, DatabaseWithTwoTuples)
{
  int tuplesCount = database.getTuplesCount();

  Common::SelectDescription::FieldConditions conditions = {
      new Common::IntFieldCondition(0,Common::FieldCondition::Greater),
      new Common::StringFieldCondition("",Common::FieldCondition::Any)
    };

  Common::SelectDescription description(conditions);
  BOOST_REQUIRE_NO_THROW(database.read(description)); // asserts tuple was found

  BOOST_CHECK_EQUAL(database.getTuplesCount(),tuplesCount);
}

BOOST_FIXTURE_TEST_CASE(InputByOneConcreteConditionAndOthersAnyGivesRightTuple, DatabaseWithTwoTuples)
{
  Common::SelectDescription::FieldConditions conditions = {
      new Common::IntFieldCondition(0,Common::FieldCondition::Any),
      new Common::StringFieldCondition("foo",Common::FieldCondition::Equal)
    };

  Common::SelectDescription description(conditions);
  Common::Tuple tuple = database.input(description);
  BOOST_CHECK(tuple == tuples[0]);
}

BOOST_FIXTURE_TEST_CASE(InputNonExistingTupleThrowsAndDoesNotChangeDBState, DatabaseWithTwoTuples)
{
  int tuplesCount = database.getTuplesCount();
  { 
    Common::SelectDescription::FieldConditions conditions = {
        new Common::IntFieldCondition(1,Common::FieldCondition::Less),
        new Common::StringFieldCondition("",Common::FieldCondition::Any),
      };

    Common::SelectDescription description(conditions);
    BOOST_CHECK_THROW(database.input(description),Exceptions::TupleDoesNotExistException); // asserts tuple was not found
  }
  BOOST_REQUIRE_EQUAL(database.getTuplesCount(),tuplesCount);
}

BOOST_FIXTURE_TEST_CASE(InputWithNotAllParametersSpecifiedInConditionDoesNotFindTuple, DatabaseWithTwoTuples)
{
  int tuplesCount = database.getTuplesCount();
  {
    Common::SelectDescription::FieldConditions conditions = {
        new Common::IntFieldCondition(2,Common::FieldCondition::Less),
      }; // there is a tuple with value < 2, but it also contains string

    Common::SelectDescription description(conditions);
    BOOST_CHECK_THROW(database.input(description),Exceptions::TupleDoesNotExistException);
  }

  BOOST_REQUIRE_EQUAL(database.getTuplesCount(),tuplesCount);
}

BOOST_AUTO_TEST_SUITE_END()

