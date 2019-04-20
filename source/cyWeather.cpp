#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <time.h>
using namespace std;


/* Abstract State class */
class State
{
public:
  int lattitude;
  int longitude;

   void getCoordinates();
};

/*City class will inherit the State class */
class City: public State
{
private:
  string city_name;
public:

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

      if(flag == 1)
          this->city_name = city;
      else
          throw CityNotFoundException();

  }
  void showStatus()
  {
    cout<<"City Name : "<<this->city_name<<endl;
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
    cty.showStatus();


    return 0;
}
