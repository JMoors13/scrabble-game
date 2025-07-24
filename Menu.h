#include "LinkedList.h"
#include "Player.h"

#include <vector>
#include <algorithm>
#include <string>
#include <iostream>
#include <limits>
#include <sstream>

#ifndef ASSIGN2_GameDriver_H
#define ASSIGN2_GameDriver_H

class Menu{
public:

    Menu();
    ~Menu();

    int UserSelect();

    std::string enterTile(LinkedList* userhand);

private:
    int menuOption;
    std::string userMove;
};

#endif // ASSIGN2_GameDriver_H