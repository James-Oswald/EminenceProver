#ifndef SKELETON_HEADER
#define SKELETON_HEADER
/* You might want to change the above name. */

#include "Absyn.H"


class Skeleton : public Visitor
{
public:
  void visitSExpr(SExpr* p);
  void visitSNum(SNum* p);
  void visitSString(SString* p);
  void visitSAtom(SAtom* p);
  void visitSCons(SCons* p);
  void visitListSExpr(ListSExpr* p);

  void visit"Integer"("Integer" x);
  void visit"Char"("Char" x);
  void visit"Double"("Double" x);
  void visit"String"("String" x);
  void visit"Ident"("Ident" x);

};


#endif
