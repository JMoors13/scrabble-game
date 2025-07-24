#include "LinkedList.h"

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <random>

LinkedList::LinkedList() {
   head = nullptr;
   length = 0;
}

LinkedList::LinkedList(const LinkedList& other) 
{
   Node* headNode = new Node(other.head->tile, other.head->next);
   while (headNode != nullptr) {
      addBack(headNode->tile);
      headNode = headNode->next;
   }
}

LinkedList::~LinkedList() {
   Node* next = nullptr;
   if (head != nullptr) {
      Node* current = head;
      while(current != nullptr) {
         next = current->next;
         free(current);
         current = next;
      }
      head = nullptr;
      this->length = 0;
   }
}

int LinkedList::size() {
   return length;
}

void LinkedList::clear() {
   if (head == nullptr) {
      std::cout << "List empty!" << std::endl;
   }
   else {
      Node* current = head;
      while(current->next != nullptr) {
         current = nullptr;
         current = current->next;
      }
      head = nullptr;
      this->length = 0;
   }
}


Tile* LinkedList::getTile() {
   Tile* returnTile = head->tile;
   head = head->next;
   length--;
   return returnTile;
}

Tile* LinkedList::getTile(char letter) {
   Node* current = head;                                                                                                         //Store the head node
   Node* prev = nullptr;

   if(current->tile->letter == letter) {
      head = head->next;                                                                                                         //Change old head
      length--;
      return head->tile;
   }
      while (current->next != nullptr && current->tile->letter != letter){                                                       //While the next tile in the list is not null and not the letter we are after
         prev = current;                                                                                                         //Move to the next tile to be checked
         current = current->next;
      }
      if(current != nullptr && current->tile->letter == letter){                                                                 //If the tile is found
         prev->next = current->next;                                                                                             
         length--;
         return current->tile;                                                                                                   //Return the found tile 
      }
   return nullptr;
}

Node* LinkedList::getNode() {
   Node* returnNode = head;
   return returnNode;
}

Tile* LinkedList::getTileAt(int index) {
   int count = 0;
   Node* current = head;
   Tile* returnvalue = nullptr;
   if (index >= 0 && index < size()) {
      while(count < index) {
         ++count;
         current = current->next;
      }
      returnvalue = current->tile;
   }
   return returnvalue;
}

Node* LinkedList::getNodeAt(int index) {
   int count = 0;
   Node* current = head;
   if (index >= 0 && index < size()) {
      while(count < index) {
         ++count;
         current = current->next;
      }
   }
   return current;
}

bool LinkedList::containsTile(char letter) {
   Node* current = head;

   if(head->tile->letter == letter) {
         return true;
      }

   while (current->next != nullptr && current->tile->letter != letter){
      current = current->next;
      
      if(current->tile->letter == letter){
      return true;
      }
   }
   return false;
}

void LinkedList::addBack(Tile* tile) {
   Node* toAdd = new Node(tile, nullptr);
   if (head == nullptr) {
      head = toAdd;
      this->length++;
   }
   else {
      Node* current = head;
      while(current->next != nullptr) {
         current = current->next;
      }
      current->next = toAdd;
      this->length++;
   }
}

void LinkedList::removeTiles(char letter){
   Node* current = head;
   Node* prev = nullptr;

   if(head->tile->letter == letter) {
         head = head->next;
         length--;
         return;
      }

   while (current->next != nullptr && current->tile->letter != letter){
      prev = current;
      current = current->next;

      if(current != nullptr && current->tile->letter == letter){ 
      prev->next = current->next;
      length--;
      return;
      }
   }
}

void LinkedList::printList() {                                                                                                   //Print the players hand
   Node* temp = head;
   if (head == nullptr) {
      std::cout << "List empty!" << std::endl;
   }
   else {
      std::cout << std::endl;
      std::cout << "Your hand is" << std::endl;
      while(temp->next != nullptr){                                                                                              //While temp->next doesnt equal NULL, print the value of that tile and set the new temp tile to that next node
      std::cout << "\033[" << colourChanger << "m" << temp->tile->letter  << "-"  << temp->tile->value << ", ";
      temp = temp->next;
   }
   std::cout << temp->tile->letter << "-"  << temp->tile->value << "\033[0m" << std::endl;
   std::cout << std::endl;
   }
}

LinkedList* LinkedList::updateTileList(std::string tilebag){
   LinkedList* list = new LinkedList();
   std::string delimiter = ", ";
   std::string doubleDigit;

   while(tilebag.size() != 0){

      std::string holder = tilebag.substr(0, tilebag.find(delimiter));

      if (holder.length() == 4){
         doubleDigit = holder.substr(2,3);
         tempTile = new Tile(holder.at(0), stoi(doubleDigit));
         }
      else{
         tempTile = new Tile(holder.at(0), holder.at(2) - '0');
      }
      list->addBack(tempTile);
      tilebag = tilebag.erase(0, holder.size() + delimiter.size());
   }
   return list;
}

LinkedList* LinkedList::newTileList() {

   LinkedList* list = new LinkedList();

   std::ifstream tileBag;
   std::string line;

   tileBag.open("ScrabbleTiles.txt");

   if (tileBag.is_open()) {
      while (std::getline(tileBag, line)) {
         tileVector.push_back(line);
      }
   } else {
      std::cerr << "Unable to open file datafile.txt";
      exit(1); 
   }
   
   tileBag.close();

   auto dre = std::default_random_engine {};
   dre.seed(time(0));
   std::shuffle(std::begin(tileVector), std::end(tileVector), dre);

   for (std::string i: tileVector){
      Tile* tempTile = new Tile(i.at(0), std::stoi(i.substr(1, 3)));
      list->addBack(tempTile);
   }

   return list;
}

bool LinkedList::isEmpty() {
   bool returnBool;
   
   if (head == nullptr) {
      returnBool = true;
   } else {
      returnBool = false;
   }
   
   return returnBool;
}