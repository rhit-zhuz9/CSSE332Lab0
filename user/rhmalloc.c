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
  return (void*)0;
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

  return (void*)0;
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
}
