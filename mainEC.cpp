#include "Room.h"
#include "Object.h"
#include "vectorObject.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <algorithm>

using namespace std;

void menu();
void look(Room* current_room);
void exits(Room* current_room);
void add(Room* current, vector<Room*>& allRooms, vector<string>&roomConnections, bool mode);
void connect(Room* current, vector<Room*> allRooms, vector<string>&roomConnections, bool mode);
void rename(Room* current);
void description(Room* current);
bool isConnected(Room*current, Direction dir); 
void jump(Room* &current, vector<Room*> allRooms); 
void save(Room* current, vector<Room*> allRooms, vector<string>roomConnections, vector<Object> bag);
void load(Room*&current, vector<Room*>& allRooms, vector<string>&roomConnections, vector<Object>&bag); 
void initializeRC(vector<string>&roomConnections, int size); 
void initializeAO(vector<Object>&bag, int size);
void pickUpObject(vector<Object>&bag, Room* current); 
void inventory(vector<Object>bag); 

//void object(Room* current_room, vector<Object>&bag); 
void createObject(vector<Room*> allRooms);
void lookAtObjects(vector<Object> bag);
Direction char2Direction(char c); // convert a char value ('N','S','E','W')
								  // to an enum value NORTH, SOUTH, EAST, WEST etc
char digit2letter(char n);
string letter2digit(char l); 
bool allFourConnected(Room*current); 

int main()
{
    // initialize
    Room* current_room = new Room();
	//Object current_object = Object(); 
	//cout << current_object.getName() << endl; 
    vector<Room*> allRooms;
	vector<string>roomConnections; //an array of strings that keeps track of all of the exits 
	initializeRC(roomConnections, 25); 
	vector<Object>bag; 
	//initializeAO(bag, 25);
    allRooms.push_back(current_room);
	Object currentObject; 
	string userInput; 
	char choice;
	bool builderMode; 

	
	cout << "Builder Mode on or off? Press 1 for ON, 0 for OFF." << endl; 
	cin >> builderMode; 

	//show name and description of current_room
	look(current_room); 
    // menu loop
    do
    {
		cout << "Enter a command (? to list): ";
        cin>>userInput;
		choice = tolower(userInput[0]); 
		if (userInput.compare("exits") == 0)
			choice = 'x'; 
		else if (userInput.compare("save") == 0)
			choice = 'z';
		else if (userInput.compare("load") == 0)
			choice = 'b';
		
        switch(choice)
        {
            case 'n':
                if (current_room->north()!= NULL)
                    current_room = current_room->north();
                else
                    cout<<"You cannot go that way"<<endl;
                break;
            case 'e':
                if (current_room->east()!= NULL)
                    current_room = current_room->east();
                else
                    cout<<"You cannot go that way"<<endl;
                break;
            case 's':
                if (current_room->south()!= NULL)
                    current_room = current_room->south();
                else
                    cout<<"You cannot go that way"<<endl;
                break;
            case 'w':
                if (current_room->west()!= NULL)
                    current_room = current_room->west();
                else
                    cout<<"You cannot go that way"<<endl;
                break;
            case 'l':
                look(current_room); break;
            case 'x':
                exits(current_room);break;
            case 'c':
                connect(current_room, allRooms, roomConnections, builderMode); break;
            case 'r':
                rename(current_room); break;
            case 'd':
                description(current_room); break;
            case 'a':
                add(current_room, allRooms, roomConnections, builderMode); break;
			case 'j': 
				jump(current_room, allRooms); break; 
			case 'o': 
				createObject(allRooms); break; 
			case 'z': 
				save(current_room, allRooms, roomConnections, bag); break;
			case 'b': 
				load(current_room, allRooms, roomConnections, bag); break;
			case 'p':
				pickUpObject(bag, current_room); break; 
			case 'f': 
				lookAtObjects(bag); break; 
			case 'i': 
				inventory(bag); break; 
			case '?':
                menu(); break;
            default:
                cout<<"invalid command"<<endl;
        }
    }while(choice != 'q');



    // cleanup before quitting

    return 0;
}

