/*** BNFC-Generated Visitor Design Pattern Skeleton. ***/
/* This implements the common visitor design pattern.
   Note that this method uses Visitor-traversal of lists, so
   List->accept() does NOT traverse the list. This allows different
   algorithms to use context information differently. */

#include "Skeleton.H"



void Skeleton::visitSExpr(SExpr* t) {} //abstract class

void Skeleton::visitSNum(SNum *snum)
{
  /* Code For SNum Goes Here */

  visitInteger(snum->integer_);

}

void Skeleton::visitSString(SString *sstring)
{
  /* Code For SString Goes Here */

  visitString(sstring->string_);

}

void Skeleton::visitSAtom(SAtom *satom)
{
  /* Code For SAtom Goes Here */

  visitIdent(satom->ident_);

}

void Skeleton::visitSCons(SCons *scons)
{
  /* Code For SCons Goes Here */

  scons->listsexpr_->accept(this);

}


void Skeleton::visitListSExpr(ListSExpr* listsexpr)
{
  for (ListSExpr::iterator i = listsexpr->begin() ; i != listsexpr->end() ; ++i)
  {
    (*i)->accept(this);
  }
}


void Skeleton::visitInteger(Integer x)
{
  /* Code for Integer Goes Here */
}

void Skeleton::visitChar(Char x)
{
  /* Code for Char Goes Here */
}

void Skeleton::visitDouble(Double x)
{
  /* Code for Double Goes Here */
}

void Skeleton::visitString(String x)
{
  /* Code for String Goes Here */
}

void Skeleton::visitIdent(Ident x)
{
  /* Code for Ident Goes Here */
}



