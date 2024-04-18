#pragma once

typedef struct Node {
	// Fields
	int value;
	struct Node* prev;
	struct Node* next;

	// Function pointers
	int (*length)(struct Node* head);
	struct Node* (*first)(struct Node* node);
	struct Node* (*last)(struct Node* node);
	void (*append)(struct Node* head, struct Node* node);
	void (*prepend)(struct Node** head, struct Node* node);
	void (*insert_next)(struct Node* node, struct Node* head_insert);
	void (*at_index)(struct Node* head, int index);
	void (*insert_at)(struct Node* head, int index, struct Node* node_insert);
} Node;

// Node "constructor" and "destructor"
Node* new_node(int value);
void delete_nodes(Node* node);

// Node methods
int node_length(Node* head);
Node* node_first(Node* node);
Node* node_last(Node* node);
void node_append(Node* head, Node* node);
void node_prepend(Node** head, Node* node);
void node_insert_next(Node* node, Node* head_insert);
Node* node_at_index(Node* head, int index);
void node_insert_at(Node* head, int index, Node* node_insert);