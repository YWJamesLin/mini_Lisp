#include "stack.h"

void stackPush (struct Stack* stack, int *val) {
  stack -> data[stack -> size] = *val;
  ++ stack -> size;
}

int stackPop (struct Stack* stack) {
  int tmp = stack -> data[stack -> size - 1];
  -- stack -> size;
  return tmp;
}
