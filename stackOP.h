#include "stack.h"

void stackPushVal (struct Stack* stack, int *val) {
  stack -> data[stack -> size] = malloc (sizeof (struct Atom));
  stack -> data[stack -> size] -> type = 0;
  stack -> data[stack -> size] -> value = *val;
  stack -> data[stack -> size] -> closureMap = NULL;
  ++ stack -> size;
}

void stackPushBool (struct Stack* stack, int *val) {
  stack -> data[stack -> size] = malloc (sizeof (struct Atom));
  stack -> data[stack -> size] -> type = 1;
  stack -> data[stack -> size] -> value = *val;
  stack -> data[stack -> size] -> closureMap = NULL;
  ++ stack -> size;
}

void stackPushFun (struct Stack* stack, struct ASTNode* fNode) {
  stack -> data[stack -> size] = malloc (sizeof (struct Atom));
  stack -> data[stack -> size] -> type = 2;
  stack -> data[stack -> size] -> funNode = fNode;
  stack -> data[stack -> size] -> closureMap = NULL;
  ++ stack -> size;
}

void stackAddClosure (struct Stack* stack, struct VarMap* vMap) {
  stack -> data[stack -> size - 1] -> closureMap = vMap;
}

struct Atom* stackTop (struct Stack* stack) {
  return stack -> data[stack -> size - 1];
}

int stackTopType (struct Stack* stack) {
  return stackTop (stack) -> type;
}

int stackTopVal (struct Stack* stack) {
  return stackTop (stack) -> value;
}

struct ASTNode* stackTopFun (struct Stack* stack) {
  return stackTop (stack) -> funNode;
}

struct VarMap* stackTopMap (struct Stack* stack) {
  return stackTop (stack) -> closureMap;
}

void stackPop (struct Stack* stack) {
  free (stackTop(stack));
  -- stack -> size;
}

int stackPopVal (struct Stack* stack) {
  int tmp = stackTopVal (stack);
  free (stackTop(stack));
  -- stack -> size;
  return tmp;
}

int stackPopBool (struct Stack* stack) {
  int tmp = stackTopVal (stack);
  free (stackTop(stack));
  -- stack -> size;
  return tmp;
}
