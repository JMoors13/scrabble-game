
#ifndef ASSIGN2_LINKEDLIST_H
#define ASSIGN2_LINKEDLIST_H

#include "Node.h"
#include "Tile.h"

#include <iostream>
#include <string>
#include <vector>

class LinkedList {
public:
   int colourChanger;

   LinkedList();
   LinkedList(const LinkedList& other);
   ~LinkedList();

   int size();
   void clear();

   Tile* getTileAt(int index);

   Node* getNodeAt(int index);

   Tile* getTile(char letter);

   Tile* getTile();

   // bool moreTiles();

   Node* getNode();

   bool containsTile(char letter);

   void addBack(Tile* tile);

   void removeTiles(char letter);

   LinkedList* getHand();

   void printList();

   bool isEmpty();

   LinkedList* updateTileList(std::string tilebag);

   LinkedList* newTileList();

private:
   std::vector<std::string> tileVector;

   Node* head;

   Tile* tempTile;

   int length;

};

#endif // ASSIGN2_LINKEDLIST_H
