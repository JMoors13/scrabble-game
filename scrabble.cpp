
#include "LinkedList.h"
#include "Player.h"
#include "Menu.h"
#include "Board.h"
#include "Student.h"

#include <iostream>
#include <fstream>
#include <string>

#define EXIT_SUCCESS 0

void gameMenu();

void newGame();

void continueTurns();

void saveGame(std::string fileName);

bool replaceTile(char c);

bool loadGame();

void gameEnd();

void gameQuit();

void credits();

int userSelect();

int playerCount = 0;

bool loadedFile = false;

std::vector<std::string> loadVector;

LinkedList *tileList = new LinkedList();

Player *player1 = new Player();
Player *player2 = new Player();

Board board;

Menu *menu = new Menu();

std::string fileName;
std::ifstream gameLoad;
std::string line;

int main(void)
{

   std::cout << "Welcome to Scrabble!" << std::endl;
   std::cout << "___________________" << std::endl;

   gameMenu();
   return EXIT_SUCCESS;

   delete tileList;
   delete player1;
   delete player2;
   delete menu;
}

void gameMenu()
{
   bool quit = false;

   do
   {
      std::cout << "Menu" << std::endl;
      std::cout << "____" << std::endl;
      std::cout << "1. New Game" << std::endl;
      std::cout << "2. Load Game" << std::endl;
      std::cout << "3. Credits" << std::endl;
      std::cout << "4. Quit" << std::endl;
      std::cout << std::endl;

      int choice = 0;
      choice = userSelect();
      if (choice == 1)
      {
         quit = true;
         newGame();
      }
      else if (choice == 2)
      {
         if (loadGame() == true)
         {
            continueTurns();
            quit = true;
         }
         else
         {
            std::cout << "Going back to main menu" << std::endl;
            gameMenu();
         }
      }
      else if (choice == 3)
      {
         credits();
      }
      else if (choice == 4)
      {
         gameQuit();
      }
      else
      {
         std::cout << "Please select one of the 4 options" << std::endl;
         std::cout << "> ";
      }
   } while (quit != true);
}

void newGame()
{
   tileList = tileList->newTileList();
   board.createDictionary();
   std::string tempName;
   bool name2Checker = false;

   std::cout << "Starting a New Game" << std::endl;
   std::cout << std::endl;

   player1->getPlayerHand()->colourChanger = 95; // Set Player Colours
   player2->getPlayerHand()->colourChanger = 96;

   player1->setName(player1->readNameIn(1)); // Read in name for player 1
   do
   {
      tempName = player2->readNameIn(2);
      if (tempName == player1->getName())
      { // Make sure player 2's name is no the same as player 1's
         std::cout << "Cannot have the same name as Player 1" << std::endl;
         name2Checker = false;
      }
      else
      {
         player2->setName(tempName);
         name2Checker = true;
      }
   } while (name2Checker == false); // Loop until player 2 enters a unique name

   board.setCurrentPlayer(player1->getName()); // Set current player to first player (first player always goes first)
   board.displayTurn();                        // Displays the current players turn

   player1->displayScore();
   player2->displayScore(); // Displays score

   board.NewBoard(); // Print empty scrabble board

   player1->genPlayersHand(tileList); // Generate the first 7 tiles for the players
   player2->genPlayersHand(tileList);
   player1->getPlayerHand()->printList(); // Print the first users hand for the initial turn

   continueTurns(); // Run this function to loop turns
}

