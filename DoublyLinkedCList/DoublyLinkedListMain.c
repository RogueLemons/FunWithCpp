
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
	printf("---Testing List of Nodes---\n\n");

	Node* list_node = new_node(1);
	list_node->append(list_node, new_node(2));
	list_node->append(list_node, new_node(3));
	list_node->append(list_node, new_node(4));
	list_node->append(list_node, new_node(5));

	list_node->prepend(&list_node, new_node(0));
	list_node->prepend(&list_node, new_node(-1));

	print_node_list(list_node);
	delete_nodes(list_node->next->next);
	print_node_list(list_node);

	return 0;
}