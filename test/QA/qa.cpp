#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <ClientAPI/API.h>
#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <exception>
#include <string>

BOOST_AUTO_TEST_SUITE( QA_test )

BOOST_AUTO_TEST_CASE( Tests_simple )
{
	pid_t server=fork();
	BOOST_REQUIRE(server!=-1);
	if (server==0) 
	{
		int  fd = open("/dev/null", O_WRONLY);
		if (fd>=0) 
		{
			dup2(fd, 1);
			dup2(fd, 2);
	    		close(fd);
			// not much we can do otherwise
		} else BOOST_REQUIRE(false);
		
		execl("./bin/Server", (char *) 0);	
	} else 
	{	
		// "give me some breath" - your server.
		printf ("Starting Queries in 3... \n");
		sleep(1);
		printf ("2... \n");
		sleep(1);
		printf ("1... \n");
		sleep(1);
		try 
		{
			Linda::output("output(111)");
			BOOST_CHECK(Linda::input("input(integer:==111, 3)").compare("(111)")==0);

			Linda::output("output(\"123\")");
			std::string s2 = Linda::input("input(string:\"123\", 3)");
//    BOOST_CHECK_THROW(Linda::input("input(string:*,1)"), Linda::QueryTimeoutException);
//		std::cout << s2 << std::endl;
			BOOST_CHECK(s2.compare("(\"123\")")==0);


			Linda::output("output(23)");
			BOOST_CHECK(Linda::read("read(integer:==23, 3)").compare("(23)")==0);
			BOOST_CHECK(Linda::read("read(integer:==23, 3)").compare("(24)")!=0);
			BOOST_CHECK(Linda::input("input(integer:==23, 3)").compare("(23)")==0);


			Linda::output("output(1,2,3)");
      //std::cout << Linda::read("read(integer:==1,integer:==2,integer:==3, 3)") << std::endl;
			BOOST_CHECK(Linda::read("read(integer:==1,integer:==2,integer:==3, 3)").compare("(1, 2, 3)")==0);
			BOOST_CHECK(Linda::read("read(integer:>=1,integer:<=2,integer:>=3, 3)").compare("(1, 2, 3)")==0);
			try 
			{
				BOOST_CHECK(Linda::read("I am invalid query").compare("(1, 2, 3)")!=0);	
				BOOST_CHECK(false);
			}			
			catch(Linda::InvalidQueryException& e) 
			{
			}
			catch (...)
			{
				BOOST_CHECK(false);
			}
			BOOST_CHECK(Linda::read("read(integer:>=1,integer:<=2,integer:>=3, 3)").compare("(1, 2, 3)")==0);
			BOOST_CHECK(Linda::read("read(integer:*,integer:*,integer:*, 3)").compare("(1, 2, 3)")==0);

			Linda::output("output(1,\"123\")");
			BOOST_CHECK(Linda::input("input(integer:*,integer:*,integer:*, 3)").compare("(1, 2, 3)")==0);
			BOOST_CHECK(Linda::input("input(integer:*,string:*, 3)").compare("(1, \"123\")")==0);
			try 
			{
				BOOST_CHECK(Linda::read("input(integer:*,string:*, 3)").compare("(1, 2, 3)")!=0);	
				BOOST_CHECK(false);
			}			
			catch(Linda::InvalidQueryTypeException& e) 
			{
			}
			catch (...)
			{
				BOOST_CHECK(false);
			}

		} 
		catch (std::exception& e)  
		{	
			std::cout << e.what() << std::endl;
			//expected if the query is invalid. It looks valid anyway. 
			BOOST_CHECK(false);
		}	
		catch (...)  
		{	
			//totally unexpected. 
			kill(server, SIGTERM);
			BOOST_CHECK(false);
			
		}	
		//Linda::input();
		kill(server, SIGTERM);
		std::cout << "Killed server no 1" << std::endl;
	}
}



BOOST_AUTO_TEST_CASE( Tests )
{
	pid_t server=fork();
	BOOST_REQUIRE(server!=-1);
	if (server==0) 
	{
		int  fd = open("/dev/null", O_WRONLY);
		if (fd>=0) 
		{
			dup2(fd, 1);
			dup2(fd, 2);
	    		close(fd);
			// not much we can do otherwise
		} else BOOST_REQUIRE(false);
		
		execl("./bin/Server", (char *) 0);	
	} else 
	{	
		// "give me some breath" - your server.
		printf ("Starting Queries in 3... \n");
		sleep(1);
		printf ("2... \n");
		sleep(1);
		printf ("1... \n");
		sleep(1);
		try 
		{
			Linda::output("output(100,200,300)");
			BOOST_CHECK(Linda::input("input(integer:<500,integer:==200,integer:>0, 3)").compare("(100, 200, 300)")==0);
		} 
		catch (std::exception& e)  
		{	
			std::cout << e.what() << std::endl;
			//expected if the query is invalid. It looks valid anyway. 
			BOOST_CHECK(false);
		}	
		catch (...)  
		{	
			BOOST_CHECK(false);
			
		}	
		//Linda::input();
		kill(server, SIGTERM);
		//printf ("O kurdele udalo sie222!!! \n");
	}


//cCurrentPath[sizeof(cCurrentPath) - 1] = '\0'; /* not really required */
//printf ("The current working directory is %s", cCurrentPath);
    /*BOOST_CHECK(parses_ok("input(5)"));
    BOOST_CHECK(parses_ok("input(55)"));
    BOOST_CHECK(parses_ok("input(55,5)"));
    BOOST_CHECK(parses_ok("input(55, 5)"));
    BOOST_CHECK(parses_ok("input(55 , 5)"));
    BOOST_CHECK(parses_ok("input(3.14)"));
    BOOST_CHECK(parses_ok("input(\"\")"));
    BOOST_CHECK(parses_ok("input(\"czosnek\")"));
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
    BOOST_CHECK(!parses_ok("read(2)"));*/
}

BOOST_AUTO_TEST_SUITE_END()
