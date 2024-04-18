
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
	Node* list_node = new_node(1);
	list_node->append(list_node, new_node(2));
	list_node->append(list_node, new_node(3));
	list_node->append(list_node, new_node(4));
	list_node->append(list_node, new_node(5));
	list_node->prepend(&list_node, new_node(0));
	list_node->prepend(&list_node, new_node(-1));
	print_node_list(list_node);

	int index = 2;
	printf("\nElement at index %d is:\t%d \n", index, node_at_index(list_node, index)->value);
	printf("The first element is:\t%d \n", node_first(list_node)->value);
	printf("The last element is:\t%d \n", node_last(list_node)->value);

	printf("\nFree up memory from third and subsequent nodes:\n");
	delete_nodes(list_node->next->next);
	print_node_list(list_node);

	printf("\nInsert three 3s after the first node:\n");
	Node* more_nodes = new_node(3);
	node_append(more_nodes, new_node(3));
	node_append(more_nodes, new_node(3));
	list_node->insert_next(list_node, more_nodes);
	print_node_list(list_node);

	int value = 9;
	printf("\nInsert %d at index %d:\n", value, index);
	node_insert_at(list_node, index, new_node(value));
	print_node_list(list_node);

	return 0;
}