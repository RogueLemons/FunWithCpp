#include "ListNode.h"
#include <stdio.h>

Node* new_node(int value)
{
	Node* new_node = (struct Node*) malloc(sizeof(Node));
	new_node->value = value;
	new_node->prev = NULL;
	new_node->next = NULL;

	new_node->length = node_length;

	return new_node;
}

void delete_node(Node* node)
{
	Node* current = node;
	Node* next = node->next;
	while (next != NULL) {
		next = current->next;
		free(current);
	}
	free(current);
}

int node_length(Node* head)
{
	if (head == NULL) {
		return 0;
	}
	int l = 1;
	Node* temp = head;
	while (temp->next != NULL) {
		l++;
		temp = temp->next;
	}

	return l;
}

void node_append(Node* head, Node* node)
{
	Node* temp = head;
	while (temp->next != NULL) {
		temp = temp->next;
	}
	temp->next = node;
}

void node_prepend(Node* head, Node* node)
{
	Node* temp = head;
	while (temp->prev != NULL) {
		temp = temp->prev;
	}
	temp->prev = node;
	node->next = temp;
}