// Show the name and description of a room */
void look(Room* r)
{
	vector<Object*>roomObjects; int size; 
	roomObjects = r->objects(); 
	size = roomObjects.size(); 
	cout << r->name() << endl; 
	cout << r->description() << endl; 

	if ( size != 0)
	{
		cout << "There is a "; 
		for (int k = 0; k < size - 1; k++)
		{
			cout << roomObjects[k]->getName();
			if (size > 2)
				cout << ", "; 
		}
		if (size > 1)
		{
			cout << " and " << roomObjects[size - 1]->getName() << " here." << endl;
		}
		else
			cout << "." << endl; 
	}

	
}


// add a new room, connected to the current room
//  Parameters
//     current -- the current room we are in
//     allRooms-- a vector of pointers to (addresses of) all the rooms in the game
//  Precondition
//     current refers to an existing room
//  Postcondition
//    if possible (current must have a free exit)
//     a) a new room has been added off one of the exits of current
//     b) the address of the new room has also been added to the allRooms vector
void add(Room* current, vector<Room*>& allRooms, vector<string>&roomConnections, bool mode)
{
	string strExit, strTo, eDigit, rDigit, tDigit; Direction exit, to; 
	int index, dirExit, dirTo; 
	Room* temp; bool flag; 
	if (mode)
	{
		if (allFourConnected(current))
			cout << "All four exits are connected!" << endl;
		else
		{


			//create a new room 
			temp = new Room();

			//determine the direction and validity of current's exit to the new room
			do
			{
				cout << "Connect new room in which direction (N, S, E, W)? ";
				cin >> strExit;
				exit = char2Direction(strExit[0]);
				flag = isConnected(current, exit);

				if (flag)
					cout << "That exit is already connected!" << endl;
			} while (flag);

			//determine the direction of the new room's exit to current
			cout << "Which exit of the new room leads to this room? ";
			cin >> strTo;
			to = char2Direction(strTo[0]);

			//connecting current to new room
			current->connect(exit, temp, to);

			eDigit = letter2digit(strExit[0]);
			rDigit = to_string(allRooms.size());
			tDigit = letter2digit(strTo[0]);

			dirExit = eDigit[0] - '0';
			cout << "dirExit = " << dirExit << " ";

			//determing where to place the above digits 
			index = (allRooms.size() - 1) * 4 + dirExit;
			roomConnections[index] = eDigit + rDigit + tDigit;

			cout << "index = " << index << endl;
			cout << "roomConnections[index] = " << roomConnections[index] << endl;

			//add the address of the new room to allRooms
			allRooms.push_back(temp);

			cout << "Room is connected ok" << endl;
		}
	}
	else
		cout << "Builder mode is OFF" << endl; 
}

// add a new connection between current room and an already existing room.
//  Parameters
//     current -- the current room we are in
//     allRooms-- a vector of pointers to (addresses of) all the rooms in the game
//  Precondition
//     current refers to an existing room
//  Postcondition
//    if possible (both rooms must have a spare exit to link them together)
//     current is now linked to another room in the game
//
void connect(Room* current, vector<Room*> allRooms, vector<string>&roomConnections, bool mode)
{
	string strExit, strTo, eDigit, rDigit, tDigit; Direction exit, to; bool flag; int cellNum, dirExit, roomNum, index;

	if (mode)
	{
		//find the index of current in allRooms
		for (int i = 0; i < allRooms.size(); i++)
		{
			if (current == allRooms[i]) //comparing addresses
			{
				roomNum = i;
			}
		}

		//determine the direction of current's exit to existing room it's connecting to
		//determine whether exit is valid 
		if (allFourConnected(current))
			cout << "All four rooms are connected!" << endl;
		else
		{
			do
			{
				cout << "Connect new room in which direction (N, S, E, W)? ";
				cin >> strExit;
				exit = char2Direction(strExit[0]);
				flag = isConnected(current, exit);
				if (flag) //if exit is already connected
					cout << "That exit is already connected!" << endl;
			} while (flag);


			//determine what the existing room current room wants to connect to 
			//determine whether room number is valid 
			cout << "Connect " << current->name() << " to which room (enter #)? " << endl;

			do
			{
				for (int i = 0; i < allRooms.size(); i++)
					cout << "      " << i << ". " << allRooms[i]->name() << endl;

				cin >> cellNum;
			} while (cellNum > allRooms.size() - 1);


			//determine the direction of existing room's exit to current room
			//determine whether exit is valid 
			do {
				cout << "Which exit of the " << allRooms[cellNum]->name() << " leads to " << current->name() << "? ";
				cin >> strTo;
				to = char2Direction(strTo[0]);
				flag = isConnected(allRooms[cellNum], to);

				if (flag)
					cout << "That exit is already connected" << endl;
			} while (flag);


			//connecting the two rooms
			current->connect(exit, allRooms[cellNum], to);

			//storing the connections (or lack thereof) as a string value
			//-1 == no connection; 0 == N, 1 == E, 2 == S, 3 == W
			eDigit = letter2digit(strExit[0]);
			rDigit = to_string(cellNum);
			tDigit = letter2digit(strTo[0]);
 
			dirExit = eDigit[0] - '0';

			//determing where to place the above digits 
			index = roomNum * 4 + dirExit;

			roomConnections[index] = eDigit + rDigit + tDigit;
			cout << "Room is connected ok" << endl;
		}
	}
	else
		cout << "Builder mode is OFF!" << endl; 
}

