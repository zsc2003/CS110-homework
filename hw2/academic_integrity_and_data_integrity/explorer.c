#include "explorer.h"
#include "dir.h"
#include "file.h"
#include "node.h"
#include <stdlib.h>
#include <string.h>
// headers
// functions
struct explorer *explorer_new(void) {
  // Allocate memory
  struct explorer *exp = calloc(1, sizeof(struct explorer));
  // Initialization
  exp->cwd = exp->root = dir_new("root");
  return exp;
}

void explorer_release(struct explorer *exp) {
  // Check for null pointer
  if (!exp) {
    return;
  }
  // Relase the resources
  dir_release(exp->root);
  free(exp);
}

bool explorer_read(const struct explorer *exp, const char *name, int offset,
                   int bytes, char *buf) {
  // Initialization
  struct node *sub = NULL;
  // Check for null pointer
  if (!exp) {
    return false;
  }
  // Check if the file exists
  sub = dir_find_node(exp->cwd, name);
  if (!sub || sub->is_dir) {
    return false;
  }
  // Read the file
  return file_read(sub->inner.file, offset, bytes, buf);
}

bool explorer_write(struct explorer *exp, const char *name, int offset,
                    int bytes, const char *buf) {
  // Initialization
  struct node *sub = NULL;
  // Check for null pointer
  if (!exp) {
    return false;
  }
  // Check if the file exisits
  sub = dir_find_node(exp->cwd, name);
  if (!sub || sub->is_dir) { // handle null pointer 
    return false;
  }
  // Write the file
  return file_write(sub->inner.file, offset, bytes, buf);
}

bool explorer_create(struct explorer *exp, char *name, int type) {
  // Check for valid arguments
  if (!exp || dir_find_node(exp->cwd, name)) {
    return false;
  }
  // Add file
  return dir_add_file(exp->cwd, type, name);
}

bool explorer_mkdir(struct explorer *exp, char *name) {
  // Check for valid arguments
  if (!exp || dir_find_node(exp->cwd, name)) { // handle null pointer
    return false;
  }
  // Add subdir
  return dir_add_subdir(exp->cwd, name);
}

bool explorer_delete(struct explorer *exp, const char *name) {
  // Check for null pointer
  if (!exp) {
    return false;
  }
  // Delete the node
  return dir_delete(exp->cwd, name);
}

bool explorer_cdpar(struct explorer *exp) {
  if (!exp || exp->cwd == exp->root) { // handle null pointer
    return false;// check not root
  }
  exp->cwd = exp->cwd->parent;
  return true;
}

bool explorer_chdir(struct explorer *exp, const char *name) {
  struct node *sub = NULL;
  if (!exp || !name) {// handle null pointer
    return false;
  }
  sub = dir_find_node(exp->cwd, name);
  if (!sub || !sub->is_dir) {
    return false;// handle null pointer
  }
  exp->cwd = sub->inner.dir;
  return true;//success
}

bool explorer_support_filetype(struct explorer *exp, open_func callback,
                               int filetype) {
  if (!exp || callback == NULL || filetype < 0 || filetype >= MAX_FT_NO) {
    return false;// handle null pointer
  }
  if (exp->ft_open[filetype] != NULL) {// handle null pointer
    return false;
  }
  exp->ft_open[filetype] = callback;//success
  return true;
}

bool explorer_open(const struct explorer *exp, const char *name) {
  struct node *sub = NULL;//initialize
  struct file *file = NULL;
  if (!exp || name == NULL) {// handle null pointer
    return false;
  }
  sub = dir_find_node(exp->cwd, name);
  if (!sub || sub->is_dir) {// handle null pointer
    return false;
  }
  file = sub->inner.file;
  if (!exp->ft_open[file->type]) {
    return false;// handle null pointer
  }
  exp->ft_open[file->type](file);
  return true;//success
}

static void search(struct node *node, char **path, const char *name,
                   find_func callback) {
  struct directory *dir = NODE_DIR(node);// make directory
  int i = 0, pathlen = strlen(*path);

  *path = realloc(*path, pathlen + strlen(node->name) + 2);
  strcat(*path, "/"); //make path
  strcat(*path, node->name);

  if (node->is_dir) {
    for (i = 0; i < dir->size; i++) { // search directory
      search(dir->subordinates[i], path, name, callback);
    }
  } else if (strncmp(node->name, name, MAX_NAME_LEN) == 0) {
    callback(*path, NODE_FILE(node)); // get the directory
  }

  *path = realloc(*path, pathlen + 1); // enlarge path
  (*path)[pathlen] = '\0';
}

void explorer_search_recursive(struct explorer *exp, const char *name,
                               find_func callback) {
  char *path = NULL; // initialize
  if (!exp || !name || !callback) {// handle null pointer
    return;
  }
  path = calloc(1, 1);
  search(exp->cwd->base, &path, name, callback); 
  free(path); // free memory
}

bool explorer_contain(struct explorer *exp, const char *name) {
  return exp ? dir_find_node(exp->cwd, name) != NULL : false; // check contain
}
