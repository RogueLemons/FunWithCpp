#include "ListNode.h"
#include <stdio.h>

Node* new_node(int value)
{
	Node* new_node = (struct Node*) malloc(sizeof(Node));
	new_node->value = value;
	new_node->prev = NULL;
	new_node->next = NULL;
	return new_node;
}