// change the name of a room
void rename(Room* current)
{
	string name; 
	cout << "Enter a new name for this room and hit <enter>: "; 
	cin >> name; 
	current->set_name(name); 
}

// change the description of a room
void description(Room* current)
{
    string text;
    cout<<"Enter a new description for this room and hit <enter> ";
    cin.ignore(1,'\n'); // ignore any leading line breaks
    getline(cin,text);  // read one line of text 
    current->set_description(text); //set description of currernt room to text we just read 
}

//shows the exits of a specifed room 
// For each exit from room r,
//   show the name of the room it connects to
//   or say "Nothing" if the link is null
void exits(Room* r)
{ 
	cout << "    North-->"; 
	if (r->north() == NULL)
		cout << " Nothing";
	else
		cout << " " << r->north()->name(); 
	cout << endl; 

	cout << "    East -->"; 
	if (r->east() == NULL)
		cout << " Nothing";
	else
		cout << " " << r->east()->name(); 
	cout << endl; 

	cout << "    South-->"; 
	if (r->south() == NULL)
		cout << " Nothing";
	else
		cout << " " << r->south()->name(); 
	cout << endl; 

	cout << "    West -->";
	if (r->west() == NULL)
		cout << " Nothing";
	else
		cout << " " << r->west()->name();
	cout << endl; 
}
void menu()
{
    cout<<"Please choose from the following: "<<endl;
    cout<<"     n or north --- move north (if possible)"<<endl;
    cout<<"      e or east --- move east (if possible)"<<endl;
    cout<<"     s or south --- move south (if possible)"<<endl;
    cout<<"      w or west --- move at west (if possible)"<<endl;
    cout<<"      l or look --- look at room"<<endl;
    cout<<"     x or exits --- show room exits"<<endl;
    cout<<"   c or connect --- connect this room to another (already made) room"<<endl;
    cout<<"    r or rename --- rename this room"<<endl;
    cout<<"      d or desc --- change description for this room"<<endl;
    cout<<"       a or add --- add new room"<<endl;
	cout<<"      j or jump --- jump to new room" << endl; 
	cout<<"      z or save --- save rooms and connections" << endl; 
	cout<<"      b or load --- load saved rooms and connections" << endl;
	cout<<"      p or pick --- pick up objects" << endl; 
	cout<<"      f or look --- look at objects" << endl;
	cout<<"    o or create --- create objects" << endl; 
	cout<<" i or inventory --- show all the objects in the bag" << endl; 
    cout<<"      q or quit --- exit program"<<endl;
    cout<<"              ? --- show commands (DUH)"<<endl;
}


// convert a char direction: 'N','S','E','W'
// into a Direction enum: NORTH, SOUTH, EAST, WEST
Direction char2Direction(char c)
{
	switch(c)
    {
        case 'N': return NORTH;
        case 'S': return SOUTH;
        case 'E': return EAST;
        case 'W': return WEST;
        default: return NORTH;
    }
}