void continueTurns()
{

   if (loadedFile == true)
   { // Set variables from loaded file
      player1->getPlayerHand()->colourChanger = 95;
      player2->getPlayerHand()->colourChanger = 96; // Set Player Colours
      player1->setName(loadVector.at(0));           // Set name for player 1
      player1->loadScore(stoi(loadVector.at(1)));   // Load score for player 1
      player1->updateHand(loadVector.at(2));        // Update player 1's hand

      player2->setName(loadVector.at(3));         // Set name for player 2
      player2->loadScore(stoi(loadVector.at(4))); // Load score for player 2
      player2->updateHand(loadVector.at(5));      // Update player 1's hand

      board.setCurrentPlayer(loadVector.at(24)); // Set current player

      std::cout << std::endl; // Spacer
      board.displayTurn();
      player1->displayScore();
      player2->displayScore(); // Displays scores

      board.UpdateBoard(loadVector);
      if (board.isEmpty() != true)
      {
         board.setFirstWord(); // Check if the board is empty and set the firstWord variable
      }

      tileList = tileList->updateTileList(loadVector.at(23)); // Update tilebag with saved game tiles

      if (board.getCurrentPlayer() == player1->getName())
      {
         player1->getPlayerHand()->printList();
      }
      else
      {
         player2->getPlayerHand()->printList();
      }
      loadedFile = false;
   }
   board.createDictionary(); // Create dictionary for the game
   bool pass = false;
   bool endTurn = false;
   bool quit = false;
   int skipper = 1;
   int tilesUsed;
   int tilesInHand;

   std::string tilePlacement;
   std::vector<std::string> placementArray;

   do
   {
      endTurn = false;
      if (skipper == 0)
      {
         if (board.getCurrentPlayer() == player1->getName())
         {                                              // If user 1, swap to user 2
            board.setCurrentPlayer(player2->getName()); // Set the users name
            board.displayTurn();                        // Display current users turn

            player1->displayScore();
            player2->displayScore(); // Displays scores
            board.PrintBoard();
            player2->getPlayerHand()->printList(); // Print the second users hand
         }
         else
         {
            board.setCurrentPlayer(player1->getName()); // Else, swap back to user 1
            board.displayTurn();                        // Display current users turn

            player1->displayScore();
            player2->displayScore(); // Displays scores
            board.PrintBoard();
            player1->getPlayerHand()->printList(); // Print the first users hand
         }
      }
      else
      {
         skipper = 0; // If the above has been skipped, reset the skipper variable to zero
      }
      placementArray.clear();
      do
      {
         tilePlacement.clear();
         if (board.getCurrentPlayer() == player1->getName())
         {
            tilePlacement = menu->enterTile(player1->getPlayerHand()); // Reads in user1 input for placement of tiles
         }
         else
         {
            tilePlacement = menu->enterTile(player2->getPlayerHand()); // Reads in user2 input for placement of tiles
         }
         if (tilePlacement == "done")
         { // If 'done', end turn
            endTurn = true;
         }
         else if (tilePlacement == "pass")
         { // If 'pass', clear tiles that might have been placed and check if there are any tiles ledt in the bag(two passes with no tiles in the bag means the game ends)
            endTurn = true;
            pass = true;
            placementArray.clear();
            if (tileList->isEmpty())
            {
               if (board.getCurrentPlayer() == player1->getName())
               {
                  if (player1->getHasPassed())
                  {
                     gameEnd();
                  }
                  player1->setHasPassed(true);
               }
               if (board.getCurrentPlayer() == player2->getName())
               {
                  if (player2->getHasPassed())
                  {
                     gameEnd();
                  }
                  player2->setHasPassed(true);
               }
            }
         }
         else if (tilePlacement == "help")
         { // If 'help', output help statements to terminal
            std::cout << "From this location you're able to:" << std::endl;
            std::cout << "- Place a tile: 'place A at A0'" << std::endl;
            std::cout << "- End your turn and place your word: 'place Done" << std::endl;
            std::cout << "- Replace a tile from your hand: 'replace A'" << std::endl;
            std::cout << "- Pass your turn to the next player: 'pass'" << std::endl;
            std::cout << "- Save the game at it's current state with a game name: 'save example'" << std::endl;
            std::cout << "-------------------------------------------------------------------" << std::endl;
         }
         else if (tilePlacement.substr(0, 7) == "replace")
         { // If 'replace', replace the tile given in the players hand and end turn
            char tileToReplace = tilePlacement.at(7);
            if (replaceTile(tileToReplace) == true)
            {
               endTurn = true;
               pass = true;
            }
            else
            {
               endTurn = false;
            }
            placementArray.clear();
         }
         else if (tilePlacement.substr(0, 4) == "save")
         { // If 'save', save game with the given name
            std::string fileName = tilePlacement.substr(4);
            saveGame(fileName);
         }
         else
         {                 // If none of the previous previous options, tilePlacement must be in tile/co-ordinate format
            tilesUsed = 0; // Reset values
            tilesInHand = 0;
            for (std::string i : placementArray)
            { // Check if the current letter entered is already in the placementArray to prevent duplicates from the same hand
               if (i.at(0) == tilePlacement.at(0))
               {
                  tilesUsed++;
               }
            }
            if (board.getCurrentPlayer() == player1->getName())
            {
               for (int i = 0; i != player1->getPlayerHand()->size(); ++i)
               {
                  if (player1->getPlayerHand()->getTileAt(i)->letter == tilePlacement.at(0))
                  {
                     tilesInHand++;
                  }
               }
            }
            else
            {
               for (int i = 0; i != player2->getPlayerHand()->size(); ++i)
               {
                  if (player2->getPlayerHand()->getTileAt(i)->letter == tilePlacement.at(0))
                  {
                     tilesInHand++;
                  }
               }
            }
            if (tilesUsed < tilesInHand)
            {                                           // Check if the player has picked too many letters of a specific letter
               placementArray.push_back(tilePlacement); // therwise, add the tile to the placement array vector
            }
            else
            {
               std::cout << "That tile has already been used" << std::endl; // Else don't add to the list
            }
         }
      } while (endTurn != true);
      if (pass != true)
      {
         if (board.placeTile(placementArray) == true)
         { // If valid tiles are entered, place them on the board
            if (board.getCurrentPlayer() == player1->getName())
            {
               if (placementArray.size() == 7)
               { // If the user places all 7 tiles from their hand, output bingo and add 50 points to their score
                  std::cout << std::endl;
                  std::cout << "BINGO!!!" << std::endl;
                  std::cout << std::endl;
                  player1->setScore(player1->getScore() + 50);
               }
            }
            if (board.getCurrentPlayer() == player2->getName())
            {
               if (placementArray.size() == 7)
               {
                  std::cout << std::endl;
                  std::cout << "BINGO!!!" << std::endl;
                  std::cout << std::endl;
                  player2->setScore(player2->getScore() + 50);
               }
            }

            for (std::string i : placementArray)
            { // Iterate through the placementArray and update score, remove tile from hand and replished used tiles
               if (board.getCurrentPlayer() == player1->getName())
               {
                  player1->updateScore(i.at(0));
                  player1->getPlayerHand()->removeTiles(i.at(0));
                  if (player1->getPlayerHand()->isEmpty())
                  {
                     gameEnd();
                  }
                  if (!tileList->isEmpty())
                  {
                     Tile *toAdd = tileList->getTile();
                     player1->getPlayerHand()->addBack(toAdd);
                  }
                  player1->setHasPassed(false);
               }
               else
               {
                  player2->updateScore(i.at(0));
                  player2->getPlayerHand()->removeTiles(i.at(0));
                  if (player2->getPlayerHand()->isEmpty())
                  {
                     gameEnd();
                  }
                  if (!tileList->isEmpty())
                  {
                     Tile *toAdd = tileList->getTile();
                     player2->getPlayerHand()->addBack(toAdd);
                  }
                  player2->setHasPassed(false);
               }
            }
         }
         else
         {
            board.PrintBoard(); // If the word wasn't place, print the board and the current players hand
            if (board.getCurrentPlayer() == player1->getName())
            {
               player1->getPlayerHand()->printList();
            }
            else
            {
               player2->getPlayerHand()->printList();
            }
            placementArray.clear();
            skipper = 1;
         }
      }
      else
      {
         pass = false;
      }
   } while (quit != true);
}

