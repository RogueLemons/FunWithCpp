#pragma once

typedef struct {
	// Fields
	int value;
	Node* prev;
	Node* next;

	// Function pointers
	int (*length)(Node* head);
} Node;

// Node "constructor" and "destructor"
Node* new_node(int value);
void delete_node(Node* node);

// Node methods
int node_length(Node* head);