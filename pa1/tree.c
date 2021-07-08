#include "delay.h"

// TREE OPERATIONS
void insert_right(TreeNode** parent, RCData* data_addr) {
  TreeNode* new_item = malloc(sizeof(*new_item));
  *new_item = (TreeNode) { .left = NULL, .right = NULL, .data_addr = data_addr, .parent=*parent };
  (*parent)->right = new_item; 
}

void insert_left(TreeNode** parent, RCData* data_addr) {
  TreeNode* new_item = malloc(sizeof(*new_item));
  *new_item = (TreeNode) { .left = NULL, .right = NULL, .data_addr = data_addr, .parent=*parent };
  (*parent)->left = new_item; 
}

void print_tree(TreeNode** root) {
  if(*root != NULL) {
    print_rc_data((*root)->data_addr);
    print_tree(&((*root)->left)); 
    print_tree(&((*root)->right)); 
  }
}

void destroy_tree(TreeNode** root) {
  if(*root != NULL) {
    destroy_tree(&((*root)->left)); 
    destroy_tree(&((*root)->right)); 
    free((*root)->data_addr);
    free(*root);
  }
}

// RC TREE GENERATION 

static void _calculate_r_c(TreeNode** curr, RCParams* params) {
  double len = ((*curr) == (*curr)->parent->left) ? (*curr)->parent->data_addr->data.nonleaf.left_len : (*curr)->parent->data_addr->data.nonleaf.right_len;
  (*curr)->data_addr->r_i = params->r_unit * len; 
  (*curr)->data_addr->c_i = 0.5 * params->c_unit * len;
  if((*curr)->data_addr->is_leaf) { // leaf
    (*curr)->data_addr->c_i = (*curr)->data_addr->c_i + (*curr)->data_addr->data.leaf.sink_c; 
  }
  else { // non-leaf
    (*curr)->data_addr->c_i = (*curr)->data_addr->c_i + 0.5 * params->c_unit * ((*curr)->data_addr->data.nonleaf.left_len + (*curr)->data_addr->data.nonleaf.right_len); 
  }
}

void populate_tree_from_stack(TreeNode** rc_root_addr, StackNode** rc_head, RCParams* params) {
  StackNode* nonleaf_head = NULL;
  assert(!*rc_root_addr); // rc_root must not be initialized
  *rc_root_addr = malloc(sizeof(**rc_root_addr));
  **rc_root_addr = (TreeNode) { .left = NULL, .right = NULL, .data_addr = stack_pop(rc_head) };
  stack_push(&nonleaf_head, rc_root_addr);
  TreeNode** curr = rc_root_addr;
  while(rc_head != NULL && curr != NULL) {
    RCData* data_addr = stack_pop(rc_head);
    if((*curr)->right) {
      insert_left(curr, data_addr);
      if(*curr != *rc_root_addr) {
        _calculate_r_c(curr, params);
      }
      curr = &((*curr)->left);
    }
    else {
      insert_right(curr, data_addr);
      curr = &((*curr)->right);
    }
    if(data_addr->is_leaf) { //leaf
      _calculate_r_c(curr, params);
      curr = (TreeNode**) stack_pop(&nonleaf_head);
    }
    else { //nonleaf
      stack_push(&nonleaf_head, curr);
    }
  }
}


