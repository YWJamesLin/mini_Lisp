#include <string.h>
#include "varmap.h"

void vMapStackPush (struct VarMapStack* stack) {
  stack -> data[stack -> size] = malloc (sizeof (struct VarMap));
  stack -> data[stack -> size] -> size = 0;
  ++ stack -> size;
}

struct VarMap* vMapStackTop (struct VarMapStack* stack) {
  return stack -> data[stack -> size - 1];
}

void vMapStackPop (struct VarMapStack* stack) {
  int i;
  for (i = 0; i < vMapStackTop (stack) -> size; ++ i) {
    if (vMapStackTop (stack) -> closureMap[i] != NULL) {
      free (vMapStackTop (stack) -> closureMap[i]);
    }
  }
  free (vMapStackTop (stack));
  -- stack -> size;
}

struct VarMap* vMapDup (struct VarMap* vMap) {
  int i;
  struct VarMap* tmp = malloc (sizeof (struct VarMap));
  for (i = 0; i < vMap -> size; ++ i) {
    strcpy(tmp -> varName[i], vMap -> varName[i]);
    tmp -> type[i] = vMap -> type[i];
    tmp -> value[i] = vMap -> value[i];
    tmp -> funNode[i] = vMap -> funNode[i];
  }
  tmp -> size = vMap -> size;
  return tmp;
}

void define (struct VarMap *vMap, char* id, int* val) {
  vMap -> value[vMap -> size] = *val;
  strcpy (vMap -> varName[vMap -> size], id);
  vMap -> type[vMap -> size] = 0;
  vMap -> closureMap[vMap -> size] = NULL;
  ++ vMap -> size;
}

void defineFun (struct VarMap *vMap, char* id, struct ASTNode* fNode, struct VarMap* cMap) {
  vMap -> funNode[vMap -> size] = fNode;
  strcpy (vMap -> varName[vMap -> size], id);
  vMap -> type[vMap -> size] = 1;
  vMap -> closureMap[vMap -> size] = cMap;
  ++ vMap -> size;
}

int findID (struct VarMap *vMap, char* id) {
  int i;
  for (i = 0; i != vMap -> size; ++ i) {
    if (strcmp (id, vMap -> varName[i]) == 0) {
      return i;
    }
  }
  return -1;
}

int findValue (struct VarMapStack *stack, char* id) {
  int tmp, i;
  for (i = stack -> size - 1; i != -1; -- i) {
    tmp = findID (stack -> data[i], id);
    if (tmp != -1 && stack -> data[i] -> type[tmp] == 0) {
      return stack -> data[i] -> value[tmp];
    }
  }
  return 0;
}

struct VarMap* findClosure (struct VarMapStack *stack, char* id) {
  int tmp, i;
  for (i = stack -> size - 1; i != -1; -- i) {
    tmp = findID (stack -> data[i], id);
    if (tmp != -1 && stack -> data[i] -> type[tmp] == 1) {
      return stack -> data[i] -> closureMap[tmp];
    }
  }
  return NULL;
}

ANode* findFun (struct VarMapStack *stack, char* id) {
  int tmp, i;
  for (i = stack -> size - 1; i != -1; -- i) {
    tmp = findID (stack -> data[i], id);
    if (tmp != -1 && stack -> data[i] -> type[tmp] == 1) {
      return stack -> data[i] -> funNode[tmp];
    }
  }
  return NULL;
}
