#include "Header.h"
#include <algorithm>
#include <math.h>
//start
#include <iostream>
//end
using namespace std;

float rnd(float min, float max) { //100 = 0.01
    float output = min + (rand() % (int)((max*Precision) - min + 1));
    return output/(Precision);
}

void Room::generateSurfaces() { //Generates the random surfaces in the range of availableS and randomizes order of array myAcousticSomething for random purposes

    myWallSurfaces.clear();
    myFloorSurfaces.clear();
    myCeilingSurfaces.clear();

    //Walls
    myWallSurfaces.push_back(0);
    for(int i = 0;i < myAcousticWalls.size(); i++) {
        myWallSurfaces.push_back(rnd(0,availableS.SW));
    }
    sort(myWallSurfaces.begin(),myWallSurfaces.end());
    for(int i = 0;i < myWallSurfaces.size();i++) {
        if(i != myWallSurfaces.size()-1){
            myWallSurfaces[i] = myWallSurfaces[i+1] - myWallSurfaces[i];
        }else{
            myWallSurfaces[i] = availableS.SW - myWallSurfaces[i];
        }
    }
    random_shuffle(myAcousticWalls.begin(),myAcousticWalls.end());

    //Floor
    myFloorSurfaces.push_back(0);
    for(int i = 0;i < myAcousticFloors.size(); i++) {
        myFloorSurfaces.push_back(rnd(0,availableS.SF));
    }
    sort(myFloorSurfaces.begin(),myFloorSurfaces.end());
    for(int i = 0;i < myFloorSurfaces.size();i++) {
        if(i != myFloorSurfaces.size()-1){
            myFloorSurfaces[i] = myFloorSurfaces[i+1] - myFloorSurfaces[i];
        }else{
            myFloorSurfaces[i] = availableS.SF - myFloorSurfaces[i];
        }
    }
    random_shuffle(myAcousticFloors.begin(),myAcousticFloors.end());

    //Ceiling
    myCeilingSurfaces.push_back(0);
    for(int i = 0;i < myAcousticCeilings.size(); i++) {
        myCeilingSurfaces.push_back(rnd(0,availableS.SC));
    }
    sort(myCeilingSurfaces.begin(),myCeilingSurfaces.end());
    for(int i = 0;i < myCeilingSurfaces.size();i++) {
        if(i != myCeilingSurfaces.size()-1){
            myCeilingSurfaces[i] = myCeilingSurfaces[i+1] - myCeilingSurfaces[i];
        }else{
            myCeilingSurfaces[i] = availableS.SC - myCeilingSurfaces[i];
        }
    }
    random_shuffle(myAcousticCeilings.begin(),myAcousticCeilings.end());
}

void Room::solve() {
    float AlphaS[7]; //Alpha*S
    myScore = 0;
    for(int t = 0;t < 7;t++) {

        AlphaS[t] = 0;
    //Ceiling
        //Acoustic
        for(int ac = 0;ac < myAcousticCeilings.size();ac++) {
            AlphaS[t] += myCeilingSurfaces[ac] * myAcousticCeilings[ac]->A[t];
        }
        //Fixed
        for(int fc = 0;fc < fixedCeiling.size();fc++) {
            AlphaS[t] += fixedCeilingS[fc] * fixedCeiling[fc]->A[t];
        }
        //Else
        AlphaS[t] += (S.SC - availableS.SC) * myCeiling->A[t];
    //Walls
        //Acoustic
        for(int ac = 0;ac < myAcousticWalls.size();ac++) {
            AlphaS[t] += myWallSurfaces[ac] * myAcousticWalls[ac]->A[t];
        }
        //Fixed
        for(int fc = 0;fc < fixedWall.size();fc++) {
            AlphaS[t] += fixedWallS[fc] * fixedWall[fc]->A[t];
        }
        //Else
        AlphaS[t] += (S.SW - availableS.SW) * myWall->A[t];
    //Floor
        //Acoustic
        for(int ac = 0;ac < myAcousticFloors.size();ac++) {
            AlphaS[t] += myFloorSurfaces[ac] * myAcousticFloors[ac]->A[t];
        }
        //Fixed
        for(int fc = 0;fc < fixedFloor.size();fc++) {
            AlphaS[t] += fixedFloorS[fc] * fixedFloor[fc]->A[t];
        }
        //Else
        AlphaS[t] += (S.SF - availableS.SF) * myFloor->A[t];

        myT[t] = (-0.163 * V) / (S.S * (log(1 - (AlphaS[t] / S.S))));
        myScore += myT[t];
    }
    myScore = myScore / 7;
}

void Room::insertBest(int n) {
    Bests[n].Floor = myAcousticFloors;
    Bests[n].Wall = myAcousticWalls;
    Bests[n].Ceiling = myAcousticCeilings;

    Bests[n].FloorSurf = myFloorSurfaces;
    Bests[n].WallSurf = myWallSurfaces;
    Bests[n].CeilingSurf = myCeilingSurfaces;

    for(int f = 0;f < 7;f++) {
        Bests[n].Ts[f] = myT[f];
    }

    Bests[n].Score = myScore;
}