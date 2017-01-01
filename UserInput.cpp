#include "Header.h"
#include "math.h"
#include <iostream>
#include <stdlib.h>

using namespace std;

void clr(){
	system("cls");
}

Room::Room(Database*dbIn) {
	myDB = dbIn;
	init();
	calculate();
	baseMaterialLister();
	fixedMaterialLister();
	acousticMaterialLister();
	getT();
	for(int i = 0;i < Iterations;i++) {
		generateSurfaces();
		solve();
		if(myScore < Bests[0].Score) {
			insertBest(0);
		} else if(myScore < Bests[1].Score) {
			insertBest(1);
		} else if(myScore < Bests[2].Score) {
			insertBest(2);
		} else if(myScore < Bests[3].Score) {
			insertBest(3);
		} else if(myScore < Bests[4].Score) {
			insertBest(4);
		}

		if(i == Iterations*0.15) {cout << "15%" << endl;}
		else if(i == Iterations*0.25) {cout << "25%" << endl;}
		else if(i == Iterations*0.50) {cout << "50%" << endl;}
		else if(i == Iterations*0.75) {cout << "75%" << endl;}
		else if(i == Iterations*0.95) {cout << "95%" << endl;}
	}
	printResults();
}

void Room::incorrectIn(int errReturn) {

	clr();

	cout << endl << "Ervenytelen szam." << endl << endl;

	cin.clear();

	cin.ignore();

	switch(errReturn) {
		case 0:
			init();
		break;
		case 1:
			baseMaterialLister();
		break;
		case 2:
			fixedMaterialLister();
		break;
		case 3:
			acousticMaterialLister();
		break;
		case 4:
			getT();
		break;
		case 5:
			printResults();
		break;
	}

}

void Room::calculate() {
	V = H*W*D;

	S.SW = H*W * 2 + H*D * 2;

	S.SF = D*W;

	S.SC = S.SF;

	S.S = S.SW + S.SF + S.SC;

	availableS.S = S.S;
	availableS.SW = S.SW;
	availableS.SF = S.SF;
	availableS.SC = S.SC;
}

void Room::init() {

	float temp;

	cout << "Magassag=";

	if (cin >> temp && temp > 0) {H = temp;} else {incorrectIn(0);}

	cout << "\nSzelesseg=";

	if (cin >> temp && temp > 0) {W = temp;} else {incorrectIn(0);}

	cout << "\nHosszusag=";

	if (cin >> temp && temp > 0) {D = temp;} else {incorrectIn(0);}

	cout << endl;
	clr();
}

void Room::baseMaterialLister() {
	cout << "Szoba adatai:" << endl << endl;

	int c;

	//Padló
	cout << "Padlo:" << endl;
	for(int i = 0;i < myDB->BaseFloors.size();i++){
		cout << i+1 << ".) " << myDB->BaseFloors[i].Name << endl;
	}
	cout << endl << "Valasztott: ";
	if(cin>>c && c > 0 && c <= myDB->BaseFloors.size()){
		myFloor = &myDB->BaseFloors[c-1];
	}else{
		incorrectIn(1);
	}

	clr();
	cout << "Szoba adatai:" << endl << endl;

	//Falak
	cout << "Falak:" << endl;
	for(int i = 0;i < myDB->BaseWalls.size();i++){
		cout << i+1 << ".) " << myDB->BaseWalls[i].Name << endl;
	}
	cout << endl << "Valasztott: ";
	if(cin>>c && c > 0 && c <= myDB->BaseWalls.size()){
		myWall = &myDB->BaseWalls[c-1];
	}else{
		incorrectIn(1);
	}

	clr();
	cout << "Szoba adatai:" << endl << endl;

	//Plafon
	cout << "Plafon:" << endl;
	for(int i = 0;i < myDB->BaseCeilings.size();i++){
		cout << i+1 << ".) " << myDB->BaseCeilings[i].Name << endl;
	}
	cout << endl << "Valasztott: ";
	if(cin>>c && c > 0 && c <= myDB->BaseCeilings.size()){
		myCeiling = &myDB->BaseCeilings[c-1];
	}else{
		incorrectIn(1);
	}

	clr();
}

