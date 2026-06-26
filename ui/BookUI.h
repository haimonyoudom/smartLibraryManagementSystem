#ifndef BOOK_UI_H
#define BOOK_UI_H

#include "../linked_list/LinkedList.h"
#include "../hash_table/hashTable.h"
#include "../queue/Queue.h"
#include "../tree/tree.h"
#include "../stack/Stack.h"

void bookMenu(LinkedList& books, HashTable& ht, Queue& queue,
              BST& categories, Stack& history);

#endif