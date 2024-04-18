#pragma once

typedef struct {
	// Fields
	int value;
	struct Node* prev;
	struct Node* next;

	// Function pointers

} Node;

// Node "constructor"
Node* new_node(int value);

// Node functions
