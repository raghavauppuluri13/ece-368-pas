#include "delay.h"
#define BUFFER 50 // Max size of line 

static void _file_to_stack(StackNode** head, RCParams* params, const char* filename) {
  FILE* file;
  assert((file = fopen(filename, "r")) != NULL);

  char* line = malloc(BUFFER * sizeof(*line) + 1);

  fgets(line, BUFFER, file);
  double rd = 0;
  sscanf(line, "%le %le %le", &rd, &(params->r_unit), &(params->c_unit));
  while(fgets(line, BUFFER, file) != NULL) {
	  RCData* rc_data_addr = malloc(sizeof(*rc_data_addr));
	  if (line[0] == '(') {
      *rc_data_addr = (RCData) { .is_leaf = false, .data.nonleaf = { .left_len = 0, .right_len = 0 } , .r_i = 0, .r_path = 0, .c_i = 0, .delay = 0};
      sscanf(line, "(%le %le)", &(rc_data_addr->data.nonleaf.left_len), &(rc_data_addr->data.nonleaf.right_len));
	  }
	  else {
      *rc_data_addr = (RCData) { .is_leaf = true, .data.leaf = { .label = 0, .sink_c = 0 } , .r_i = 0, .r_path = 0, .c_i = 0, .delay = 0 };
      sscanf(line, "%d(%le)", &(rc_data_addr->data.leaf.label), &(rc_data_addr->data.leaf.sink_c));
	  }
	  stack_push(head, rc_data_addr); 
  }
  RCData* root = stack_pop(head);
  root->r_i = rd; 
  root->c_i = 0.5 * params->c_unit * (root->data.nonleaf.left_len + root->data.nonleaf.right_len);
  stack_push(head, root);
  free(line);
  fclose(file);
}

void create_rc_tree(TreeNode** rc_root_addr, const char* filename) {
  StackNode* rc_head = NULL;

  RCParams params = (RCParams) { .r_unit = 0, .c_unit = 0 }; 
  
  _file_to_stack(&rc_head, &params, filename);
  populate_tree_from_stack(rc_root_addr, &rc_head, &params);
  destroy_stack(&rc_head);
}

void print_rc_data(RCData* data_addr) {
    if(data_addr->is_leaf) {
      printf("%d(%le), r:%le, r_path:%le, c:%le, delay: %le\n", data_addr->data.leaf.label, data_addr->data.leaf.sink_c, data_addr->r_i, data_addr->r_path, data_addr->c_i, data_addr->delay);
    }
    else{
      printf("(%le %le), r:%le, r_path:%le, c:%le, delay: %le\n", data_addr->data.nonleaf.left_len, data_addr->data.nonleaf.right_len, data_addr->r_i, data_addr->r_path, data_addr->c_i, data_addr->delay);
    }
}

static void _delay_from_parent(TreeNode** curr) {
    double parent_r_path = (*curr)->parent->data_addr->r_path;
    double curr_r_path = parent_r_path + (*curr)->data_addr->r_i; 
    (*curr)->data_addr->delay = ((*curr)->parent->data_addr->delay - (*curr)->data_addr->c_i * parent_r_path) * (parent_r_path / curr_r_path) + (*curr)->data_addr->c_i * curr_r_path;
    (*curr)->data_addr->r_path = curr_r_path;
}

// Calculates delays of all children from root 
static void _propogate_delay(TreeNode** root) {
  StackNode* nonleaf_head = NULL;
  TreeNode** curr = root;
  assert((*root)->data_addr->delay != 0);
  while(curr != NULL) {
    if(*curr != *root){
      _delay_from_parent(curr);
    }
    if((*curr)->data_addr->is_leaf) { //leaf
      curr = (TreeNode**) stack_pop(&nonleaf_head);
      curr = (curr) ? (&((*curr)->right)) : NULL;
    }
    else { //nonleaf
      stack_push(&nonleaf_head, curr);
      curr = &((*curr)->left);
    }
  }
}

// Updates capacitance values to the sum of the capacitance of itself and its children for delay calculations
static void _update_capacitances(TreeNode** node) {
  if(!(*node)->data_addr->is_leaf) {
    _update_capacitances(&((*node)->left));
    _update_capacitances(&((*node)->right));
    (*node)->data_addr->c_i = (*node)->data_addr->c_i + (*node)->left->data_addr->c_i + (*node)->right->data_addr->c_i;
  }
}

void calculate_delay(TreeNode** rc_root_addr) {
  _update_capacitances(rc_root_addr);
  double root_r = (*rc_root_addr)->data_addr->r_i;
  double root_c = (*rc_root_addr)->data_addr->c_i;
  (*rc_root_addr)->data_addr->delay = (1.0 / root_r) * (root_c * root_r * root_r); 
  (*rc_root_addr)->data_addr->r_path = root_r;
  _propogate_delay(rc_root_addr);
}

static void _write_rc_data(RCData* data_addr, FILE* file) {
    if(data_addr->is_leaf) {
      fprintf(file, "%d(%le)\n", data_addr->data.leaf.label, data_addr->data.leaf.sink_c);
    }
    else{
      fprintf(file, "(%le %le)\n", data_addr->data.nonleaf.left_len, data_addr->data.nonleaf.right_len);
    }
}

void write_rc_tree_preorder(TreeNode** root, FILE* file) {
  if(*root != NULL) {
    _write_rc_data((*root)->data_addr, file);
    write_rc_tree_preorder(&((*root)->left), file); 
    write_rc_tree_preorder(&((*root)->right), file); 
  }
}

void write_delays(TreeNode** root, FILE* file) {
  if(*root != NULL) {
    if((*root)->data_addr->is_leaf) {
      fprintf(file, "%d(%le)\n", (*root)->data_addr->data.leaf.label, (*root)->data_addr->delay);
    }
    write_delays(&((*root)->left), file); 
    write_delays(&((*root)->right), file); 
  }
}
