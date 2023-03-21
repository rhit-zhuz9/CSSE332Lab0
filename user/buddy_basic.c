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
#include "user/rhtest.h"
#include "user/user.h"

void test_basic_allocate(void)
{
  printf("**************** %s *************************\n", __func__);
  void *p1, *p2;

  rhmalloc_init();

  p1 = rhmalloc(4);
  printf("p1 is %p\n", p1);

  p2 = rhmalloc(4);
  printf("p2 is %p\n", p2);

  // should be 32 bytes apart
  printf("p2 - p1 = %d (should be 32 bytes)\n", (uint64)(p2 - p1));

  rhfree_all();
  printf("**************** %s *************************\n", __func__);
}

void test_basic_free(void)
{
  printf("**************** %s *************************\n", __func__);
  void *p1, *p2;

  rhmalloc_init();

  p1 = rhmalloc(4);
  printf("p1 = %p\n", p1);
  rhfree(p1);

  p2 = rhmalloc(4);
  printf("p2 = %p (should be the same as p1)\n", p2);
  rhfree(p2);

  printf("**************** %s *************************\n", __func__);
}

int main(int argc, char **argv)
{
  test_basic_allocate();
  test_basic_free();
  exit(0);
}
