#include "vectorObject.h"

void vectorObject::deleteVectorContent(int start, int stop)
{
	for (int k = start; k < stop; k++)
		delete objVec_[k]; 

}

void vectorObject::addObject(Object* object)
{
	objVec_.push_back(object); 
}