//
// Created by liuzheng on 17-5-14.
//

#include <stdlib.h>
#include <string.h>
#include "onlinelinklist.h"


Node* initonlinelist()
{
    head = (Node *)malloc(sizeof(Node));

    return head;
}
Node* find(char *id)
{
    Node *node = head->next;

    while(node)
    {
        if(strcmp(node->user.id, id) == 0)
            return node;
        node = node->next;
    }

    return NULL;
}


bool del(char *id)
{
    Node *node = head->next;
    Node *before = head;
    while(node)
    {
        if(strcmp(node->user.id, id) == 0)
        {
            before->next = node->next;
            free(node);
            node = NULL;
        }
        else
        {
            before = node;
            node = node->next;
        }
    }

    return NULL;
}


bool insert(User user)
{
    Node *n = (Node *)malloc(sizeof(Node));

    if(n == NULL)
        return false;

    strcpy(n->user.id, user.id);
    strcpy(n->user.username, user.username);
    strcpy(n->user.ip, user.ip);

    n->next = head->next;
    head->next = n;
}



void clear()
{
    Node *first;

    while(head->next)
    {
        first = head->next;
        head->next = first->next;

        free(first);

        first = NULL;
    }
}