//Converts a (String) digit to its associated letter, 
//	which stands for one of the cardinal directions 
char digit2letter(char n)
{
	switch (n)
	{
		case '0': return 'N';
		case '1': return 'E'; 
		case '2': return 'S'; 
		case '3': return 'W'; 
		default: return 'N'; 

	}

}

//Converts a letter (which stands for one of the cardinal directions) 
//	into its associated digit 
string letter2digit(char l)
{
	switch (l)
	{
	case 'N': return "0";
	case 'E': return "1";
	case 'S': return "2";
	case 'W': return "3";
	default: return "0";

	}
}

//return false if current->direction() == NULL, meaning you can connect
//return true if current->direction() != NULL, meaning there's already a connection 
bool isConnected(Room*current, Direction dir)
{
	switch (dir)
	{
		case NORTH: return current->north() != NULL;
		case EAST: return current->east() != NULL;
		case SOUTH: return current->south() != NULL; 
		case WEST: return current->west() != NULL; 
		default: 
			return false; 
	}
	
}
//return true if all four exits are connected, false otherwise 
bool allFourConnected(Room* current)
{
	return current->north() != NULL & current->east() != NULL
		& current->south() != NULL & current->west() != NULL; 
		
}

//allows the user to go to a room that is not connected to the room 
//	the user is currently in 
void jump(Room* &current, vector<Room*> allRooms)
{
	bool foundRoom = false; int cellNum; 
	cout << "Which room do you want to jump to (enter a number)? " << endl; 

	do
	{
		for (int i = 0; i < allRooms.size(); i++)
			cout << "      " << i << ". " << allRooms[i]->name() << endl;

		cin >> cellNum;
	} while (cellNum > allRooms.size() - 1);

	current = allRooms[cellNum]; 
}


void save(Room* current, vector<Room*> allRooms, vector<string>roomConnections, vector<Object> bag)
{ 
	cout << "HELLO!" << endl; 
	int cellNum = 0, bagSize, length;
	ofstream gameFile("savedGame.txt");
	Room* last = current; //player has to be in the same room she was when she saved the game 
	

	//write the room number player is in when she saves the game
	for (int i = 0; i < allRooms.size(); i++)
	{
		if (last == allRooms[i])
			cellNum = i;
	}

	gameFile << cellNum << endl;

	//write in total number of rooms
	gameFile << allRooms.size() << endl; 
	cellNum = 0; 

	for (int i = 0; i < allRooms.size(); i++)
	{
		vector <Object*> &roomObjects = allRooms[i]->objects();
		length = allRooms[i]->numObjects();
		cout << "number of objects in room = " << length << endl;
		current = allRooms[i];

		//write name 
		gameFile << current->name() << endl;

		//write description 
		gameFile << current->description() << endl;

		//write in each object in the room's name + description + mobility status 
		gameFile << length << endl;
		for (int i = 0; i < length; i++)
		{
			gameFile << roomObjects[i]->getName() << endl;
			gameFile << roomObjects[i]->getDescription() << endl;
			gameFile << roomObjects[i]->getMobility() << endl; 
		}

		
		string exits = "";
		//write in the room connections 
		for (int k = 0; k < 4; k++)
		{
			gameFile << roomConnections[cellNum];
			cellNum++;
		}
		gameFile << endl;



		//write in each object in bag
		bagSize = bag.size();
		//gameFile << bagSize << endl;
		for (int i = 0; i < bagSize; i++)
		{
			gameFile << bag[i].getName() << endl;
			gameFile << bag[i].getDescription() << endl;

		}

		for (int i = 0; i < bag.size(); i++)
			cout << bag[i].getName() << endl;
		//specify the rooms the exits are connected to 
	}
}

