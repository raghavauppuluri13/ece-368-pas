#include "delay.h"

// STACK OPERATIONS
void stack_push(StackNode** head, void* data_addr) {
  StackNode* new_item = malloc(sizeof(*new_item));
  *new_item = (StackNode) { .next = *head, .data_addr = data_addr };
  *head = new_item;
}

void* stack_pop(StackNode** head) {
  StackNode* temp = *head;
  void* data_addr = NULL;
  if(temp) {
    data_addr = temp->data_addr;
    *head = temp->next;
    free(temp);
  }
  return data_addr;
}

void* stack_stackpop(StackNode** head) {
  void* item = stack_pop(head);
  stack_push(head, item);
  return item;
}

void destroy_stack(StackNode** head) {
  StackNode* walker = *head;
  while(walker != NULL) {
    StackNode* temp = walker; 
    walker = walker->next;
    free(temp);
  }
  *head = NULL;
}

void print_stack(StackNode** head) {
  StackNode** walker = head;
  while(*walker != NULL) {
    print_rc_data((*walker)->data_addr);
    walker = &((*walker)->next);
  }
}
