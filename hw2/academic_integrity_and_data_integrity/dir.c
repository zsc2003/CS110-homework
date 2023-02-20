#include "dir.h"
#include "explorer.h"
#include "file.h"
#include "node.h"
#include <stdlib.h>
#include <string.h>
/* header files */
static bool dir_add_sub(struct directory *dirnode, struct node *sub); /* static function */

struct directory *dir_new(char *name) {
  /* Initialization */
  struct directory *dir = NULL;
  /* Check for null pointer */
  if (!name) {
    return NULL;
  }
  /* Allocate memory */
  dir = calloc(1, sizeof(struct directory));
  dir->capacity = DEFAULT_DIR_SIZE;
  dir->subordinates = calloc(dir->capacity, sizeof(struct node*));
  dir->size = 0;/* added myself to make sure the size is 0 */
  dir->parent = NULL;
  /* Create base node */
  dir->base = node_new(true, name, dir);
  return dir;
}
#include <stdio.h>
void dir_release(struct directory *dir) {
  /* Initialization */
  int i = 0;
  if (!dir) { /* handle null pointer*/
    return;
  }
  /* Release all the subordiniates */
  for (i = 0; i < dir->size; i++) {
    node_release(dir->subordinates[i]);
  }
  /* Release the resources */
  /* Check if base has already been released. Prevent circular call. */
  if (dir->base) {
    dir->base->inner.dir = NULL;
    node_release(dir->base);
  }
  /* Release data and self. */
  free(dir->subordinates);
  free(dir);
}

struct node *dir_find_node(const struct directory *dir, const char *name) {
  /* YOUR CODE HERE */
  /* printf("NOT IMPLEMENTED\n"); */
  if(!dir) /*handle null pointer*/
    return NULL;
  if(!name)/*handle null pointer*/
    return NULL;
  if(strcmp(dir->base->name, name) == 0)
    return dir->base;/* matched the dir */
  for(int i = 0; i < dir->size ; ++i)
    if(strcmp(dir->subordinates[i]->name, name) == 0)
      return dir->subordinates[i];/* matched the subordinate node */

  return NULL;/* do not matched any node*/
}

bool dir_add_file(struct directory *dir, int type, char *name) {
  /* YOUR CODE HERE */
  /*printf("NOT IMPLEMENTED\n");*/
  if(!dir) /*handle null pointer*/
    return false;
  if(!name)/*handle null pointer*/
    return false;
  for(int i = 0; i < dir->size ; ++i)
  {
    if(dir->subordinates[i]->is_dir == false)/* it should be a file*/
    {
      if(strcmp(dir->subordinates[i]->name, name) == 0)  
        return false; /* `name` already exists */
    }
  }
    
  if(dir->size == dir->capacity)
    dir->capacity *= 2; /* enlarge the capacity of dir*/
    
  dir->subordinates[dir->size] = node_new(false, name, dir); /* set up a new file*/
  ++dir->size;/* enlarge the size of dir*/
  return true;/*success*/
}

bool dir_add_subdir(struct directory *dir, char *name) {
  /* YOUR CODE HERE */
  /*printf("NOT IMPLEMENTED\n"); */
  if(!dir) /*handle null pointer*/
    return false;
  if(!name)/*handle null pointer*/
    return false;
  for(int i = 0; i < dir->size ; ++i)
  {
    if(dir->subordinates[i]->is_dir == true)/* it should be a dir*/
    {
      if(strcmp(dir->subordinates[i]->name, name) == 0)  
        return false; /* `name` already exists */
    }
  }

  if(dir->size == dir->capacity)
    dir->capacity *= 2; /* enlarge the capacity of dir*/
  dir->subordinates[dir->size] = 
  
  (true, name, dir); /* set up a new dir*/
  ++dir->size;/* enlarge the size of dir*/
  return true;/*success*/
}

bool dir_delete(struct directory *dir, const char *name) {
  /* YOUR CODE HERE */
  printf("NOT IMPLEMENTED\n");
  return false;
}
