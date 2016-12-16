#include <iostream>
#include <string>
#include "Header.h"
#include <time.h>
using namespace std;

int main() {
	clr();
	cout << "RoomGen version 3.0 by DBStudio" << endl;
	cout << "Készítette: Horváth Dávid" << endl;
	cout << "Minden egység méterben van megadva, a tizedestörtek ponttal elválasztva" << endl << endl;

	//Start code
	srand((unsigned)time(0));
	Database myDatabase; //Read database
	Room myRoom(&myDatabase);
	cin.ignore();
}
