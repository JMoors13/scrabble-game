#include "Player.h"
#include <iostream>
#include <cstring>
#include <string>

Player::Player() {

}

Player::~Player() {
    delete playersHand;
}

std::string Player::readNameIn(int playerNum) {
    this->playerNumber = playerNum;
    
    do {
        exitName = true;
        exitNameUpper = false;
        exitNameDigit = false;
        std::cout<< "Enter a name for player " << playerNum << " (uppercase characters only)" << std::endl;

        std::cout << "> ";
        getline(std::cin, nameReadIn);
        if(std::cin.eof()){
            std::cout << "Goodbye" << std::endl;
            exit(1);
        }
        for(size_t i = 0; i < nameReadIn.size(); i++){
            char c = nameReadIn[i]; 
            if (isdigit(c) == true){  
                exitNameDigit = true;
            }
            if (c & 0x20){
                exitNameUpper = true;
            }
        }
        if(nameReadIn.empty() == true ){
            std::cout<< "Name cannot be empty" << std::endl;
            std::cin.clear();
            exitName = false;
        }
        else if(nameReadIn == "help"){
            std::cout << "From this location you're able to:" << std::endl;
            std::cout << "- Enter your in-game name: 'EXAMPLE'" << std::endl;
            std::cout << "-------------------------------------------------------------------" << std::endl;             std::cin.clear();
            exitName = false;
        }
        else if (exitNameDigit == true){
            std::cout<< "Name cannot contain numbers (sorry X Æ A-12)" << std::endl;
            std::cin.clear();
            exitName = false;
        }
        else if(exitNameUpper == true){
            std::cout<< "Full name must be in uppercase" << std::endl;
            std::cin.clear();
            exitName = false;
        }
        }
    while (exitName == false);

    return nameReadIn;
}

void Player::updateScore(char letter) {
    Node* temp = playersHand->getNodeAt(0);
    while (temp->next != nullptr) {
        if(temp->tile->letter == letter){
            playerScore = playerScore + temp->tile->value;
            break;
        }
        temp = temp->next;
    }
}

void Player::loadScore(int loadedScore){
    playerScore = loadedScore;
}

int Player::getScore(){
    return playerScore;
}

void Player::setScore(int score) {
    playerScore = score;
}

void Player::displayScore(){
    std::cout << "Score for "<< name << ": "<< playerScore << std::endl;
}

void Player::genPlayersHand(LinkedList* tileList) {
    for(int i = 0; i < 7; i++){
        tempTile = tileList->getTile();
        this->playersHand->addBack(tempTile);
    }
}

LinkedList* Player::getPlayerHand(){
    return this->playersHand;
}

void Player::updateHand(std::string savedHand){
    std::string delimiter = ", ";
    std::string doubleDigit;

    while(savedHand.size() != 0){
        std::string holder = savedHand.substr(0, savedHand.find(delimiter));
        if (holder.length() == 4){
            doubleDigit = holder.substr(2,3);
            tempTile = new Tile(holder.at(0), stoi(doubleDigit));
        }
        else{
            tempTile = new Tile(holder.at(0), holder.at(2) - '0');
        }
        this->playersHand->addBack(tempTile);
        savedHand = savedHand.erase(0,  holder.size() + delimiter.size());
    }
}


std::string Player::getName(){ 
    return name;
}

void Player::setName(std::string n){
    name = n;
}

bool Player::getHasPassed() {
    return hasPassed;
}

void Player::setHasPassed(bool hasPassed) {
    this->hasPassed = hasPassed;
}