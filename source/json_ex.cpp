#include <string>
#include <sstream>
#include <iostream>
#include <fstream>

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace std;

namespace
{
const long MyPort = 80;
}

/** 
 * This example is made to show you how you can use the Options.
 */
int main(int, char **)
{
	try
	{
		curlpp::Cleanup myCleanup;
		fstream usr_details;
		fstream city_details;

		// First easy example.
		{
		  // The first easiest example is to retreive the content of
		  // a web page and put it in a stream.

		  //std::cout<<curlpp::options::Url("http://dataservice.accuweather.com/locations/v1/cities/search?apikey=eHmyUvaR0JwzYpejDI9AOXq5mBVGMAzI&q=Gadag")<<endl;

		 //Writing the json response to a json file
		   // usr_details.open("data.json");
		   //  if(!usr_details){
		   //  cout << "Unable to open file" << endl;
		   //  exit(1);
		   //  }
		   //  //usr_details<<curlpp::options::Url("http://dataservice.accuweather.com/locations/v1/cities/search?apikey=eHmyUvaR0JwzYpejDI9AOXq5mBVGMAzI&q=Hubli")<<endl;
		   //  usr_details.close();

		    std::ifstream i("data.json");
		    json j = json::parse(i);
		    string s = j[0]["Key"];
		    //std::cout<<"Unique City Code "<<s<<std::endl;
		    string test = "http://dataservice.accuweather.com/currentconditions/v1/?apikey=eHmyUvaR0JwzYpejDI9AOXq5mBVGMAzI&details=true";
		    test.insert(56, s);

		    //std::cout<<curlpp::options::Url(test)<<std::endl;
		    // city_details.open("cities.json");
		    // if(!city_details){
		    // cout << "Unable to open file" << endl;
		    // exit(1);
		    // }
		    // city_details<<curlpp::options::Url(test)<<std::endl;
		    // city_details.close();

		    std::ifstream ifs("cities.json");
		    json j2 = json::parse(ifs);
		    cout<<"Current Temprature : "<<std::setw(4)<<j2[0]["ApparentTemperature"]["Metric"]["Value"]<<endl;
		    cout<<"Maximum Temprature : "<<std::setw(4)<<j2[0]["TemperatureSummary"]["Past6HourRange"]["Maximum"]["Metric"]["Value"]<<endl;
		    cout<<"Minimum Temprature : "<<std::setw(4)<<j2[0]["TemperatureSummary"]["Past6HourRange"]["Minimum"]["Metric"]["Value"]<<endl;
		    cout<<"Average Temprature : "<<std::setw(4)<<j2[0]["TemperatureSummary"]["Past12HourRange"]["Minimum"]["Metric"]["Value"]<<endl;
		    cout<<"Humidity : "<<std::setw(4)<<j2[0]["RelativeHumidity"]<<endl;
		    cout<<"Rainfall in Past 12 hours : "<<std::setw(4)<<j2[0]["PrecipitationSummary"]["Past12Hours"]["Metric"]["Value"]<<endl;

		    
			
			

		}

	}

	catch( curlpp::RuntimeError &e )
	{
		std::cout << e.what() << std::endl;
	}

	catch( curlpp::LogicError &e )
	{
		std::cout << e.what() << std::endl;
	}
    
  return 0;
}