//FIX!!!!!! (room connections) 
void load(Room* &current, vector<Room*>&allRooms, vector<string>&roomConnections, vector<Object>&bag)
{

	ifstream gameFile("savedGame.txt");
	string name, description, currentDir, tempDir, tempExits, currentExits, objName, objDescription;
	Room*  temp; int k = 0, size = 1, totalRooms = 0, numObjects, mobility, objMobility; Object *newObject; 
	Direction exit, to; int cellNum, bagSize, roomCIndex, roomAIndex; 
	
	
	//read in the index of the last room as well as the total number of rooms 
	gameFile >> cellNum;  
	gameFile >> totalRooms; 
	gameFile.ignore(); 
	
	for (int k = 0; k<totalRooms; k++)//three lines of code for each room  
	{
		//set name
		getline(gameFile, name);
		current->set_name(name);
		 
		//set description
		getline(gameFile, description);
		current->set_description(description);
		
		//place all object into the room it was at when player saved the game 
		gameFile >> numObjects; 
		if (numObjects > 0)
		{
			for (int i = 0; i < numObjects; i++)
			{
				newObject = new Object(); 
				gameFile >> objName;
				gameFile.ignore();
				getline(gameFile, objDescription);
				gameFile >> (int)objMobility; 
				newObject->setName(objName);
				newObject->setDescription(objDescription);
				newObject->setMobility(objMobility); 
				current->add_object(newObject);
			}
		}

		//read in the exits of the room 
		gameFile >> currentExits;
		
		//sort out which exit has another room connected to it and add the connections to roomConnection
		for (int i = 4 * k; i < 4 * k + 4; i++)
		{
			if (currentExits.substr(0, 2).compare("-1") != 0) 
			{
				roomConnections[i] = currentExits.substr(0, 3);
				currentExits = currentExits.substr(3, currentExits.length() - 3);
				
			}
			else
			{
				roomConnections[i] = currentExits.substr(0, 2);
				currentExits = currentExits.substr(2, currentExits.length() - 2);
			}
		}
		gameFile.ignore();
		if (!(k == 0))
			allRooms.push_back(current);
		current = new Room();
		 
	}

	roomCIndex = 0; //(roomConnectionsIndex)
	roomAIndex = 0; //(roomAllIndex)
	
	//connect the rooms 
	for (int i = 0; i < totalRooms * 4; i++)
	{
		//if one of the exits is connected to another room... 
		if (roomConnections[roomCIndex].substr(0, 2).compare("-1"))
		{

			current = new Room(); temp = new Room();
			current = allRooms[roomAIndex];

			//convert the exit char to its corresponding direction  
			string digits = roomConnections[i];
			char exitChar = digit2letter(digits[0]);
			Direction exit = char2Direction(exitChar);

			//retrieve room you are connecting to 
			int cellNum = digits[1] - '0';
			temp = allRooms[cellNum];

			//convert the 'to' char to its corresponding direction  
			char toChar = digit2letter(digits[2]);
			Direction to = char2Direction(toChar);
			//cout << "to = " << to << endl;
			current->connect(exit, temp, to);

			//move index to next exit
			roomCIndex++;
		}
		else //if the exit is not connected to another room...
		{

			//move index to next exit 
			roomCIndex++;
		}

		//checks to see if you have finished checking all the exits of a room
		//if so, move on to the next room and check its exits 
		if ((i + 1) % 4 == 0)
		{
			roomAIndex++;
		}
	}

	//place all the objects the player had when she saved the game into the bag
	gameFile >> bagSize;
	
	//bag = vector<Object>(bagSize); 
	for (int i = 0; i < bagSize; i++)
	{

		gameFile >> objName;
		//cout << "objName = " << objName << endl;
		//gameFile >> objDescription; 
		gameFile.ignore();
		getline(gameFile, objDescription);
		//cout << "objDescription = " << objDescription << endl;
		gameFile >> (int)objMobility; 
		Object newObject = Object(objName, objDescription, objMobility); 

		bag.push_back(newObject); 

		for (int i = 0; i < bag.size(); i++)
		{
			cout << bag[i].getName() << endl;
		}
		cout << bag.size() << endl;
		
		//move to the room she was in when she saved the game 
		current = allRooms[cellNum];
	}
	
}

//initialize roomConnections
void initializeRC(vector<string>&roomConnections, int size)
{
	for (int k = 0; k < size; k++)
		roomConnections.push_back("-1");
}

//inititialize the vector of objects in bag 
void initializeAO(vector<Object>&bag, int size)
{

	Object newObject = Object(); 
	string random = " "; 
	for (int k = 0; k < size; k++)
		bag.push_back(newObject); 
}


