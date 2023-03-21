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
/**
 * This constant determines the maximum size of our heap, restricted to 1MB for
 * now. 
 * 
 * A real library would allocate some memory up front, then call to the OS to 
 * request additional memory chunks as that initial supply is depleted. On the
 * contrary, in this assignment, we will only allocate one large blob of memory
 * upfront and then fail if that blob is fully consumed. 
 */
#define MAX_HEAP_SIZE 1024*1024*4

/**
 * This is our target start for the area of memory to make sure that all of our
 * buddies are accounted for.
*/
#define TARGET_START (void*)(1 << 22)

/**
 * Align things to the next unsigned long boundary.
 */
#define ALIGNMENT sizeof(unsigned long)
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~(ALIGNMENT-1))

/**
 * The metadata struct is how we will keep track of our memory regions. It is
 * structured as a doubly linked listed aligned to the closest 8 bytes location.
 */
struct __attribute__((aligned(8))) metadata_t {
  unsigned size;
  unsigned in_use:1;
  struct metadata_t *next;
  struct metadata_t *prev;
};

typedef struct metadata_t metadata_t;
