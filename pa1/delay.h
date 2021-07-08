#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

#ifndef DELAY_H

#define DELAY_H

// GENERAL

typedef struct {
  double r_unit; // resistance per unit length
  double c_unit; // capacitance per unit length
} RCParams;

typedef struct {
  bool is_leaf;
	union {
    struct {
      int label; 
      double sink_c; // sink capacitance
    } leaf;
    struct {
      double left_len; // length of wire to the left
      double right_len; // length of wire to the right 
    } nonleaf;
	} data;
  double r_i; // node resistance
  double r_path; // common path resistance to calculate delay
  double c_i; // node capacitance
  double delay; // node delay
} RCData;

void print_rc_data(RCData* data_addr);

// STACK

typedef struct _StackNode {
  struct _StackNode* next;
  void* data_addr;
} StackNode; 

void stack_push(StackNode** head, void* data_addr);
void* stack_pop(StackNode** head);
void* stack_stackpop(StackNode** head);
void destroy_stack(StackNode** head);
void print_stack(StackNode** head);

// TREE

typedef struct _TreeNode {
  struct _TreeNode* left;
  struct _TreeNode* right;
  struct _TreeNode* parent;
  RCData* data_addr;
} TreeNode;

void insert_right(TreeNode** parent, RCData* data_addr);
void insert_left(TreeNode** parent, RCData* data_addr);
void populate_tree_from_stack(TreeNode** rc_root, StackNode** rc_head, RCParams* params);
void destroy_tree(TreeNode** root);
void print_tree(TreeNode** root);

// MAIN

void create_rc_tree(TreeNode** rc_root_addr, const char* filename);
void calculate_delay(TreeNode** rc_root);
void write_rc_tree_preorder(TreeNode** rc_root, FILE* file);
void write_delays(TreeNode** rc_root, FILE* file);

#endif
