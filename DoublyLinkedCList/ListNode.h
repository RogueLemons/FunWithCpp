#pragma once

typedef struct Node {
	// Fields
	int value;
	struct Node* prev;
	struct Node* next;

	// Function pointers
	int (*length)(struct Node* head);
	void (*append)(struct Node* head, struct Node* node);
	void (*prepend)(struct Node** head, struct Node* node);
	void (*insert_next)(struct Node** node, struct Node* head_insert);
} Node;

// Node "constructor" and "destructor"
Node* new_node(int value);
void delete_nodes(Node* node);

// Node methods
int node_length(Node* head);
void node_append(Node* head, Node* node);
void node_prepend(Node** head, Node* node);
void node_insert_next(Node* node, Node* head_insert);