typedef struct ASTNode {
  /*
   * 0 : number
   * 1 : id
   * 2 : bool-val
   *
   * 11 : print-num
   * 12 : print-bool
   *
   * 31 : plus
   * 32 : minus
   * 33 : multiply
   * 34 : divide
   * 35 : modulus
   *
   * 36 : greater
   * 37 : smaller
   * 38 : equal
   *
   * 41 : and
   * 42 : or
   * 43 : not
   *
   * 51 : if
   *
   * 60 : define
   *
   * 70 : function
   * 71 : func_id
   * 72 : func_call
   * 73 : func_param
   * 74 : named function
   * 75 : fun_body
   * 76 : fun_def
   */
  int type;

  int childSize;
  struct ASTNode* child[10];

  int val;
  char id[20];

}ANode;
