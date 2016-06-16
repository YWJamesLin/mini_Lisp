#include <stdlib.h>
#include <stdio.h>
#include "varmapOP.h"
#include "stackOP.h"

void deleteTree (ANode* head) {
  if (head -> child[0] == NULL) {
    free (head);
    return;
  }

  int i;
  for (i = 0; i < head -> childSize; ++ i) {
    if ( ! head -> child[i]) {
      deleteTree (head -> child[i]);
    }
  }
  free (head);
}

void evaluateTree (struct Stack* stack, struct ASTNode* head, struct VarMap *vMap, struct VarMap *topMap) {
  int i, tmp;
  struct Stack *tmpStack;
  struct VarMap *tmpMap;
  ANode* tmpNode;
  if (head == NULL) { return; }
  switch (head -> type) {
    case 0 :
    case 2 :
      stackPush (stack, &(head -> val));
      return;
    case 1 :
      tmp = varValue (vMap, head -> id);
      stackPush (stack, &tmp);
      return;
    case 11 :
      evaluateTree (stack, head -> child[0], vMap, topMap);
      printf ("%d\n", stackPop(stack));
      break;
    case 12 :
      evaluateTree (stack, head -> child[0], vMap, topMap);
      puts((stackPop(stack) ? "#t" : "#f"));
      break;
    case 31 :
      tmp = 0;
      for (i = 0; i < head -> childSize; ++ i) {
        evaluateTree (stack, head -> child[i], vMap, topMap);
        tmp += stackPop (stack);
      }
      stackPush (stack, &tmp);
      break;
    case 32 :
      evaluateTree (stack, head -> child[0], vMap, topMap);
      tmp = stackPop (stack);
      evaluateTree (stack, head -> child[1], vMap, topMap);
      tmp -= stackPop (stack);
      stackPush (stack, &tmp);
      break;
    case 33 :
      tmp = 1;
      for (i = 0; i < head -> childSize; ++ i) {
        evaluateTree (stack, head -> child[i], vMap, topMap);
        tmp *= stackPop (stack);
      }
      stackPush (stack, &tmp);
      break;
    case 34 :
      evaluateTree (stack, head -> child[0], vMap, topMap);
      tmp = stackPop (stack);
      evaluateTree (stack, head -> child[1], vMap, topMap);
      tmp /= stackPop (stack);
      stackPush (stack, &tmp);
      break;
    case 35 :
      evaluateTree (stack, head -> child[0], vMap, topMap);
      tmp = stackPop (stack);
      evaluateTree (stack, head -> child[1], vMap, topMap);
      tmp %= stackPop (stack);
      stackPush (stack, &tmp);
      break;
    case 36 :
      evaluateTree (stack, head -> child[0], vMap, topMap);
      tmp = stackPop (stack);
      evaluateTree (stack, head -> child[1], vMap, topMap);
      tmp = (tmp > stackPop (stack));
      stackPush (stack, &tmp);
      break;
    case 37 :
      evaluateTree (stack, head -> child[0], vMap, topMap);
      tmp = stackPop (stack);
      evaluateTree (stack, head -> child[1], vMap, topMap);
      tmp = (tmp < stackPop (stack));
      stackPush (stack, &tmp);
      break;
    case 38 :
      evaluateTree (stack, head -> child[0], vMap, topMap);
      tmp = stackPop (stack);
      evaluateTree (stack, head -> child[1], vMap, topMap);
      tmp = (tmp == stackPop (stack));
      stackPush (stack, &tmp);
      break;
    case 41 :
      tmp = 1;
      for (i = 0; i < head -> childSize; ++ i) {
        evaluateTree (stack, head -> child[i], vMap, topMap);
        tmp &= stackPop (stack);
      }
      stackPush (stack, &tmp);
      break;
    case 42 :
      tmp = 0;
      for (i = 0; i < head -> childSize; ++ i) {
        evaluateTree (stack, head -> child[i], vMap, topMap);
        tmp |= stackPop (stack);
      }
      stackPush (stack, &tmp);
      break;
    case 43 :
      evaluateTree (stack, head -> child[0], vMap, topMap);
      tmp = ! stackPop (stack);
      stackPush (stack, &tmp);
      break;
    case 51 :
      evaluateTree (stack, head -> child[0], vMap, vMap);
      if (stackPop (stack)) {
        evaluateTree (stack, head -> child[1], vMap, vMap);
      } else { evaluateTree (stack, head -> child[2], vMap, topMap); }
      break;
    case 60 :
      if (head -> child[1] -> type == 70) {
        defineFun (vMap, head -> child[0] -> id, head -> child[1]);
      } else {
        evaluateTree (stack, head -> child[1], vMap, topMap);
        tmp = stackPop (stack);
        define (vMap, head -> child[0] -> id, &tmp);
      }
      break;
    case 74 :
      tmpStack = malloc (sizeof (struct Stack));
      tmpStack -> data = malloc (sizeof (struct Stack));
      tmpStack -> size = 0;

      tmpMap = malloc (sizeof (struct VarMap));
      tmpMap -> varName = malloc (sizeof (char *) * 100);
      tmpMap -> type = malloc (sizeof (int) * 100);
      tmpMap -> value = malloc (sizeof (int) * 100);
      tmpMap -> size = 0;

      tmpNode = funNode (topMap, head -> child[0] -> id);
      if (tmpNode -> child[0] != NULL) {
        for (i = 0; i < tmpNode -> child[0] -> childSize; ++ i) {
          evaluateTree (tmpStack, head -> child[1] -> child[i], vMap, topMap);
          tmp = stackPop (tmpStack);
          define (tmpMap, tmpNode -> child[0] -> child[i] -> id, &tmp);
        }
      }
      
      evaluateTree (tmpStack, tmpNode -> child[1], tmpMap, topMap);
      tmp = stackPop (tmpStack);
      stackPush (stack, &tmp);
      
      free (tmpMap -> type);
      free (tmpMap -> varName);
      free (tmpMap -> value);
      free (tmpMap);

      free (tmpStack -> data);
      free (tmpStack);
      break;
    case 72 :
      tmpStack = malloc (sizeof (struct Stack));
      tmpStack -> data = malloc (sizeof (struct Stack));
      tmpStack -> size = 0;

      tmpMap = malloc (sizeof (struct VarMap));
      tmpMap -> varName = malloc (sizeof (char *) * 100);
      tmpMap -> type = malloc (sizeof (int) * 100);
      tmpMap -> value = malloc (sizeof (int) * 100);
      tmpMap -> size = 0;

      if (head -> child[0] -> child[0] != NULL && head -> child[0] -> child[0] -> childSize) {
        for (i = 0; i < head -> child[0] -> child[0] -> childSize; ++ i) {
          evaluateTree (tmpStack, head -> child[1] -> child[i], vMap, topMap);
          tmp = stackPop (tmpStack);
          define (tmpMap, head -> child[0] -> child[0] -> child[i] -> id, &tmp);
        }
      }
      
      evaluateTree (tmpStack, head -> child[0] -> child[1], tmpMap, topMap);
      tmp = stackPop (tmpStack);
      stackPush (stack, &tmp);

      free (tmpMap -> type);
      free (tmpMap -> varName);
      free (tmpMap -> value);
      free (tmpMap);

      free (tmpStack -> data);
      free (tmpStack);
      break;

  }
}