void createObject(vector<Room*> allRooms)
{
	bool repeat = true; 
	char choice; 
	while (repeat)
	{
		string name, description; bool mobility;  Room* home; int index;
		Object* newObject = new Object();
		
		//add object's name
		cout << "What do you want to name your object? ";
		cin >> name;
		newObject->setName(name);
		cin.ignore();

		//add object's description
		cout << "Add a description of the object: ";
		getline(cin, description);
		newObject->setDescription(description);
		
		//determine the mobility of the object
		cout << "Is this object movable or locked down? Enter 1 for movable and 0 for locked down: "; 
		cin >> mobility; 
		newObject->setMobility(mobility);

		//place object into the specified room
		cout << "Add the room the object belongs to: " << endl;
		for (int i = 0; i < allRooms.size(); i++)
			cout << "	" << i << " " << allRooms[i]->name() << endl;
		cin >> index;
		home = allRooms[index];
		home->add_object(newObject);

		//ask user if they want to create another object 
		cout << "Do you want to create another object?(Y/N) ";
		cin >> choice; 
		if (choice == 'N')
			repeat = false; 
	}

}

void pickUpObject(vector<Object>&bag, Room* current)
{
	bool repeat = true; 
	char choice; 
	while (repeat)
	{
		vector<Object*> &roomObjects = current->objects(); //stores the reference to objectsInRoom
		int total; string name; char flag;
		total = current->numObjects();
		
		if (total == 0)
		{
			cout << "There are no objects to pick up in " << current->name() << endl;
			repeat = false; 
		}
		else
		{
			cout << "There is a ";
			for (int k = 0; k < total - 1; k++)
			{
				cout << roomObjects[k]->getName();
				if (total > 2)
					cout << ", ";
			}

			if (total > 2)
				cout << "and " << roomObjects[total - 1]->getName() << " here." << endl;
			else if (total == 2)
				cout << " and " << roomObjects[total - 1]->getName() << " here." << endl;
			else
				cout << roomObjects[total - 1]->getName() << " here." << endl;

			cout << "What item do you want to pick up?" << endl;
			cin >> name;

			//check to see if the item can be moved or not 
			for (int i = 0; i < roomObjects.size(); i++)
			{
				Object* newObject = current->getObject(i); 
				string objectName = current->getObject(i)->getName();
				if (name == objectName) //when you get to the object you want to pick up, 
				{
					int mobility = current->getObject(i)->getMobility();

					if (mobility == 1) //if the item can be moved
					{
						Object *obj = current->takeObject(i); //pick up the object (takeObject has already deleted the pointer of the object in objectsInRoom)
						bag.push_back(*obj);                  //add it to your bag of objects
						cout << "You now have a " << objectName << " in your bag." << endl; 
						delete obj;                           //delete obj (a pointer to Object)
						break;
					}
					else //if the item cannot be moved 
					{
						cout << "You cannot pick up " << objectName << " from " << current->name() << endl; 
						break; 
					}
				}
				

			}

	
			if (current->numObjects() != 0) //as long as there are still objects left in the room...
			{
				cout << "Do you want to pick up another object? (Y/N): ";
				cin >> choice;
			}
			else
				choice = 'N'; 

			if (choice == 'N')
				repeat = false;
			

		}
	}
		

}

void lookAtObjects(vector<Object> bag)
{
	cout << "bag.size = " << bag.size() << endl; 
	for (int i = 0; i < bag.size(); i++)
		cout << bag[i].getName() << " ";
	cout << endl; 
}

void inventory(vector<Object>bag)
{
	if (bag.size() == 0)
		cout << "There are no objects in your bag." << endl;
	else if (bag.size() == 1)
		cout << "There is a " << bag[0].getName() << " in your bag." << endl;
	else if (bag.size() == 2)
		cout << "There is a " << bag[0].getName() << " and " << bag[1].getName() << " in your bag." << endl;
	else
	{
		cout << "There is a "; 
		for (int i = 0; i < bag.size()-1; i++)
		{
			cout << bag[i].getName() << " ,";
		}
		cout << "and  " << bag[bag.size() - 1].getName() << " in your bag." << endl; 
	}
}




//Sources: 
/*

	https://stackoverflow.com/questions/439573/how-to-convert-a-single-char-into-an-int

*/