void Room::fixedMaterialLister() {
	
	int c;
	float surf;

	//Padló
	if(myDB->FixedFloors.size() != 0){
		while(true){
			clr();
			cout << "Fix anyagok felulettel:" << endl << endl;
			cout << "Padlo:" << endl;
			for(int i = 0;i < myDB->FixedFloors.size();i++){
				cout << i+1 << ".) " << myDB->FixedFloors[i].Name << endl;
			}
			cout << "0.) Tovabb" << endl;
			cout << "Valasztott: ";
			if(cin>>c && c >= 0 && c <= myDB->FixedFloors.size()){
				if(c==0){break;}
				cout << "Felulet (" << availableS.SF << " m^2 elerheto): ";
				if(cin>>surf && surf > 0 && surf <= availableS.SF){
					fixedFloor.push_back(&myDB->FixedFloors[c-1]);
					fixedFloorS.push_back(surf);
					availableS.SF -= surf;
					availableS.S -= surf;
					if(availableS.SF==0){break;}
				}else{
					incorrectIn(2);
				}
			}else{
				incorrectIn(2);
			}
		}
	}

	clr();
	cout << "Fix anyagok felulettel:" << endl << endl;

	//Falak
	if(myDB->FixedWalls.size() != 0){
		while(true){
			clr();
			cout << "Fix anyagok felulettel:" << endl << endl;
			cout << "Falak:" << endl;
			for(int i = 0;i < myDB->FixedWalls.size();i++){
				cout << i+1 << ".) " << myDB->FixedWalls[i].Name << endl;
			}
			cout << "0.) Tovabb" << endl;
			cout << "Valasztott: ";
			if(cin>>c && c >= 0 && c <= myDB->FixedWalls.size()){
				if(c==0){break;}
				cout << "Felulet (" << availableS.SW << " m^2 elerheto): ";
				if(cin>>surf && surf > 0 && surf <= availableS.SW){
					fixedFloor.push_back(&myDB->FixedWalls[c-1]);
					fixedFloorS.push_back(surf);
					availableS.SW -= surf;
					availableS.S -= surf;
					if(availableS.SW==0){break;}
				}else{
					incorrectIn(2);
				}
			}else{
				incorrectIn(2);
			}
		}
	}

	clr();
	cout << "Fix anyagok felulettel:" << endl << endl;

	//Plafon
	if(myDB->FixedCeilings.size() != 0){
		while(true){
			clr();
			cout << "Fix anyagok felulettel:" << endl << endl;
			cout << "Plafon:" << endl;
			for(int i = 0;i < myDB->FixedCeilings.size();i++){
				cout << i+1 << ".) " << myDB->FixedCeilings[i].Name << endl;
			}
			cout << "0.) Tovabb" << endl;
			cout << "Valasztott: ";
			if(cin>>c && c >= 0 && c <= myDB->FixedCeilings.size()){
				if(c==0){break;}
				cout << "Felulet (" << availableS.SC << " m^2 elerheto): ";
				if(cin>>surf && surf > 0 && surf <= availableS.SC){
					fixedFloor.push_back(&myDB->FixedCeilings[c-1]);
					fixedFloorS.push_back(surf);
					availableS.SC -= surf;
					availableS.S -= surf;
					if(availableS.SC==0){break;}
				}else{
					incorrectIn(2);
				}
			}else{
				incorrectIn(2);
			}
		}
	}

	clr();
}

