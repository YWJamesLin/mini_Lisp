struct VarMap {
  /*
   * 0 : val
   * 1 : func
   */
  char** varName;
  int *type;

  int *value;
  struct ASTNode** funNode;

  int size;
};
