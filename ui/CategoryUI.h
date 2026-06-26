#ifndef CATEGORY_UI_H
#define CATEGORY_UI_H

#include "../linked_list/LinkedList.h"
#include "../hash_table/hashTable.h"
#include "../queue/Queue.h"
#include "../tree/tree.h"
#include "../stack/Stack.h"

void categoryMenu(LinkedList& books, HashTable& ht, Queue& queue,
                  BST& categories, Stack& history);

#endif