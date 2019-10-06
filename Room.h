// Room.h: interface for the Room class.
//
//////////////////////////////////////////////////////////////////////

#ifndef ROOM_H
#define ROOM_H

#include<string>
#include<ciso646>  // http://www.cplusplus.com/reference/clibrary/ciso646/
#include<vector>
#include<iostream>
#include "Object.h"
#include "vectorObject.h"
using namespace std;

enum Direction {NORTH = 0, EAST = 1, SOUTH = 2, WEST = 3};

class Room
{
public:
	// constructors
	Room(string name, string desc);
	Room();

	// exit pointers
	Room * west() const;
	Room * east() const;
	Room * south() const;
	Room * north() const;

	// inspectors for other attributes
	string description() const;
	string name() const;
	vector<Object*>& objects(); 
	int numObjects(); 
	Object* getObject(int index); 
	Object* takeObject(int index); 
	//vectorObject* objects(); 
	 
    // mutators
	void set_description(string d);
	void set_name(string n);
	void add_object(Object* obj);
	//void set_object(Object o); 

	// connect this room to room r
	// exit is the direction you exit this
	// room from. to is the direction you enter
	// the other room from.
	// Theres no law that says the exits have to make sense
	// The method returns true if the connection is successful
	// To prevent memory leaks, you cannot disconnect an exiting
	// attached exit.
	// Examples:
	//    r1.connect(NORTH, &r2, SOUTH);
	//       -- exiting r1 to the north takes you to r2
	//       -- exiting r2 to the south takes you to r1
	//       -- returns true
	//    r1.connect(WEST, &r3, WEST); (extra credit only)
	//       -- exiting r1 to the west takes you to r2
	//       -- exiting r3 to the west takes you to r1
	//       -- odd but permissible
	//       -- returns true
	//    r1.connect(WEST, &r4, EAST);
	//       -- returns false
	//       -- no connections are made because there is
	//       -- already an exit WEST
	//    r1.connect(EAST, &r2, SOUTH);
	//       -- returns false
	//       -- no connections are made because there is
	//       -- already an exit SOUTH from r2.
	bool connect(Direction exit, Room * r, Direction to);

	// destructor
	// the destructor disconnects itself from
	// other rooms before the room itself is
	// destroyed.  This prevents dangling pointer
	// in other rooms
	virtual ~Room();
	
	
private:

	// helper method for destructor
	// CAUTION: dangerous
	//   --- may cause memroy errors!
	void disconnect(Direction d);

	//  attributes
	string description_;
	string name_;
	Object* object_;
	vector<Room *> exits_;
	vector<Object*> objectsInRoom; 
	//vectorObject* objectsInRoom; 
};

ostream & operator<<(ostream & out, const Room & r);

#endif