void saveGame(std::string fileName)
{ // Save function to save all variables

   std::ofstream gameSave(fileName + ".save");

   if (gameSave.is_open())
   {
      std::cout << std::endl;
      std::cout << "Game successfully saved" << std::endl;
      std::cout << std::endl;

      gameSave << player1->getName() << "\n";
      gameSave << player1->getScore() << "\n";
      for (int i = 0; i < player1->getPlayerHand()->size() - 1; i++)
      {
         gameSave << player1->getPlayerHand()->getNodeAt(i)->tile->letter << "-" << player1->getPlayerHand()->getNodeAt(i)->tile->value << ", ";
      }
      gameSave << player1->getPlayerHand()->getNodeAt(6)->tile->letter << "-" << player1->getPlayerHand()->getNodeAt(6)->tile->value;
      gameSave << "\n";

      gameSave << player2->getName() << "\n";
      gameSave << player2->getScore() << "\n";
      for (int i = 0; i < player2->getPlayerHand()->size() - 1; i++)
      {
         gameSave << player2->getPlayerHand()->getNodeAt(i)->tile->letter << "-" << player2->getPlayerHand()->getNodeAt(i)->tile->value << ", ";
      }
      gameSave << player2->getPlayerHand()->getNodeAt(6)->tile->letter << "-" << player2->getPlayerHand()->getNodeAt(6)->tile->value;
      gameSave << "\n";

      std::vector<std::string> saveBoard = board.getBoard();
      for (std::string i : saveBoard)
      {
         gameSave << i << "\n";
      }

      if (!tileList->isEmpty())
      {
         for (int i = 0; i < tileList->size() - 1; i++)
         {
            gameSave << tileList->getNodeAt(i)->tile->letter << "-" << tileList->getNodeAt(i)->tile->value << ", ";
         }
         gameSave << tileList->getNodeAt(tileList->size())->tile->letter << "-" << tileList->getNodeAt(tileList->size())->tile->value;
         gameSave << "\n";
      }
      else
      {
         gameSave << "\n";
      }

      gameSave << board.getCurrentPlayer();
   }
   else
   {
      std::cout << "Problem with saving file" << std::endl;
      ;
   }
   gameSave.close();
   gameQuit();
}

