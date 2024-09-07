//
//  snake.cpp
//  C++
//
//  Created by qt on 2024/3/18.
//

#include "snake.hpp"

int Play() {
    gameT game;
    
    InitializeGame(game);
    RunSimulation(game);
    return 0;
}

void InitializeGame(gameT& game) {
    srand(static_cast<unsigned int>(time(NULL)));
    ifstream input;
    OpenUserFile(input);
    LoadWorld(game, input);
}

pointT MakePoint(long row, long col) {
    pointT point;
    point.row = row;
    point.col = col;
    return point;
}

void LoadWorld(gameT& game, ifstream& input) {
    input >> game.numRows >> game.numCols;
    game.world.resize(game.numRows);
    input >> game.dx >> game.dy;
    
    string dummy;
    getline(input, dummy);
    for (long i = 0; i < game.numRows; ++i) {
        getline(input, game.world[i]);
        long col = game.world[i].find(kSnakeTile);
        if (col != string::npos) {
            game.snake.push_back(MakePoint(i, col));
        }
    }
    
    game.numEaten = 0;
}

void PrintWorld(gameT& game) {
    system(kClearCommand.c_str());
    for(int row = 0; row < game.numRows; ++row)
        cout << game.world[row] << endl;
    cout << "Food eaten: " << game.numEaten << endl;
}

bool InWorld(pointT& pt, gameT& game) {
     return pt.col >= 0 &&
     pt.row >= 0 &&
     pt.col < game.numCols &&
     pt.row < game.numRows;
}

bool Crashed(pointT headPos, gameT& game) {
     return !InWorld(headPos, game) ||
     game.world[headPos.row][headPos.col] == kSnakeTile ||
     game.world[headPos.row][headPos.col] == kWallTile;
}

bool RandomChance(double probability) {
    return (rand() / (RAND_MAX + 1.0)) < probability;
}

void PerformAi(gameT& game) {
    pointT nextHead = GetNextPosition(game, game.dx, game.dy);
    if( Crashed(nextHead, game) || RandomChance(kTurnRate)) {
        int leftDx = -game.dy;
        int leftDy = game.dx;

        int rightDx = game.dy;
        int rightDy = -game.dx;

        /* Check if turning left or right will cause us to crash. */
        bool canLeft = !Crashed(GetNextPosition(game, leftDx, leftDy),game);
        bool canRight = !Crashed(GetNextPosition(game, rightDx, rightDy),game);
        
        bool willTurnLeft = false;
        if(!canLeft && !canRight)
            return; // If we can't turn, don't turn.
        else if(canLeft && !canRight)
            willTurnLeft = true; // If we must turn left, do so.
        else if(!canLeft && canRight)
            willTurnLeft = false; // If we must turn right, do so.
        else
            willTurnLeft = RandomChance(0.5); // Else pick randomly
        
        game.dx = willTurnLeft? leftDx : rightDx;
        game.dy = willTurnLeft? leftDy : rightDy;
    }
}

void PlaceFood(gameT& game) {
     while(true) {
         int row = rand() % game.numRows;
         int col = rand() % game.numCols;

     /* If the specified position is empty, place the food there. */
     if(game.world[row][col] == kEmptyTile) {
         game.world[row][col] = kFoodTile;
         return;
     }
     }
 }

bool MoveSnake(gameT& game) {
    pointT nextHead = GetNextPosition(game, game.dx, game.dy);
    
    if(Crashed(nextHead, game)) {
        return false;
    }
    bool isFood = (game.world[nextHead.row][nextHead.col] == kFoodTile);
    
    game.world[nextHead.row][nextHead.col] = kSnakeTile;
    game.snake.push_front(nextHead);
    
    if (!isFood) {
        game.world[game.snake.back().row][game.snake.back().col] = kEmptyTile;
        game.snake.pop_back();
    } else {
        ++game.numEaten;
        PlaceFood(game);
    }

    return true;
}

pointT GetNextPosition(gameT& game, int dx, int dy) {
     /* Get the head position. */
     pointT result = game.snake.front();

     /* Increment the head position by the current direction. */
     result.row += dy;
     result.col += dx;
     return result;
 }

void DisplayResult(gameT& game) {
    PrintWorld(game);
     if(game.numEaten == kMaxFood)
     cout << "The snake ate enough food and wins!" << endl;
     else
     cout << "Oh no! The snake crashed!" << endl;
}

void Pause() {
    clock_t startTime = clock();
    
    while(static_cast<double>(clock() - startTime) / CLOCKS_PER_SEC < kWaitTime);
}

void RunSimulation(gameT& game) {
    while(game.numEaten < kMaxFood) {
        PrintWorld(game);
        PerformAi(game);
        if(!MoveSnake(game)) break;
        Pause();
    }
    DisplayResult(game);
}
