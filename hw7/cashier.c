#include "cashier.h"
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>



struct cashier *cashier_init(struct cache_config config)
{
  // YOUR CODE HERE
  // create a cache simulator with a set of parameters. 

  cashier* cache = (cashier*)malloc(sizeof(cashier));
  if(!cache) // You should return NULL on error.
    return NULL;
  cache->config = config;

  cache->lines = malloc(sizeof(cache_line) * config.lines);
  if(!cache->lines) // You should return NULL on error.
  {
    free(cache);
    return NULL;
  }

  // total data bytes in the cache
  cache->size = config.line_size * config.lines;

  // number of bits in tag segment; bit mask for extracting the tag bits.
  cache->tag_bits = config.address_bits - config.index_bits - config.offset_bits;
  cache->tag_mask = (1 << cache->tag_bits) - 1;

  // number of bits in index segment; bit mask for extracting the index bits.
  cache->index_bits = config.index_bits;
  cache->index_mask = (1 << cache->index_bits) - 1;

  // number of bits in offset segment; bit mask for extracting the offset bits.
  cache->offset_bits = config.offset_bits;
  cache->offset_mask = (1 << cache->offset_bits) - 1;

  // initialize the cache lines
  for (int i = 0; i < config.lines; ++i)
  {
    cache->lines[i].valid = false;
    cache->lines[i].dirty = false;
    cache->lines[i].tag = 0;
    cache->lines[i].last_access = 0;
    cache->lines[i].data = malloc(sizeof(uint8_t) * config.line_size);

    if(!cache->lines[i].data) // You should return NULL on error.
    {
      for(int j = 0; j < i; ++j)
        free(cache->lines[j].data);
      free(cache->lines); // all memory should be freed
      free(cache);
      return NULL;
    }
  }

  // You don’t need to validate the parameters.
  return cache;
}

void cashier_release(struct cashier *cache)
{
  // YOUR CODE HERE

  // release the resources allocated for the cache simulator
  for (int i = 0; i < cache->config.lines; ++i)
    free(cache->lines[i].data);
  free(cache->lines); // all memory should be freed
  free(cache);
  return;
}

bool cashier_read(struct cashier *cache, uint64_t addr, uint8_t *byte)
{
  // YOUR CODE HERE





}

bool cashier_write(struct cashier *cache, uint64_t addr, uint8_t byte)
{
  // YOUR CODE HERE





}
