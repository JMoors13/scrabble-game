#include "LinkedList.h"

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include <algorithm>

#ifndef ASSIGN2_BOARD_H
#define ASSIGN2_BOARD_H

class Board{
public:

Board();

~Board();

void NewBoard();

void PrintBoard();

void UpdateBoard(std::vector<std::string> loadVector);

std::vector<std::string> getBoard();

void setFirstWord();

void createDictionary();

void setCurrentPlayer(std::string name);

std::string getCurrentPlayer();

void displayTurn();

bool isEmpty();

bool placeChecker(std::vector<std::string> placementArray);

bool placeTile(std::vector<std::string> placementArray);

bool wordChecker(std::vector<std::string> placementArray);

private:
std::vector<std::string> boardVector;
std::vector<std::string> boardVectorPrevious;
std::vector<std::string> dictionary;
std::string currentPlayer;
std::string userVar;
std::string aboveTile;
std::string belowTile;
std::string boardRow;
std::string stoiVar;
std::string wordToTest;
char tileLetter;
char space = ' ';
char yordChar;
int yaxis = 0;
int xaxis = 0;
int counter = 0;
int firstTile = 0;
int firstWord = 0;
int xCopy;
int yCopy;

};

#endif // ASSIGN2_BOARD_H