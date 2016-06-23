struct Atom {
  /*
   * 0 : int
   * 1 : bool
   * 2 : func
   */
  int type;

  int value;
  struct ASTNode* funNode;
  struct VarMap* closureMap;
};

struct Stack {
  struct Atom* data[100];
  int size;
};