bool replaceTile(char tileToReplace)
{ // replaceTile method that checks the players hand for a tile, grabs anothr from the tilebag, and adds the removed tile to the back of the tilebag
   LinkedList *tempList;
   if (board.getCurrentPlayer() == player1->getName())
   {
      if (player1->getPlayerHand()->containsTile(tileToReplace) == true)
      {
         if (tileList->isEmpty() == false)
         {
            tempList = player1->getPlayerHand();
            Tile *toReplace = tempList->getTile(tileToReplace); // Get tile from the user hand that should be replaced
            tileList->addBack(toReplace);                       // Add the tile from the players hand to the tilebag
            Tile *toAdd = tileList->getTile();                  // Get the first tile from the tile bag
            player1->getPlayerHand()->addBack(toAdd);           // Add new tile to back of players hand
         }
         else
         {
            std::cout << "No tiles left in bag" << std::endl;
            return false;
         }
      }
      else
      {
         std::cout << "You don't have that tile in your hand" << std::endl;
         return false;
      }
   }
   else if (board.getCurrentPlayer() == player2->getName())
   {
      if (player2->getPlayerHand()->containsTile(tileToReplace) == true)
      {
         if (tileList->isEmpty() == false)
         {
            tempList = player2->getPlayerHand();
            Tile *toReplace = tempList->getTile(tileToReplace);
            tileList->addBack(toReplace);
            Tile *toAdd = tileList->getTile();
            player2->getPlayerHand()->addBack(toAdd);
         }
         else
         {
            std::cout << "No tiles left in bag" << std::endl;
            return false;
         }
      }
      else
      {
         std::cout << "You don't have that tile in your hand" << std::endl;
         return false;
      }
   }
   return true;
}

