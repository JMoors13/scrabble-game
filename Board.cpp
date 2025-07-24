#include "Board.h"
#include "Player.h"

Board::Board(){

}

Board::~Board(){

}

void Board::NewBoard(){

    std::ifstream boardArray;
    std::string line;

    boardArray.open("ScrabbleBase.txt");                                                                                        //Print board display to terminal
    if (!boardArray.is_open()) {
       std::cout << "File not open" << std::endl;
    }

    if (boardArray.is_open()) {
        while (getline (boardArray, line)){                                                                                     //Add each line to the vector
        boardVector.push_back(line);
        }
    }
    else{
        std::cerr << "Unable to open file datafile.txt";
        exit(1);                                                                                                                //Call system to stop
    }
    for (std::string i: boardVector){                                                                                           //Prints the new board
      std::cout << i << std::endl;
    }
}

void Board::PrintBoard(){
    for (std::string boardRow: boardVector){                                                                                    //Prints the board

      for (long unsigned int boardChar = 0; boardChar < boardRow.length(); boardChar++){                                        //Iterate through the board rows and check if there is a letter place on the board
          if (boardChar > 0 && isalpha(boardRow.at(boardChar))){
            std::cout <<"\033[32m" << boardRow.at(boardChar) << "\033[0m";                                                      //If a letter is found, print it in colour
          }
          else{
            std::cout << boardRow.at(boardChar);                                                                                //therwise print without colour
          }
        }
        std::cout<<std::endl;
    }
}

void Board::UpdateBoard(std::vector<std::string> loadVector){
    for(int i = 6; i < 23; i++){
        boardVector.push_back(loadVector.at(i));                                                                                //Load board from a save file
    }
    for (std::string boardRow: boardVector){                                                                                    

      for (long unsigned int boardChar = 0; boardChar < boardRow.length(); boardChar++){
          if (boardChar > 0 && isalpha(boardRow.at(boardChar))){
            std::cout <<"\033[32m" << boardRow.at(boardChar) << "\033[0m";
          }
          else{
            std::cout << boardRow.at(boardChar);
          }
        }
        std::cout<<std::endl;
    }

}

std::vector<std::string> Board::getBoard(){
    return boardVector;
}

void Board::setFirstWord(){                                                                                                     //Sets the firstWord varaible in the case of a save game that has no tiles placed
    firstWord = 1;

}

bool Board::isEmpty(){
    for (std::string row: boardVector){                                                                                         //Prints the updated board
        for (long unsigned int e = 1; e < row.length(); e++ ){
            if(isalpha(row.at(e))){
                return false;
            }
        }    
    }
    return true;
}

void Board::createDictionary(){
    std::ifstream dictionaryList;
    std::string line;

    dictionaryList.open("words.txt");

    if (dictionaryList.is_open()) {
        while (std::getline(dictionaryList, line)) {
            std::transform(line.begin(), line.end(), line.begin(), ::toupper);
            dictionary.push_back(line);
        }
    } else {
        std::cerr << "Unable to open file datafile.txt";
        exit(1);
    }
   
    dictionaryList.close();
}            

