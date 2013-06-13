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

			Linda::output("output(111.1)");
			std::string s1 = Linda::input("input(float:>100.0, 3)");
			std::cout << s1 << std::endl;
			BOOST_CHECK(s1.compare("(111.1)")==0);

			Linda::output("output(\"123\")");
			std::string s2 = Linda::input("input(string:\"123\", 3)");
			std::cout << s2 << std::endl;
			BOOST_CHECK(s2.compare("(\"123\")")==0);


			/*BOOST_CHECK(Linda::read("read(integer:<20, 3)") == "(12)");
      			BOOST_CHECK(Linda::input("input(integer:*, 3)") == "(12)");
      			if(forked == 0)
      			{
        			for(int i = 0; i < 2000; ++i) Linda::output("output(12)");
        			while (true) 
				{
					sleep(10);
				}		
      			} else
      			{
        			try
       				{
        				for(int i = 0; i < 2000; ++i)
          					BOOST_CHECK(Linda::input("input(integer:*, 3)") == "(12)");
        			}
        			catch(std::exception& e)
        			{
          				std::cout << e.what() << std::endl;
       				}
        			catch(...)
        			{
					std::cout << "damn" << std::endl;
       				}
      			}
			Linda::output("output(12)");
      			BOOST_CHECK(Linda::read("read(integer:<20, 3)") == "(12)");
      			BOOST_CHECK(Linda::input("input(integer:*, 3)") == "(12)");*/
		} 
		catch (std::exception& e)  
		{	
			std::cout << e.what() << std::endl;
			//expected if the query is invalid. It looks valid anyway. 
			BOOST_CHECK(false);
		}	
		catch (...)  
		{	
			////printf ("O kurdele uber fail! \n");
			//totally unexpected. 
			kill(server, SIGTERM);
			BOOST_CHECK(false);
			
		}	
		//Linda::input();
		kill(server, SIGTERM);
		//printf ("O kurdele udalo sie222!!! \n");
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
			int forked=fork();
      			if(forked == 0)
      			{
        			for(int i = 0; i < 2000; ++i) Linda::output("output(12)");
        			while (true) 
				{
					sleep(10);
				}		
      			} else
      			{
        			try
       				{
        				for(int i = 0; i < 2000; ++i)
          					BOOST_CHECK(Linda::input("input(integer:*, 3)") == "(12)");
        			}
        			catch(std::exception& e)
        			{
          				std::cout << e.what() << std::endl;
       				}
        			catch(...)
        			{
					std::cout << "damn" << std::endl;
       				}
      			}
			Linda::output("output(12)");
      			BOOST_CHECK(Linda::read("read(integer:<20, 3)") == "(12)");
      			BOOST_CHECK(Linda::input("input(integer:*, 3)") == "(12)");
			kill(forked, SIGTERM);
		} 
		catch (std::exception& e)  
		{	
			std::cout << e.what() << std::endl;
			//expected if the query is invalid. It looks valid anyway. 
			BOOST_CHECK(false);
		}	
		catch (...)  
		{	
			////printf ("O kurdele uber fail! \n");
			//totally unexpected. 
			kill(server, SIGTERM);
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
