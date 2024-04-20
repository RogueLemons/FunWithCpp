
#include <stdio.h>
#include "ListNode.h"

void print_node_list(Node* head) {
	if (head == NULL) {
		printf("List is Empty: [ ]\n");
		return;
	}

	printf("Doubly Linked List: [ ");
	Node* temp = head;
	while (temp->next != NULL) {
		printf("%d, ", temp->value);
		temp = temp->next;
	}
	printf("%d ]\n", temp->value);
}

int main() {
	printf("---Testing List of Nodes---\n");

	printf("\nAppend and Prepend nodes:\n");
	Node* linked_nodes = new_node(1);
	linked_nodes->append(linked_nodes, new_node(2));
	linked_nodes->append(linked_nodes, new_node(3));
	linked_nodes->append(linked_nodes, new_node(4));
	linked_nodes->append(linked_nodes, new_node(5));
	linked_nodes->prepend(&linked_nodes, new_node(0));
	linked_nodes->prepend(&linked_nodes, new_node(-1));
	print_node_list(linked_nodes);

	int index = 2;
	printf("\nElement at index %d is:\t%d \n", index, node_at_index(linked_nodes, index)->value);
	printf("The first element is:\t%d \n", node_first(linked_nodes)->value);
	printf("The last element is:\t%d \n", node_last(linked_nodes)->value);

	printf("\nFree up memory from third and subsequent nodes:\n");
	delete_nodes(linked_nodes->next->next);
	print_node_list(linked_nodes);

	printf("\nInsert three 3s after the first node:\n");
	Node* more_nodes = new_node(3);
	node_append(more_nodes, new_node(3));
	node_append(more_nodes, new_node(3));
	linked_nodes->insert_next(linked_nodes, more_nodes);
	print_node_list(linked_nodes);

	int value = 9;
	printf("\nInsert %d at index %d:\n", value, index);
	node_insert_at(linked_nodes, index, new_node(value));
	print_node_list(linked_nodes);

	delete_nodes(linked_nodes);
	return 0;
}