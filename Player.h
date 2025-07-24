#include <string>

#include "LinkedList.h"
#include "Tile.h"

#ifndef ASSIGN2_PLAYER_H
#define ASSIGN2_PLAYER_H


class Player {
public:

Player();

~Player();

bool isInteger(std::string line);

std::string readNameIn(int playerNum);

void updateScore(char letter);

void loadScore(int loadedScore);

int getScore();

void setScore(int score);

void displayScore();

void genPlayersHand(LinkedList* tileList);

LinkedList* getPlayerHand();

void updateHand(std::string savedHand);

void setName(std::string n);

std::string getName();

bool getHasPassed();

void setHasPassed(bool hasPassed);


private:
    std::string nameReadIn;
    std::string name;
    std::string player1Name;

    bool hasPassed;
    bool exitName;
    bool exitNameUpper;
    bool exitNameDigit;

    int playerScore = 0;
    int playerNumber;

    Tile* tempTile;

    LinkedList* playersHand = new LinkedList();

};


#endif //ASSIGN2_PLAYER_H