
#include <stdio.h>
#include "ListNode.h"

int main() {
	printf("Hello, World!\n");
	printf("%d\n\n", 5);

	Node* list_node = new_node(1);
	list_node->append(list_node, new_node(2));
	list_node->append(list_node, new_node(3));
	list_node->append(list_node, new_node(4));
	list_node->append(list_node, new_node(5));

	list_node->prepend(&list_node, new_node(0));
	list_node->prepend(&list_node, new_node(-1));

	int l = list_node->length(list_node);
	printf("%d", l);

	return 0;
}