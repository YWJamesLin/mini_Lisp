#include <string.h>
#include "varmap.h"

void define (struct VarMap *vMap, char* id, int* val) {
  vMap -> value[vMap -> size] = *val;
  vMap -> varName[vMap -> size] = strdup(id);
  vMap -> type[vMap -> size] = 0;
  ++ vMap -> size;
}

void defineFun (struct VarMap *vMap, char* id, struct ASTNode* fNode) {
  vMap -> funNode[vMap -> size] = fNode;
  vMap -> varName[vMap -> size] = strdup(id);
  vMap -> type[vMap -> size] = 1;
  ++ vMap -> size;
}

int findID (struct VarMap *vMap, char* id) {
  int i;
  for (i = 0; i < vMap -> size; ++ i) {
    if (strcmp (id, vMap -> varName[i]) == 0) {
      return i;
    }
  }
  return -1;
}

void assign (struct VarMap *vMap, char* id, int* val) {
  int tmp = findID (vMap, id);
  if (tmp != -1) {
    vMap -> value[tmp] = *val;
    vMap -> varName[vMap -> size] = strdup(id);
    ++ vMap -> size;
  }
}

int varValue (struct VarMap *vMap, char* id) {
  int tmp = findID (vMap, id);
  if (tmp != -1) {
    return vMap -> value[tmp];
  }
  return 0;
}

ANode* funNode (struct VarMap *vMap, char* id) {
  int tmp = findID (vMap, id);
  if (tmp != -1) {
    return vMap -> funNode[tmp];
  }
  return 0;
}
