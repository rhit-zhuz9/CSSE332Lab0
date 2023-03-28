/****************************************************************************
 * Copyright © 2022 Rose-Hulman Institute of Technology
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the “Software”), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 ****************************************************************************/
#include "kernel/types.h"
#include "user/user.h"
#include "user/rhmalloc.h"
#include <stdint.h>

/**
 * Record the original start of our memory area in case we needed to allocate
 * more stuff to align our target start address.
*/
static void *original_start = 0;

/**
 * For testing purposes, we need to record where our memory starts. Generally
 * this is hidden from the users of the library but we're just using it here to
 * make our tests more meaningful.
 */
static void *heap_mem_start = 0;

/**
 * Check if the freelist has been initialized, if not, call rhmalloc_init()
*/
static int initialized = 0;

static int MIN_BLOCK_SIZE = 32;

metadata_t *free_lists[23];
/**
 * For testing purposes, exposed the initialization bit.
*/
int is_initialized(void) { return initialized; }

/**
 * Return the pointer to the start of the heap memory.
 * 
 * @return The heam_mem_start ptr.
 */
void *heap_start(void) { return heap_mem_start; }

int round_pow2(int size) {
  int res = 1;
  while (res < size) {
    res <<= 1;
  }
  return res;
}

int get_pow(int size){
  int res = 1;
  int pow = 0;
  while (res < size) {
    res <<= 1;
    pow ++;
  }
  return pow;
}

void add_to_free_list(metadata_t *metadata) {
    int size_index = get_pow(metadata->size);
    metadata->in_use = 0;
    metadata->next = free_lists[size_index];
    metadata->prev = 0;
    if (free_lists[size_index] != 0) {
        free_lists[size_index]->prev = metadata;
    }
    free_lists[size_index] = metadata;
}

/**
 * Initialize the rh memroy allocator system.
 *
 * @return 0 on success, non-zero number on failure.
 */
uint8 rhmalloc_init(void)
{
  char *p;

  /* Grab the start of the memory where we are allocating. */
  original_start = sbrk(0);

  /* keep allocating useless carp until we hit our target starting address */
  p = sbrk(TARGET_START - original_start);

  /* grow the memory area by MAX_HEAP_SIZE bytes */
  p = sbrk(MAX_HEAP_SIZE);
  if(p == (char *)-1) {
    fprintf(2, "sbrk failed:exiting....\n");
    exit(1);
  }

  /* check if we matched the target start */
  if(p != TARGET_START) {
    fprintf(2, "sbrk failed: cannot get to target start\n");
    exit(1);
  }

  /* check if lower 21 bits are all zeros */
  if((uint64)p & (uint64)0x1FFFFF) {
    fprintf(2, "sbrk failed: cannot get good start of memory...\n");
    exit(1);
  }

  /* update the heap start */
  heap_mem_start = p;

  // mark it as initialized
  initialized = 1;

  // TODO: Add your initialization code here, but do not change anything above
  // this line.
  metadata_t *block = (metadata_t*) heap_mem_start;
  block->size = MAX_HEAP_SIZE-sizeof(metadata_t);
  block->in_use = 0;
  block->next = 0;
  block->prev = 0;
  for(int i = 0; i<23 ; i++){
    free_lists[i] = 0;
  }
  add_to_free_list(block);
  
  return 0;
}

/**
 * Deallocates everything and frees back all the memory to the operating system.
 *
 * This routine is useful to do between tests so that we can reset everything.
 * You should not need to modify this routine though if you use global
 * variables, it might be useful to reset their values here.
 */
void rhfree_all(void)
{
  /* Imagine what would happen on a double free, yikes! */
  sbrk(-MAX_HEAP_SIZE);

  /* move back if we did have to allocate more stuff */
  sbrk(-(TARGET_START - original_start));

  heap_mem_start = 0;
  original_start = 0;
  initialized = 0;

  // TODO: Add your destruction code here, but do not change anything above this
  // line.

}

/**
 * Grab the pointer to the buddy region, given a pointer to a memory chunk of a
 * given size.
 * 
 * @param ptr               A pointer to a given memory chunk of a given size.
 * @param exponent          The exponent representing the size. 
 * 
 * @return A pointer to the buddy region, i.e., the adjacent region of the same
 * size with only 1 bit difference.
 * 
*/
void *get_buddy(void *ptr, int exponent)
{
  // TODO: Add your code here.
  int size = 1 << exponent;
  void *buddy = (void *)((uintptr_t)ptr ^ size);
  return buddy;
}

void split(int index){
  if(free_lists[index] == 0){
    split(index + 1);
  }
  int curSize = 1 << (index-1);
  metadata_t *temp = free_lists[index];
  if(temp->next != 0){
    metadata_t *next = temp->next;
    temp->prev = 0;
    free_lists[index] = next;
  }else{
    free_lists[index] = 0;
  }
  metadata_t *block1 = temp;
  metadata_t *block2 = (metadata_t *)((char *)block1 + curSize);
  block1->size = curSize;
  block2->size = curSize;
  add_to_free_list(block2);
  add_to_free_list(block1);
}

metadata_t *searchFree(int index){
  if(free_lists[index] == 0){
    split(index+1);
  }
  metadata_t *block = free_lists[index];
  if(block->next != 0){
    metadata_t *temp = block->next;
    temp->prev = 0;
    free_lists[index] = temp;
  }else{
    free_lists[index] = 0;
  }
  return block;
}


/**
 * Allocate size bytes and return a pointer to start of the region. 
 * 
 * @return A valid void ptr if there is enough room, 0 on error. 
 */
void *rhmalloc(uint32 size)
{
  /* Check if we need to call rhmalloc_init and call it if needed. */
  if(!initialized)
    if(rhmalloc_init()) return 0;

  // TODO: Add your malloc code here.
  int actual_size = round_pow2(size + sizeof(metadata_t));
  if(actual_size < MIN_BLOCK_SIZE){
    actual_size = MIN_BLOCK_SIZE;
  }
  int index = get_pow(actual_size);
  metadata_t *block = searchFree(index);
  block->in_use = 1;
  return (void*) block;
}

/**
 * Free a memory region and return it to the memory allocator.
 *
 * @param ptr The pointer to free.
 *
 * @warning This routine is not responsible for making sure that the free
 * operation will not result in an error. If freeing a pointer that has already
 * been freed, undefined behavior may occur.
 */
void rhfree(void *ptr)
{
  // TODO: Add your free code here.
  metadata_t *block = (metadata_t *)ptr;
  int exp = get_pow(block->size);
  metadata_t *buddy =(metadata_t *)get_buddy(ptr,exp);
  while(buddy->in_use != 1 && buddy->size == block->size){
    if(buddy < block){
      block = buddy;
      block->in_use = 0;
      block->size = block->size * 2;
    }
    buddy = get_buddy(block);
  }
  
}
