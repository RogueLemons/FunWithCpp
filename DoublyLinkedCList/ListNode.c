#include "ListNode.h"
#include <stdio.h>
#include <stdlib.h>

Node* new_node(int value)
{
	Node* new_node = (struct Node*) malloc(sizeof(Node));
	new_node->value = value;
	new_node->prev = NULL;
	new_node->next = NULL;

	new_node->length = node_length;
	new_node->first = node_first;
	new_node->last = node_last;
	new_node->append = node_append;
	new_node->prepend = node_prepend;
	new_node->insert_next = node_insert_next;
	new_node->at_index = node_at_index;
	new_node->insert_at = node_insert_at;

	return new_node;
}

void delete_nodes(Node* node)
{
	Node* current = node;
	Node* next = node->next;
	while (next != NULL) {
		current = next;
		next = current->next;
		free(current);
	}
	if (node->prev != NULL) {
		node->prev->next = NULL;
	}
	free(node);
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

Node* node_first(Node* node)
{
	Node* temp = node;
	while (temp->prev != NULL) {
		temp = temp->prev;
	}
	return temp;
}

Node* node_last(Node* node)
{
	Node* temp = node;
	while (temp->next != NULL) {
		temp = temp->next;
	}
	return temp;
}

void node_append(Node* head, Node* node)
{
	Node* temp = head;
	while (temp->next != NULL) {
		temp = temp->next;
	}
	temp->next = node;
}

void node_prepend(Node** head, Node* node)
{
	Node* temp = *head;
	while (temp->prev != NULL) {
		temp = temp->prev;
	}
	temp->prev = node;
	node->next = temp;
	*head = node;
}

void node_insert_next(Node* node, Node* head_insert)
{
	if (node->next == NULL) {
		node->next = head_insert;
		return;
	}
	Node* temp = head_insert;
	while (temp->next != NULL) {
		temp = temp->next;
	}
	temp->next = node->next;
	head_insert->prev = node;
	node->next = head_insert;
}

Node* node_at_index(Node* head, int index)
{
	Node* temp = head;
	for (int i = 0; i < index; i++) {
		temp = temp->next;
	}
	return temp;
}

void node_insert_at(Node* head, int index, Node* node_insert)
{
	Node* temp = node_at_index(head, index - 1);
	node_insert_next(temp, node_insert);
}


