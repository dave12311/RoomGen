#ifndef HEADER_H
#define HEADER_H
#include <string>
#include <vector>
using namespace std;

#define Precision 1000
#define Iterations 5000000

void clr();

float rnd(float min, float max);

class Data {
public:
	string Name;
	float A[7];
	Data();
};

class Database {
public:
	vector<Data> BaseFloors;
	vector<Data> BaseWalls;
	vector<Data> BaseCeilings;

	vector<Data> FixedFloors;
	vector<Data> FixedWalls;
	vector<Data> FixedCeilings;

	vector<Data> AcousticFloors;
	vector<Data> AcousticWalls;
	vector<Data> AcousticCeilings;

	Database();
private:
	void readData();
};

class Room {
	struct surface {
		float S, SW, SF, SC;
	};
public:
	float H, W, D;
	float V;
	surface S;
	surface availableS;

	float T;

	Room(Database*dbIn);
private:

	struct Best {
		vector<Data*> Floor, Wall, Ceiling;
		vector<float> FloorSurf, WallSurf, CeilingSurf;
		float Ts[7];
		float Score = 1000;
	};

	float myT[7];
	float myScore;

	Database*myDB;

	Data *myFloor, *myWall, *myCeiling;
	vector<Data*> fixedFloor, fixedWall, fixedCeiling;
	vector<float> fixedFloorS, fixedWallS, fixedCeilingS;
	vector<Data*> myAcousticFloors, myAcousticWalls, myAcousticCeilings;
	vector<float> myFloorSurfaces, myWallSurfaces, myCeilingSurfaces;

	Best Bests[5];

	void init();
	void calculate();
	void incorrectIn(int errReturn);
	void baseMaterialLister();
	void fixedMaterialLister();
	void acousticMaterialLister();
	void getT();

	void generateSurfaces();
	void solve();

	void insertBest(int n);

	void printResults();
};

#endif