#include "delay.h"

int main(int argc, char *argv[]) {
  TreeNode* rc_root = NULL;
  create_rc_tree(&rc_root, "pa1_examples/5.txt");
  calculate_delay(&rc_root);
  print_tree(&rc_root);
  destroy_tree(&rc_root);
  return EXIT_SUCCESS;
}
