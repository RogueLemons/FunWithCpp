#pragma once

typedef struct {
	// Fields
	int value;
	Node* prev;
	Node* next;

	// Function pointers
	int (*length)(Node* head);
	void (*append)(Node* head, Node* node);
	void (*prepend)(Node* head, Node* node);
} Node;

// Node "constructor" and "destructor"
Node* new_node(int value);
void delete_node(Node* node);

// Node methods
int node_length(Node* head);
void node_append(Node* head, Node* node);
void node_prepend(Node* head, Node* node);