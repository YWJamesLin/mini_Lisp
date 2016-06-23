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
  
void handleMessage (char* message, int tType, int fType) {
  sprintf (message, "Type error!");
}


int evaluateTree (struct Stack* stack, struct ASTNode* head, struct VarMapStack *vStack, char* message) {
  int i, tmp, tmp2;
  struct VarMap *tmpMap, *tmpMap2;
  ANode* tmpNode;
  if (head == NULL) { return 1; }
  switch (head -> type) {
    case 0 :
      stackPushVal (stack, &(head -> val));
      return 1;
    case 2 :
      stackPushBool (stack, &(head -> val));
      return 1;
    case 1 :
      tmp = findValue (vStack, head -> id);
      stackPushVal (stack, &tmp);
      return 1;
    case 11 :
      tmp = evaluateTree (stack, head -> child[0], vStack, message);
      if (tmp && stackTopType (stack) == 0) {
        printf ("%d\n", stackPopVal (stack));
        return 1;
      }
      handleMessage (message, 0, stackTopType (stack));
      return 0;
    case 12 :
      tmp = evaluateTree (stack, head -> child[0], vStack, message);
      if (tmp && stackTopType (stack) == 1) {
        puts((stackPopBool (stack) ? "#t" : "#f"));
        return 1;
      }
      handleMessage (message, 1, stackTopType (stack));
      return 0;
    case 31 :
      tmp = 0;
      for (i = 0; i < head -> childSize; ++ i) {
        tmp2 = evaluateTree (stack, head -> child[i], vStack, message);
        if ( ! tmp2 || stackTopType (stack) != 0) {
          handleMessage (message, 0, stackTopType (stack));
          return 0;
        }
        tmp += stackPopVal (stack);
      }
      stackPushVal (stack, &tmp);
      return 1;
    case 32 :
      tmp2 = evaluateTree (stack, head -> child[0], vStack, message);
      if ( ! tmp2 || stackTopType (stack) != 0) {
        handleMessage (message, 0, stackTopType (stack));
        return 0;
      }
      tmp = stackPopVal (stack);
      tmp2 = evaluateTree (stack, head -> child[1], vStack, message);
      if ( ! tmp2 || stackTopType (stack) != 0) {
        handleMessage (message, 0, stackTopType (stack));
        return 0;
      }
      tmp -= stackPopVal (stack);
      stackPushVal (stack, &tmp);
      return 1;
    case 33 :
      tmp = 1;
      for (i = 0; i < head -> childSize; ++ i) {
        tmp2 = evaluateTree (stack, head -> child[i], vStack, message);
        if ( ! tmp2 || stackTopType (stack) != 0) {
          handleMessage (message, 0, stackTopType (stack));
          return 0;
        }
        tmp *= stackPopVal (stack);
      }
      stackPushVal (stack, &tmp);
      return 1;
    case 34 :
      tmp2 = evaluateTree (stack, head -> child[0], vStack, message);
      if ( ! tmp2 || stackTopType (stack) != 0) {
        handleMessage (message, 0, stackTopType (stack));
        return 0;
      }
      tmp = stackPopVal (stack);
      tmp2 = evaluateTree (stack, head -> child[1], vStack, message);
      if ( ! tmp2 || stackTopType (stack) != 0) {
        handleMessage (message, 0, stackTopType (stack));
        return 0;
      }
      tmp /= stackPopVal (stack);
      stackPushVal (stack, &tmp);
      return 1;
    case 35 :
      tmp2 = evaluateTree (stack, head -> child[0], vStack, message);
      if ( ! tmp2 || stackTopType (stack) != 0) {
        handleMessage (message, 0, stackTopType (stack));
        return 0;
      }
      tmp = stackPopVal (stack);
      tmp2 = evaluateTree (stack, head -> child[1], vStack, message);
      if ( ! tmp2 || stackTopType (stack) != 0) {
        handleMessage (message, 0, stackTopType (stack));
        return 0;
      }
      tmp %= stackPopVal (stack);
      stackPushVal (stack, &tmp);
      return 1;
    case 36 :
      tmp2 = evaluateTree (stack, head -> child[0], vStack, message);
      if ( ! tmp2 || stackTopType (stack) != 0) {
        handleMessage (message, 0, stackTopType (stack));
        return 0;
      }
      tmp = stackPopVal (stack);
      tmp2 = evaluateTree (stack, head -> child[1], vStack, message);
      if ( ! tmp2 || stackTopType (stack) != 0) {
        handleMessage (message, 0, stackTopType (stack));
        return 0;
      }
      tmp = (tmp > stackPopVal (stack));
      stackPushBool (stack, &tmp);
      return 1;
    case 37 :
      tmp2 = evaluateTree (stack, head -> child[0], vStack, message);
      if ( ! tmp2 || stackTopType (stack) != 0) {
        handleMessage (message, 0, stackTopType (stack));
        return 0;
      }
      tmp = stackPopVal (stack);
      tmp2 = evaluateTree (stack, head -> child[1], vStack, message);
      if ( ! tmp2 || stackTopType (stack) != 0) {
        handleMessage (message, 0, stackTopType (stack));
        return 0;
      }
      tmp = (tmp < stackPopVal (stack));
      stackPushBool (stack, &tmp);
      return 1;
    case 38 :
      tmp2 = evaluateTree (stack, head -> child[0], vStack, message);
      if ( ! tmp2 || stackTopType (stack) != 0) {
        handleMessage (message, 0, stackTopType (stack));
        return 0;
      }
      tmp = stackPopVal (stack);
      tmp2 = evaluateTree (stack, head -> child[1], vStack, message);
      if ( ! tmp2 || stackTopType (stack) != 0) {
        handleMessage (message, 0, stackTopType (stack));
        return 0;
      }
      tmp = (tmp == stackPopVal (stack));
      stackPushBool (stack, &tmp);
      return 1;
    case 41 :
      tmp = 1;
      for (i = 0; i < head -> childSize; ++ i) {
        tmp2 = evaluateTree (stack, head -> child[i], vStack, message);
        if ( ! tmp2 || stackTopType (stack) != 1) {
          handleMessage (message, 1, stackTopType (stack));
          return 0;
        }
        tmp &= stackPopVal (stack);
      }
      stackPushBool (stack, &tmp);
      return 1;
    case 42 :
      tmp = 0;
      for (i = 0; i < head -> childSize; ++ i) {
        tmp2 = evaluateTree (stack, head -> child[i], vStack, message);
        if ( ! tmp2 || stackTopType (stack) != 1) {
          handleMessage (message, 1, stackTopType (stack));
          return 0;
        }
        tmp |= stackPopVal (stack);
      }
      stackPushBool (stack, &tmp);
      return 1;
    case 43 :
      tmp2 = evaluateTree (stack, head -> child[0], vStack, message);
      if ( ! tmp2 || stackTopType (stack) != 1) {
        handleMessage (message, 1, stackTopType (stack));
        return 0;
      }
      tmp = ! stackPopVal (stack);
      stackPushBool (stack, &tmp);
      return 1;
    case 51 :
      tmp = evaluateTree (stack, head -> child[0], vStack, message);
      if ( ! tmp || stackTopType (stack) != 1) { return 0; }
      if (stackPopVal (stack)) {
        tmp = evaluateTree (stack, head -> child[1], vStack, message);
        if ( ! tmp) { return 0; }
      } else {
        tmp = evaluateTree (stack, head -> child[2], vStack, message);
        if ( ! tmp) { return 0; }
      }
      return 1;
    case 60 :
      if (head -> child[1] -> type == 70) {
        defineFun (vMapStackTop(vStack), head -> child[0] -> id, head -> child[1], NULL);
      } else {
        tmp = evaluateTree (stack, head -> child[1], vStack, message);
        if ( ! tmp) { return 0; }
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
      return 1;
    case 70 :
      stackPushFun (stack, head);
      stackAddClosure (stack, vMapDup (vMapStackTop (vStack)));
      return 1;
    case 72 :
      tmpMap = malloc (sizeof (struct VarMap));
      tmpMap -> size = 0;

      if (head -> child[0] -> child[0] != NULL && head -> child[0] -> child[0] -> childSize) {
        for (i = 0; i < head -> child[0] -> child[0] -> childSize; ++ i) {
          tmp2 = evaluateTree (stack, head -> child[1] -> child[i], vStack, message);
          if ( ! tmp2) { return 0; }
          tmp = stackPopVal (stack);
          define (tmpMap, head -> child[0] -> child[0] -> child[i] -> id, &tmp);
        }
      }
      vStack -> data[vStack -> size] = tmpMap;
      ++ vStack -> size;
      if (head -> child[0] -> child[1] -> child[0] != NULL && head -> child[0] -> child[1] -> child[0] -> childSize) {
        for (i = 0; i < head -> child[0] -> child[1] -> child[0] -> childSize; ++ i) {
          tmp2 = evaluateTree (stack, head -> child[0] -> child[1] -> child[0] -> child[i], vStack, message);
          if ( ! tmp2) { return 0; }
        }
      }
      tmp2 = evaluateTree (stack, head -> child[0] -> child[1] -> child[1], vStack, message);
      if ( ! tmp2) { return 0; }

      vMapStackPop (vStack);

      return 1;
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
          tmp2 = evaluateTree (stack, head -> child[1] -> child[i], vStack, message);
          if ( ! tmp2) { return 0; }
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
          tmp2 = evaluateTree (stack, tmpNode -> child[1] -> child[0] -> child[i], vStack, message);
          if (! tmp2) { return 0; } 
        }
      }

      tmp2 = evaluateTree (stack, tmpNode -> child[1] -> child[1], vStack, message);
      if ( ! tmp2) { return 0; }

      vMapStackPop (vStack);

      return 1;
  }
}