bool Board::placeTile(std::vector<std::string> placementArray){
    bool validTile = true;
    boardVectorPrevious = boardVector;
        for (std::string rawMove: placementArray){                                                                              //Iterate through the placementArray to place each tile 
            boardRow.clear(); 
            yaxis = 0;                                                                                                          //Reset all the int values for the next tile
            xaxis = 0; 
            counter = 0;
            if (rawMove.size() == 5){                                                                                           //Find the x- co-ordinate for this tile
                char c1 = rawMove.at(3);                                                                                        //Checking for a double value
                char c2 = rawMove.at(4);
                stoiVar = std::string(1, c1)+c2;
                xaxis = std::stoi (stoiVar);
            }
            else{
                xaxis = rawMove.at(3) - '0';                                                                                    //If not, must be a single value
            }
            if(xaxis > 15 || xaxis < 0){                                                                                        
                validTile = false;
                std::cout << "Outside of board area (x-axis)" << std::endl;
                return validTile;
            }
            yordChar = rawMove.at(2);
            tileLetter = rawMove.at(0);
            for (std::string i: boardVector){                                                                                   //Find the y co-ordinate for the tile
                if(yordChar == i.at(0)){                                                                                        //Iterate through each row's first letter and see if that equals the y co-ord        
                    yaxis += counter;                                                                                           //Set the y value to the value of all the previous attempts before it
                }
                else{
                    counter++;
                }
            }
            if (counter == 17){                                                                                                 //Add a checker to find the if the player has gone outside the board size
                validTile = false;
                std::cout << "Outside of board area (y-axis)" << std::endl;
                return validTile;
            }                  
            boardRow = boardVector.at(yaxis);                                                                                   //Get row from boardVector at yaxis location

                if(boardRow.at(4+(4*xaxis)) == space){                                                                          //Check if the location to update is empty before replacing it with the tile
                    boardRow.at(4+(4*xaxis)) = tileLetter;                                                                      //Set the location to the tile
                }
                else{
                    std::cout << "Cannot place a tile on an occupied location" << std::endl;                                    //If not empty, throw a error and ask for another word
                    validTile = false;
                    boardVector = boardVectorPrevious;
                    return validTile;
                }     
            boardVector.at(yaxis) = boardRow;

        }
    if(placeChecker(placementArray) == false){                                                                                  //Used to see if the word placing is valid
        std::cout << "Word must be placed next to another word" << std::endl;
        validTile = false;
        boardVector = boardVectorPrevious;
        return validTile;
    }    

    if(wordChecker(placementArray) == false){                                                                                   //Cross references the words made with the words list to see if its a valid word
        std::cout << "That word is not in the Scrabble Dictionary" << std::endl;
        validTile = false;
        boardVector = boardVectorPrevious;
        return validTile;
        }


    if(validTile != false && firstWord == 0){                                                                                   //If valid and its the first word, change the variable so no other word will be classified as the first word
        firstWord = 1;
    }

    std::cout << std::endl;                                                                                                     
    return validTile;
}

bool Board::placeChecker(std::vector<std::string> placementArray){

    for (std::string moveCheck: placementArray){
        wordToTest.clear();
        yaxis = 0;
        xaxis = 0;
        counter = 0;
        if (moveCheck.size() == 5){
            char c1 = moveCheck.at(3);
            char c2 = moveCheck.at(4);
            stoiVar = std::string(1, c1)+c2;
            xaxis = std::stoi (stoiVar);
        }
        else{
            xaxis = moveCheck.at(3) - '0';
        }
        yordChar = moveCheck.at(2);    
        tileLetter = moveCheck.at(0);

        for (std::string i: boardVector){
            if(yordChar == i.at(0)){        
                yaxis += counter;      
            }
            else{
                counter++;
            }
        }
        boardRow = boardVector.at(yaxis);
        if(yaxis != 16){
            belowTile = boardVector.at(yaxis + 1);
        }
        else{
            belowTile = boardVector.at(1);
        }
        aboveTile = boardVector.at(yaxis - 1);

        xCopy = xaxis;                                                                                                          //set a copy of x axis
        yCopy = yaxis;                                                                                                          //set a copy of y axis        
        while(xCopy <= 14 && boardRow.at(4+(4*(xCopy + 1))) != space){                                                          //Checking the right of the current tile
            wordToTest = wordToTest + boardRow.at(4+(4*(xCopy + 1)));
            if(xCopy < 15){
                xCopy++;
            }
        }
        xCopy = xaxis;                                                                                                          //Reset x value
        wordToTest = boardRow.at(4+(4*(xaxis))) + wordToTest;

        while(boardRow.at(4+(4*(xCopy - 1))) != space && xCopy > 0){                                                            //Checking the left of the current tile
            wordToTest = boardRow.at(4+(4*(xCopy - 1))) + wordToTest;
            if(xCopy > 0){
                xCopy--;
            }
        }

        if(wordToTest.length() > placementArray.size()){
            return true;
        }
        else if(firstWord == 0){
            if(wordToTest.length() == placementArray.size()){
                return true;
            }
        }
        wordToTest.clear();

        while(belowTile.at(4+(4*(xaxis))) != space  && yCopy < 16){                                                             //Checking below of the current tile
            wordToTest = wordToTest + belowTile.at(4+(4*(xaxis)));
            yCopy++;
            if(yCopy < 16){
                belowTile = boardVector.at(yCopy + 1);
            }
        }
        yCopy = yaxis;                                                                                                          //reset y value
        wordToTest = boardRow.at(4+(4*(xaxis))) + wordToTest;

        while(aboveTile.at(4+(4*(xaxis))) != space && yCopy >= 3){                                                              //Checking above of the current tile
            wordToTest = aboveTile.at(4+(4*(xaxis))) + wordToTest;
            yCopy--;
            aboveTile = boardVector.at(yCopy - 1);
        }

        if(wordToTest.length() > placementArray.size()){
            return true;
        }
        else if(firstWord == 0){
            if(wordToTest.length() == placementArray.size()){
                return true;
            }
        }
    }
    return false;
}