bool loadGame()
{ // Load game method which reads in the game variables
   bool exitBool = false;
   do
   {
      fileName.clear();
      std::cout << "Enter a filename to load" << std::endl;
      std::cout << "> ";
      getline(std::cin, fileName);

      if (std::cin.eof())
      {
         gameQuit();
      }
      if (fileName == "help")
      {
         std::cout << "From this location you're able to:" << std::endl;
         std::cout << "- Input a file name to read that save file: example" << std::endl;
         std::cout << "- Type 'back' to go back to the main menu" << std::endl;
         std::cout << "-------------------------------------------------------------------" << std::endl;
      }
      else if (fileName == "back")
      {
         exitBool = true;
         return false;
      }
      else
      {
         gameLoad.open(fileName + ".save");

         if (gameLoad.is_open())
         {
            loadedFile = true;
            exitBool = true;
            while (std::getline(gameLoad, line))
            {
               loadVector.push_back(line);
            }
         }
         else
         {
            std::cout << "No save file with that name" << std::endl;
            std::cout << std::endl;
         }
      }
   } while (exitBool == false);
   return true;
}

void credits()
{

   Student *student1 = new Student("Jacob Moors", "3607359", "s3607359@student.rmit.edu.au");
   Student *student2 = new Student("Zali Spurgeon", "3832759", "3832759@student.rmit.edu.au");
   Student *student3 = new Student("Timothy Parker", "3742101", "3742101@student.rmit.edu.au");

   std::cout << std::endl;
   std::cout << "----------------------------------" << std::endl;
   student1->printStudent();
   std::cout << std::endl;
   student2->printStudent();
   std::cout << std::endl;
   student3->printStudent();
   std::cout << std::endl;
   std::cout << "----------------------------------" << std::endl;
   std::cout << std::endl;
}

int userSelect()
{
   std::string menuOptionString;
   int menuOption = 0;

   do
   {
      menuOptionString.clear();
      std::cout << "> ";
      getline(std::cin, menuOptionString);
      if (std::cin.eof())
      {
         gameQuit();
      }
      if (menuOptionString == "help")
      {
         std::cout << "From this location you're able to:" << std::endl;
         std::cout << "- Input number 1 for a new game" << std::endl;
         std::cout << "- Input number 2 to load a game" << std::endl;
         std::cout << "- Input number 3 to show the game credits" << std::endl;
         std::cout << "- Input number 4 to quit the application" << std::endl;
         std::cout << "-------------------------------------------------------------------" << std::endl;
      }
      else if (menuOptionString.length() > 1)
      {
         std::cout << "Too many characters entered" << std::endl;
      }
      else if (menuOptionString.empty() == true)
      {
         std::cout << "Nothing entered, you don't want to do anything?" << std::endl;
      }
      else if (isdigit(menuOptionString.at(0)) == true)
      {
         menuOption = (menuOptionString.at(0) - '0');
         if (menuOption <= 4 && menuOption >= 1)
         {
            return menuOption;
         }
         else
         {
            std::cout << "Menu selection is out of bounds" << std::endl;
         }
      }
      else
      {
         std::cout << "Please enter a number" << std::endl;
      }

   } while (menuOption > 4 || menuOption < 1);
   return menuOption;
}

void gameEnd()
{ // Game end method which outputs the scores and declares a winner
   std::cout << "Game over" << std::endl;
   std::cout << "Score for " << player1->getName() << ": " << player1->getScore() << std::endl;
   std::cout << "Score for " << player2->getName() << ": " << player2->getScore() << std::endl;

   if (player1->getScore() > player2->getScore())
   {
      std::cout << "Player " << player1->getName() << " won!" << std::endl;
   }
   else
   {
      std::cout << "Player " << player2->getName() << " won!" << std::endl;
   }
   std::cout << std::endl;

   gameQuit();
}

void gameQuit()
{ // Prints goodbye and exits the application
   std::cout << "Goodbye" << std::endl;
   exit(1);
}
