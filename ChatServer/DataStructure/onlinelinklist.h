//
// Created by liuzheng on 17-5-14.
// this link list may be replaced with skiplist
//

#ifndef CHATSERVER_ONLINELINKLIST_H
#define CHATSERVER_ONLINELINKLIST_H

#include <stdbool.h>
#include "../MessageStructure/user.h"

typedef struct _Node{
    User user;
    struct _Node *next;
}Node;

Node *head;

Node* initonlinelist();

Node* find(char *id);


bool del(char *id);

bool insert(User user);

void clear();
#endif //CHATSERVER_ONLINELINKLIST_H
