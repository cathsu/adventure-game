#ifndef vectorObject_H
#define vectorObject_H

#include<vector>
//#include "Object.h"
using namespace std; 

class Object; 
class vectorObject
{
public: 
	void deleteVectorContent(int start, int stop); 
	void addObject(Object* object); 
private: 
	vector<Object*> objVec_; 
	Object* object_; 
};
#endif