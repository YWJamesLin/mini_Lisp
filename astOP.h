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

void evaluateTree (struct Stack* stack, struct ASTNode* head, struct VarMapStack *vStack) {
  int i, tmp, tmp2;
  struct VarMap *tmpMap, *tmpMap2;
  ANode* tmpNode;
  if (head == NULL) { return; }
  switch (head -> type) {
    case 0 :
    case 2 :
      stackPushVal (stack, &(head -> val));
      return;
    case 1 :
      tmp = findValue (vStack, head -> id);
      stackPushVal (stack, &tmp);
      return;
    case 11 :
      evaluateTree (stack, head -> child[0], vStack);
      printf ("%d\n", stackPopVal (stack));
      break;
    case 12 :
      evaluateTree (stack, head -> child[0], vStack);
      puts((stackPopBool (stack) ? "#t" : "#f"));
      break;
    case 31 :
      tmp = 0;
      for (i = 0; i < head -> childSize; ++ i) {
        evaluateTree (stack, head -> child[i], vStack);
        tmp += stackPopVal (stack);
      }
      stackPushVal (stack, &tmp);
      break;
    case 32 :
      evaluateTree (stack, head -> child[0], vStack);
      tmp = stackPopVal (stack);
      evaluateTree (stack, head -> child[1], vStack);
      tmp -= stackPopVal (stack);
      stackPushVal (stack, &tmp);
      break;
    case 33 :
      tmp = 1;
      for (i = 0; i < head -> childSize; ++ i) {
        evaluateTree (stack, head -> child[i], vStack);
        tmp *= stackPopVal (stack);
      }
      stackPushVal (stack, &tmp);
      break;
    case 34 :
      evaluateTree (stack, head -> child[0], vStack);
      tmp = stackPopVal (stack);
      evaluateTree (stack, head -> child[1], vStack);
      tmp /= stackPopVal (stack);
      stackPushVal (stack, &tmp);
      break;
    case 35 :
      evaluateTree (stack, head -> child[0], vStack);
      tmp = stackPopVal (stack);
      evaluateTree (stack, head -> child[1], vStack);
      tmp %= stackPopVal (stack);
      stackPushVal (stack, &tmp);
      break;
    case 36 :
      evaluateTree (stack, head -> child[0], vStack);
      tmp = stackPopVal (stack);
      evaluateTree (stack, head -> child[1], vStack);
      tmp = (tmp > stackPopVal (stack));
      stackPushVal (stack, &tmp);
      break;
    case 37 :
      evaluateTree (stack, head -> child[0], vStack);
      tmp = stackPopVal (stack);
      evaluateTree (stack, head -> child[1], vStack);
      tmp = (tmp < stackPopVal (stack));
      stackPushVal (stack, &tmp);
      break;
    case 38 :
      evaluateTree (stack, head -> child[0], vStack);
      tmp = stackPopVal (stack);
      evaluateTree (stack, head -> child[1], vStack);
      tmp = (tmp == stackPopVal (stack));
      stackPushVal (stack, &tmp);
      break;
    case 41 :
      tmp = 1;
      for (i = 0; i < head -> childSize; ++ i) {
        evaluateTree (stack, head -> child[i], vStack);
        tmp &= stackPopVal (stack);
      }
      stackPushBool (stack, &tmp);
      break;
    case 42 :
      tmp = 0;
      for (i = 0; i < head -> childSize; ++ i) {
        evaluateTree (stack, head -> child[i], vStack);
        tmp |= stackPopVal (stack);
      }
      stackPushBool (stack, &tmp);
      break;
    case 43 :
      evaluateTree (stack, head -> child[0], vStack);
      tmp = ! stackPopVal (stack);
      stackPushBool (stack, &tmp);
      break;
    case 51 :
      evaluateTree (stack, head -> child[0], vStack);
      if (stackPopVal (stack)) {
        evaluateTree (stack, head -> child[1], vStack);
      } else { evaluateTree (stack, head -> child[2], vStack); }
      break;
    case 60 :
      if (head -> child[1] -> type == 70) {
        defineFun (vMapStackTop(vStack), head -> child[0] -> id, head -> child[1], NULL);
      } else {
        evaluateTree (stack, head -> child[1], vStack);
        if (stackTopType (stack) == 2) {
          tmpNode = stackTopFun (stack);
          tmpMap = stackTopMap (stack);
          stackPop (stack);
          defineFun (vMapStackTop (vStack), head -> child[0] -> id, tmpNode, tmpMap);
        } else {
          tmp = stackPopVal (stack);
          define (vMapStackTop (vStack), head -> child[0] -> id, &tmp);
        }
      }
      break;
    case 70 :
      stackPushFun (stack, head);
      stackAddClosure (stack, vMapDup (vMapStackTop (vStack)));
      break;
    case 72 :
      tmpMap = malloc (sizeof (struct VarMap));
      tmpMap -> size = 0;

      if (head -> child[0] -> child[0] != NULL && head -> child[0] -> child[0] -> childSize) {
        for (i = 0; i < head -> child[0] -> child[0] -> childSize; ++ i) {
          evaluateTree (stack, head -> child[1] -> child[i], vStack);
          tmp = stackPopVal (stack);
          define (tmpMap, head -> child[0] -> child[0] -> child[i] -> id, &tmp);
        }
      }
      vStack -> data[vStack -> size] = tmpMap;
      ++ vStack -> size;
      if (head -> child[0] -> child[1] -> child[0] != NULL && head -> child[0] -> child[1] -> child[0] -> childSize) {
        for (i = 0; i < head -> child[0] -> child[1] -> child[0] -> childSize; ++ i) {
        evaluateTree (stack, head -> child[0] -> child[1] -> child[0] -> child[i], vStack);
        }
      }
      evaluateTree (stack, head -> child[0] -> child[1] -> child[1], vStack);

      vMapStackPop (vStack);

      break;
    case 74 :
      tmpNode = findFun (vStack, head -> child[0] -> id);
      tmpMap2 = findClosure (vStack, head -> child[0] -> id);

      if (tmpMap2 != NULL) {
        tmpMap = vMapDup (tmpMap2);
      } else {
        tmpMap = malloc (sizeof (struct VarMap));
        tmpMap -> size = 0;
      }

      if (tmpNode -> child[0] != NULL) {
        for (i = 0; i < tmpNode -> child[0] -> childSize; ++ i) {
          evaluateTree (stack, head -> child[1] -> child[i], vStack);
          if (stackTopType (stack) == 2) {
            defineFun (tmpMap, tmpNode -> child[0] -> child[i] -> id, stackTopFun (stack), NULL);
          } else {
            tmp = stackPopVal (stack);
            define (tmpMap, tmpNode -> child[0] -> child[i] -> id, &tmp);
          }
        }
      }

      vStack -> data[vStack -> size] = tmpMap;
      ++ vStack -> size;
     
      if (tmpNode -> child[1] -> child[0] != NULL && tmpNode -> child[1] -> child[0] -> childSize) {
        for (i = 0; i < tmpNode -> child[1] -> child[0] -> childSize; ++ i) {
          evaluateTree (stack, tmpNode -> child[1] -> child[0] -> child[i], vStack);
        }
      }

      evaluateTree (stack, tmpNode -> child[1] -> child[1], vStack);

      vMapStackPop (vStack);

      break;
  }
}