bool Board::wordChecker(std::vector<std::string> placementArray){

    for (std::string moveValidate: placementArray){
        wordToTest.clear();
        yaxis = 0;
        xaxis = 0;
        counter = 0;
        if (moveValidate.size() == 5){  
            char c1 = moveValidate.at(3);
            char c2 = moveValidate.at(4);
            stoiVar = std::string(1, c1)+c2;
            xaxis = std::stoi (stoiVar);
        }
        else{
            xaxis = moveValidate.at(3) - '0';
        }
        yordChar = moveValidate.at(2); 
        tileLetter = moveValidate.at(0);
        for (std::string i: boardVector){
            if(yordChar == i.at(0)){        
                yaxis += counter;      
            }
            else{
                counter++;
            }
        }
        boardRow = boardVector.at(yaxis);
        xCopy = xaxis;
        while(xCopy <= 14 && boardRow.at(4+(4*(xCopy + 1))) != space){                                                          //Checking the right of the current tile
            wordToTest = wordToTest + boardRow.at(4+(4*(xCopy + 1)));                                                           //Add tile to string and continue adding until you find a space
            if(xCopy < 15){                                                                                                     //Checker to stay in the boards area
                xCopy++;
            }
        }
        xCopy = xaxis;
        wordToTest = boardRow.at(4+(4*(xaxis))) + wordToTest;

        while(boardRow.at(4+(4*(xCopy - 1))) != space &&  xCopy > 0){                                                           //Checking the left of the current tile
            wordToTest = boardRow.at(4+(4*(xCopy - 1))) + wordToTest;
            if(xCopy > 0){
                xCopy--;
            }
        }
        if(wordToTest.length() > 1){
            if (std::find(dictionary.begin(), dictionary.end(), wordToTest) != dictionary.end()){                               //Check if the word that is formed horizontally is in the word list
            }
            else{
                return false;
           }
        }

        wordToTest.clear();                                                                                                     //Clear the 'wordToTest' varaible and start checking vertically

        if(yaxis != 16){                                                                                                        //Check if the y co-ordinate is at max
            belowTile = boardVector.at(yaxis + 1);
        }
        else{
            belowTile = boardVector.at(1);                                                                                      //If y is max, set belowTile to a row which has no spaces and allow other clause to prevent the statement
        }
        aboveTile = boardVector.at(yaxis - 1);
        yCopy = yaxis;                                                                                                          //set a copy of y axis

        while(belowTile.at(4+(4*(xaxis))) != space  && yCopy < 16){                                                             //Checking below of the current tile
            wordToTest = wordToTest + belowTile.at(4+(4*(xaxis)));
            yCopy++;
            if(yCopy < 16){
                belowTile = boardVector.at(yCopy + 1);
            }
        }
        yCopy = yaxis;                                                                                                          //Reset y value
        wordToTest = boardRow.at(4+(4*(xaxis))) + wordToTest;

        while(aboveTile.at(4+(4*(xaxis))) != space  && yCopy >= 3){                                                             //Checking above of the current tile
            wordToTest = aboveTile.at(4+(4*(xaxis))) + wordToTest;
            yCopy--;
            aboveTile = boardVector.at(yCopy - 1);
        }

        
        if(wordToTest.length() > 1){
            if (std::find(dictionary.begin(), dictionary.end(), wordToTest) != dictionary.end()){                               //Check if the word that is formed vertically is in the word list
            }
            else{
                return false;
           }
        }
    }
    return true;
}

void Board::displayTurn(){                                                                                                      //Output the current player to terminal
    std::cout << currentPlayer << ", it's your turn"<< std::endl;
}

std::string Board::getCurrentPlayer(){                                                                                          //Return the current player
    return currentPlayer;
}

void Board::setCurrentPlayer(std::string name){                                                                                 //Sets the current player in the board class
    currentPlayer = name;
} 