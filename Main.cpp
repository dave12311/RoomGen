#include <iostream>
#include <string>
#include "Header.h"
#include <time.h>
#include <stdlib.h>
using namespace std;

int main() {
	clr();
	cout << "RoomGen version 3.0 by DBStudio" << endl;
	cout << "Keszitette: Horvath David" << endl;
	cout << "Minden egyseg meterben van megadva, a tizedestortek ponttal elvalasztva" << endl << endl;

	//Start code
	srand((unsigned)time(0));
	Database myDatabase; //Read database
	Room myRoom(&myDatabase);
	cin.ignore();
}
