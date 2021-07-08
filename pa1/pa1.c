#include <assert.h>
#include "delay.h"

int main(int argc, char *argv[]){
  if(argc != 4) {
    return EXIT_FAILURE;
  }

  TreeNode* rc_root = NULL;
  create_rc_tree(&rc_root, argv[1]);
  calculate_delay(&rc_root);

  FILE* write_file = fopen(argv[2], "w");
  assert(write_file != NULL);
  write_rc_tree_preorder(&rc_root,write_file);
  fclose(write_file);

  write_file = fopen(argv[3], "w");
  assert(write_file != NULL);
  write_delays(&rc_root, write_file);
  fclose(write_file);

  destroy_tree(&rc_root);

	return EXIT_SUCCESS;
}
