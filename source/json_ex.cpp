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
 * Global Declaration to store all the initial data regarding the city 
 */

  float currentTemp;
  float maximumTemp;
  float minimumTemp;
  float averageTemp;
  float humid;
  float rain;

  /* time variables to create random user id to each users */
  time_t t = time(NULL);
  tm* timePtr = localtime(&t);



/* 
* function to store the data  the data gloabally to use in further classes 
*/

void  statusForDays(string myCity)
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
		   usr_details.open("data.json");
		    if(!usr_details){
		    cout << "Unable to open file" << endl;
		    exit(1);
		    }
		    string myUrl = "http://dataservice.accuweather.com/locations/v1/cities/search?apikey=eHmyUvaR0JwzYpejDI9AOXq5mBVGMAzI&q=";
		    myUrl.insert(104, myCity);
		    usr_details<<curlpp::options::Url(myUrl)<<endl;
		    usr_details.close();

		    std::ifstream i("data.json");
		    json j = json::parse(i);
		    string s = j[0]["Key"];
		    
		    string test = "http://dataservice.accuweather.com/currentconditions/v1/?apikey=eHmyUvaR0JwzYpejDI9AOXq5mBVGMAzI&details=true";
		    test.insert(56, s);

		    
		    city_details.open("cities.json");
		    if(!city_details){
		    cout << "Unable to open file" << endl;
		    exit(1);
		    }
		    city_details<<curlpp::options::Url(test)<<std::endl;
		    city_details.close();

		    std::ifstream ifs("cities.json");
		    json j2 = json::parse(ifs);
		    currentTemp = j2[0]["ApparentTemperature"]["Metric"]["Value"];
		    maximumTemp = j2[0]["TemperatureSummary"]["Past6HourRange"]["Maximum"]["Metric"]["Value"];
		    minimumTemp = j2[0]["TemperatureSummary"]["Past6HourRange"]["Minimum"]["Metric"]["Value"];
		    averageTemp = j2[0]["TemperatureSummary"]["Past12HourRange"]["Minimum"]["Metric"]["Value"];
		    humid = j2[0]["RelativeHumidity"];
		    rain = j2[0]["PrecipitationSummary"]["Past12Hours"]["Metric"]["Value"];
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
    

}

/*Abstract Forecast Class */
class Forecast{
public:
  float currentTemprature = currentTemp;
  float maximumTemprature = maximumTemp;
  float minimumTemprature = minimumTemp;
  float averageTemprature = averageTemp;
  float humidity = humid;
  float rainfall = rain;
  string summary;

  virtual void getAvgTemp() = 0;
  virtual void getAvgRain() = 0;
  virtual void showStats() = 0;
  virtual void predict() = 0;

};

/*Day Forecast Class */
class DayForecast:public Forecast
{
private:
  int date = timePtr->tm_mday ;
public:
  void getAvgTemp () {
    cout<<"Average Temprature of the day is  :"<<this->averageTemprature<<endl;
  }
  void getAvgRain () {
    cout<<"Average Temprature of the day is  :"<<this->rainfall<<endl;
  }
  void showStats() {
        cout<<"Today is :"<<this->date<<"th Of April"<<endl;
        cout<<"Current Temprature : "<<this->currentTemprature<<"\n";
		    cout<<"Maximum Temprature : "<<this->maximumTemprature<<endl;
		    cout<<"Minimum Temprature : "<<this->minimumTemprature<<'\n';
		    cout<<"Average Temprature : "<<this->averageTemprature<<endl;
		    cout<<"Humidity : "<<this->humidity<<'\n';
		    cout<<"Rainfall in Past 12 hours : "<<this->rainfall<<endl;
  }
  void predict() {
    cout<<this->summary<<endl;
  }
};


/* Abstract State class */
class State
{
public:
  int lattitude;
  int longitude;

  virtual void getCoordinates(int, int) = 0;
  virtual void getCity(string) = 0;
  virtual void assignForecast(Forecast *F) = 0;
};

/*City class will inherit the State class */
class City: public State
{
private:
  string city_name;
public:
 	Forecast *DailyForecast;
  /* City not found exception */
  class CityNotFoundException
  {
  public:
    int errCode;
    string message;

    CityNotFoundException()
    {
      errCode = 404;
      message = "The city entered cannot be found !";
    }
  };

  void getCity(string city)
  {
      int flag = 0;
      ifstream inFile;
      string line;

      inFile.open("cities.txt");

      if(!inFile){
      cout << "Unable to open file" << endl;
      exit(1);
      }

      size_t pos;
      while(inFile.good())
        {
            getline(inFile,line); // get line from file
            pos=line.find(city); // search
            if(pos!=string::npos) // string::npos is returned if string is not found
              {
                  flag = 1;
                  break;
              }
            else
              continue;
        }

      if(flag == 1) {
      	statusForDays(city);
      	this->city_name = city;
      }
        
      else
          throw CityNotFoundException();

  }

  void assignForecast(Forecast *F) {
  	this->DailyForecast = F;
  }

  void showStatus()
  {
    cout<<"City Name : "<<this->city_name<<endl;
    DailyForecast->showStats();
  }

  void getCoordinates(int lat_val, int long_val)
  {
      this->lattitude = lat_val;
      this->longitude = long_val;
  }

};




/* User Details for no of active users using the software and further improvemnet */

class User
{
private:

  int unique_usr_id;
  string usr_name;
  string usr_city;
  fstream usr_details;

  /* Private Class to write user's data into the file */

  void writeIntoFile()
  {
    usr_details.open("users.txt", std::ios_base::app);
    if(!usr_details){
    cout << "Unable to open file" << endl;
    exit(1);
    }
    usr_details << this->unique_usr_id <<endl;
    usr_details << this->usr_name <<endl;
    usr_details << this->usr_city <<endl;
    usr_details.close();
  }

public:

  void getDetails()
  {
    srand(time(0)); //Seeding the random at time 0.
    this->unique_usr_id = rand() % 9000 + 1000; //Providing each user with random 4 digit unique id.
    cout<<"Enter Your Name : "<<endl;
    cin>>this->usr_name;
    cout<<"Enter Your City : "<<endl;
    cin>>this->usr_city;

    /* Write the aquired data into a file 'users.txt' */
    writeIntoFile();

  }

  void showDetails()
  {
    cout<<"Unique Id : "<<this->unique_usr_id<<endl;
    cout<<"Name : "<<this->usr_name<<endl;
    cout<<"City : "<<this->usr_city<<endl;
  }

};

int main()
{
	
    	User usr;
			City cty;
			string desired_city;
			usr.getDetails();
			cout<<"Welcome to cyWeather, Enter the city which you want to search ?"<<endl;
	    cin>>desired_city;
	    try {
	      cty.getCity(desired_city);
	    }
	    catch(City::CityNotFoundException ce) {
	      cout<<"An Error Occurred"<<endl;
	      cout<<"Error Code : "<<ce.errCode<<endl;
	      cout<<"Message : "<<ce.message<<endl;
	      exit(1);
	    }

	    Forecast *F = new DayForecast;
	    cty.assignForecast(F);
	    cty.showStatus();
	  	


    return 0;
}