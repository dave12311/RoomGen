#include "Header.h"
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

#include <iostream>

Data::Data() {
	Name = "";
	for(int i=0;i<7;i++){
		A[i] = 0;
	}
}

void Database::readData(){
	ifstream reader;
	string line;

	string Name, Type;
	float Dat[7];

	Data constr;

	reader.open("Data.txt");
	while(true){
		getline(reader, line);
		if(line[0] != '#' && line != "DATA_END"){
			reader >> Name >> Type >> Dat[0] >> Dat[1] >> Dat[2] >> Dat[3] >> Dat[4] >> Dat[5] >> Dat[6];

			if(Name=="DATA_END"){break;}

			constr.Name = Name;
			for(int i = 0;i < 7;i++){
				constr.A[i] = Dat[i];
			}

			if(Type == "BF"){BaseFloors.push_back(constr);}
			else if(Type == "BW"){BaseWalls.push_back(constr);}
			else if(Type == "BC"){BaseCeilings.push_back(constr);}
			else if(Type == "FF"){FixedFloors.push_back(constr);}
			else if(Type == "FW"){FixedWalls.push_back(constr);}
			else if(Type == "FC"){FixedCeilings.push_back(constr);}
			else if(Type == "F"){AcousticFloors.push_back(constr);}
			else if(Type == "W"){AcousticWalls.push_back(constr);}
			else{AcousticCeilings.push_back(constr);}
		}
	}
	reader.close();
}

Database::Database(){
	readData();
}