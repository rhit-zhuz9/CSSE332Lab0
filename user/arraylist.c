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
#include "user/rhtest.h"

#define DEF_ARRAY_LIST_CAPACITY 4

/**
 * A array list structure that can contain a list of integers.
 *
 * @size          The current size of the array list.
 * @capacity      The max size of the array list.
 * @list          The actual list.
 */
struct arraylist {
  int size;
  int capacity;
  int *list;
};

/**
 * al_new - Allocate and initialize a new array list.
 *
 * @return a new array list.
 */
struct arraylist *al_new(void)
{
  /* TODO: Add your code here. */
  return 0;
}

/**
 * al_free - Free an allocated list.
 *
 * @warning The point is invalid after returning from this function.
 */
void al_free(struct arraylist *al)
{
  /* TODO: Add your code here. */
}

/**
 * al_insert_at - Insert a new element at the position pos.
 *
 * @al          The array list.
 * @pos         The position where to insert.
 * @val         The value to insert.
 *
 * @warning This function does not increase the size of the array list, it
 * rather replaces an element in the list at position pos.
 */
void al_insert_at(struct arraylist *al, int pos, int val)
{
  /* TODO: Add your code here. */
}

/**
 * al_get_at - Get the element at position pos.
 *
 * @al         The array list.
 * @pos        The position where to get.
 *
 * @return     Returns the element at position pos, 0xffffffffff if invalid.
 */
int al_get_at(struct arraylist *al, int pos)
{
  /* TODO: Add your code here. */
  return 0;
}

/**
 * al_resize - Resize an array list.
 *
 * @al         The array list.
 *
 * This would be easy if we had access to the realloc libc function, but we do
 * not, so we must do things the old hard way.
 */
void al_resize(struct arraylist *al)
{
  /* TODO: Add your code here. */
}

/**
 * al_append - Append a new element to the list.
 *
 * @al            The array list.
 * @val           The value to append.
 *
 * This function will cause the array list to increase in size. If the array
 * list goes over capacity, it will resize the array list and double the
 * capacity.
 */
void al_append(struct arraylist *al, int val)
{
  /* TODO: Add your code here. */
}

/**
 * al_print - Print the elements of an array list.
 *
 * @al         The array list to print.
 */
void al_print(struct arraylist *al)
{
  int i;

  printf("[");
  for(i = 0; i < al->size; i++) {
    printf("%d, ", al->list[i]);
  }
  printf("]\n");
}

int main(int argc, char **argv)
{
  struct arraylist *al;

  /* test creating a new array list */
  al = al_new();
  rhassert(al != 0);
  rhassert_int_equals(al->size, 0);
  rhassert_int_equals(al->capacity, DEF_ARRAY_LIST_CAPACITY);

  /* test appending two numbers */
  al_append(al, 1);
  al_append(al, 2);
  rhassert_int_equals(al->size, 2);
  rhassert_int_equals(al->capacity, DEF_ARRAY_LIST_CAPACITY);

  /* test getting an integer */
  rhassert_int_equals(al_get_at(al, 0), 1);
  rhassert_int_equals(al_get_at(al, 1), 2);
  rhassert_int_equals(al_get_at(al, 5), 0xffffffff);

  /* add more elements, check on size and capacity */
  al_append(al, 3);
  al_append(al, 4);
  rhassert_int_equals(al->size, 4);
  rhassert_int_equals(al->capacity, DEF_ARRAY_LIST_CAPACITY);

  /* check for automatic resizing */
  al_append(al, 5);
  rhassert_int_equals(al->size, 5);
  rhassert_int_equals(al->capacity, 2 * DEF_ARRAY_LIST_CAPACITY);

  /* check all elements are there after resize */
  rhassert_int_equals(al_get_at(al, 0), 1);
  rhassert_int_equals(al_get_at(al, 1), 2);
  rhassert_int_equals(al_get_at(al, 2), 3);
  rhassert_int_equals(al_get_at(al, 3), 4);
  rhassert_int_equals(al_get_at(al, 4), 5);

  exit(0);
}