void Room::acousticMaterialLister() {

	int c;
	bool x, y;

	//Padló
	while(true){
		cout << "Lehetseges akusztikai elemek:" << endl << endl;
		cout << "Padlo:" << endl;
		for(int i = 0;i < myDB->AcousticFloors.size();i++){
			cout << i+1 << ".) " << myDB->AcousticFloors[i].Name;
			if(myAcousticFloors.size() == 0){
				cout << " [ ]" << endl;
			}else{
				x = false;
				for(int f = 0;f < myAcousticFloors.size();f++){
					if(myDB->AcousticFloors[i].Name == myAcousticFloors[f]->Name){x = true;}
				}
				if(x == true){
					cout << " [X]";
				}else{
					cout << " [ ]";
				}
				cout << endl;
			}
		}
		cout << "0.) Tovabb" << endl;
		cout << "Valasztott: ";
		if(cin>>c && c >= 0 && c <= myDB->AcousticFloors.size()){
			if(c==0){break;}
			if(myAcousticFloors.size() == 0){
				myAcousticFloors.push_back(&myDB->AcousticFloors[c-1]);
			}else{
				y = false;
				for(int k = 0;k < myAcousticFloors.size();k++){
					if(myDB->AcousticFloors[c-1].Name == myAcousticFloors[k]->Name){y = true;}
				}
				if(y == false){
					myAcousticFloors.push_back(&myDB->AcousticFloors[c-1]);
				}
			}
			clr();
		}else{
			incorrectIn(3);
		}
	}

	//Falak
	while(true){
		clr();
		cout << "Lehetseges akusztikai elemek:" << endl << endl;
		cout << "Falak:" << endl;
		for(int i = 0;i < myDB->AcousticWalls.size();i++){
			cout << i+1 << ".) " << myDB->AcousticWalls[i].Name;
			if(myAcousticWalls.size() == 0){
				cout << " [ ]" << endl;
			}else{
				x = false;
				for(int f = 0;f < myAcousticWalls.size();f++){
					if(myDB->AcousticWalls[i].Name == myAcousticWalls[f]->Name){x = true;}
				}
				if(x == true){
					cout << " [X]";
				}else{
					cout << " [ ]";
				}
				cout << endl;
			}
		}
		cout << "0.) Tovabb" << endl;
		cout << "Valasztott: ";
		if(cin>>c && c >= 0 && c <= myDB->AcousticWalls.size()){
			if(c==0){break;}
			if(myAcousticWalls.size() == 0){
				myAcousticWalls.push_back(&myDB->AcousticWalls[c-1]);
			}else{
				y = false;
				for(int k = 0;k < myAcousticWalls.size();k++){
					if(myDB->AcousticWalls[c-1].Name == myAcousticWalls[k]->Name){y = true;}
				}
				if(y == false){
					myAcousticWalls.push_back(&myDB->AcousticWalls[c-1]);
				}
			}
			clr();
		}else{
			incorrectIn(3);
		}
	}

	//Plafon
	while(true){
		clr();
		cout << "Lehetseges akusztikai elemek:" << endl << endl;
		cout << "Plafon:" << endl;
		for(int i = 0;i < myDB->AcousticCeilings.size();i++){
			cout << i+1 << ".) " << myDB->AcousticCeilings[i].Name;
			if(myAcousticCeilings.size() == 0){
				cout << " [ ]" << endl;
			}else{
				x = false;
				for(int f = 0;f < myAcousticCeilings.size();f++){
					if(myDB->AcousticCeilings[i].Name == myAcousticCeilings[f]->Name){x = true;}
				}
				if(x == true){
					cout << " [X]";
				}else{
					cout << " [ ]";
				}
				cout << endl;
			}
		}
		cout << "0.) Tovabb" << endl;
		cout << "Valasztott: ";
		if(cin>>c && c >= 0 && c <= myDB->AcousticCeilings.size()){
			if(c==0){break;}
			if(myAcousticCeilings.size() == 0){
				myAcousticCeilings.push_back(&myDB->AcousticCeilings[c-1]);
			}else{
				y = false;
				for(int k = 0;k < myAcousticCeilings.size();k++){
					if(myDB->AcousticCeilings[c-1].Name == myAcousticCeilings[k]->Name){y = true;}
				}
				if(y == false){
					myAcousticCeilings.push_back(&myDB->AcousticCeilings[c-1]);
				}
			}
			clr();
		}else{
			incorrectIn(3);
		}
	}

	clr();
}

void Room::getT() {
	clr();
	float temp;
	cout << "Utozengesi ido: ";
	if(cin >> temp && temp > 0) {
		T = temp;
	} else {
		incorrectIn(4);
	}
}

void Room::printResults() {
	cout << "Eredmenyek: " << endl << endl;
	for(int i = 0;i < 5;i++) {
		cout << i+1 << ".) " << (Bests[i].Ts[0]+Bests[i].Ts[1]+Bests[i].Ts[2]+Bests[i].Ts[3]+Bests[i].Ts[4]+Bests[i].Ts[5]+Bests[i].Ts[6])/7 << "   (" << Bests[i].Ts[0] << " " << Bests[i].Ts[1] << " " << Bests[i].Ts[2] << " " << Bests[i].Ts[3] << " " << Bests[i].Ts[4] << " " << Bests[i].Ts[5] << " " << Bests[i].Ts[6]  << ")" << endl; 
	}
	cout << endl;
	cout << "Valasztott: ";
	int k;
	if(cin >> k && k >= 0 && k <= 5) {
		clr();
		cout << "Padlo:" << endl;
		for(int f = 0;f < Bests[k-1].Floor.size();f++) {
			if(Bests[k-1].FloorSurf[f] >= 0.1){
				cout << Bests[k-1].Floor[f]->Name << " " << Bests[k-1].FloorSurf[f] << "m^2" << endl;
			}
		}
		cout << endl << "Falak:" << endl;
		for(int f = 0;f < Bests[k-1].Wall.size();f++) {
			if(Bests[k-1].WallSurf[f] >= 0.1){
				cout << Bests[k-1].Wall[f]->Name << " " << Bests[k-1].WallSurf[f] << "m^2" << endl;
			}
		}
		cout << endl << "Plafon:" << endl;
		for(int f = 0;f < Bests[k-1].Ceiling.size();f++) {
			if(Bests[k-1].CeilingSurf[f] >= 0.1){
				cout << Bests[k-1].Ceiling[f]->Name << " " << Bests[k-1].CeilingSurf[f] << "m^2" << endl;
			}
		}
		printResults();
	} else {
		incorrectIn(5);
	}
}