struct VarMap {
  /*
   * 0 : val
   * 1 : func
   */
  char varName[100][20];
  int type[100];

  int value[100];
  struct ASTNode* funNode[100];
  struct VarMap* closureMap[100];

  int size;
};

struct VarMapStack {
  int size;
  struct VarMap* data[100];
};
