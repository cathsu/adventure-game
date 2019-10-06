#include "Object.h"; 

Object::Object()
{
	name_ = "Object"; 
	description_ = "The object lies in the room"; 
	mobility_ = true; 
	//returned_ = true; 
	
}

Object::Object(string name, string description, bool mobility) :


	name_(name),
	description_(description),
	mobility_(mobility) 
	//returned_(returned)

{}


//getters
string Object::getName()
{
	return name_; 
}

string Object::getDescription()
{
	return description_; 
}

bool Object::getMobility()
{
	return mobility_; 
}

/*
bool Object::getReturned()
{
	return returned_; 
}
*/
//setters
void Object::setName(string name)
{
	name_ = name; 
}

void Object::setDescription(string description)
{
	description_ = description; 
}

void Object::setMobility(bool mobility)
{
	mobility_ = mobility; 
}

/*
void Object::setReturned(bool returned)
{
	returned_ = returned; 
}
*/