//
//  snake.hpp
//  C++
//
//  Created by qt on 2024/3/18.
//

#ifndef snake_hpp
#define snake_hpp

#include <iostream>
#include <string>
#include <deque>
#include <vector>
#include "helper.hpp"
#include <ctime>
#include <cstdlib>

using namespace std;

/* Number of food pellets that must be eaten to win*/
const int kMaxFood = 20;

/* Constants for different tile types */
const char kEmptyTile = ' ';
const char kWallTile = '#';
const char kFoodTile = '$';
const char kSnakeTile = '*';
const double kWaitTime = 0.1;
const double kTurnRate = 0.2;

const string kClearCommand = "CLS";

struct pointT {
    long row, col;
};

struct gameT {
    vector<string> world;
    int numRows, numCols;
    
    deque<pointT> snake;
    int dx, dy; // The snake direction
    
    int numEaten; // How much food snake has eaten
};

int Play();
void InitializeGame(gameT& game);
void RunSimulation(gameT& game);
void LoadWorld(gameT& game, ifstream& input);
string GetLine();
void OpenUserFile(ifstream& input);
pointT MakePoint(long row, long col);

void PrintWorld(gameT& game);
void PerformAi(gameT& game);
bool MoveSnake(gameT& game);
void DisplayResult(gameT& game);
void Pause();

pointT GetNextPosition(gameT& game, int dx, int dy);
bool Crashed(pointT headPos,gameT& game);
bool RandomChance(double rate);
bool InWorld(pointT& pt, gameT& game);
void PlaceFood(gameT& game);

#endif /* snake_hpp */
