#ifndef Object_H
#define Object_H

#include<string>
#include<ciso646>  
#include<vector>
#include<iostream>
#include <istream>

using namespace std;
class Room; 
class Object
{
public: 
	//constructors
	Object(); 
	Object(string name, string description, bool mobility); 


	//getters
	string getName(); 
	string getDescription(); 
	bool getMobility(); 
	//bool getReturned(); 

	//setters
	void setName(string name); 
	void setDescription(string descr); 
	void setMobility(bool mobility); 
	//void setReturned(bool returned); 
	
	

private:
	bool returned_; 
	bool mobility_; 
	string name_; 
	string description_; 
	Room* home_;
};
#endif