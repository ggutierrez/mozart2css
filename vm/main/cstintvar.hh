#ifndef __CSTINTVAR_H
#define __CSTINTVAR_H

#include "mozartcore.hh"

#ifndef MOZART_GENERATOR

namespace mozart {
#include "CstIntVar-implem.hh"

CstIntVar::CstIntVar(VM vm, RichNode min, RichNode max) 
  : WithHome(vm), _varIndex(0) {

  nativeint mn = getArgument<nativeint>(vm,min, MOZART_STR("integer"));
  nativeint mx = getArgument<nativeint>(vm,max, MOZART_STR("integer"));
  if (!validAsElement(mn))
    raiseTypeError(vm, MOZART_STR("IntVarLike"), mn);
  if (!validAsElement(mx))
    raiseTypeError(vm, MOZART_STR("IntVarLike"), mx);
  // TODO: learn how to rise errors properly
  if (mn > mx)
    raiseTypeError(vm, MOZART_STR("IntVarLike"), mx);

  auto& sp = home()->getCstSpace();
  _varIndex = sp.newIntVar((int)mn,(int)mx);
}

bool CstIntVar::validAsElement(nativeint x) {
  // Conceptually this method should return true for any integer.
  // As we talk about integer decision variables then any integer n
  // will represent the (instantiated) domain [n,n].
  // The practical limitation is that an element in a domain must be in
  // a range [min,max]. This limitation is imposed by gecode. max is defined
  // as INT_MAX-1 (gecode/int.hh:111) and min is defined as -max.
  // From this we can say that an small integer can be part of a domain if and
  // only if it is in that range.

  int max = INT_MAX - 1;
  int min = -max;
  return x <= max && x >= min;
}

// IntVarLike ------------------------------------------------------------------

UnstableNode CstIntVar::min(Self self, VM vm) {
  return SmallInt::build(vm,getVar().min());
}

UnstableNode CstIntVar::max(Self self, VM vm) {
  return SmallInt::build(vm,getVar().max());
}

UnstableNode CstIntVar::value(Self self, VM vm) {
  if (!assigned(self,vm))
    raiseTypeError(vm,MOZART_STR("IntVarLike"),self);
  return SmallInt::build(vm,getVar().val());
}

UnstableNode CstIntVar::isIn(Self self, VM vm, RichNode right) {
  nativeint r = getArgument<nativeint>(vm, right, MOZART_STR("integer"));
  if(!CstIntVar::validAsElement(r))
    raiseTypeError(vm,MOZART_STR("IntVarLike"),self);
  int e = (int)r;
  return getVar().in(e) ? 
    Boolean::build(vm,true) : Boolean::build(vm,false);
}

} // mozart

#endif // MOZART_GENERATOR

#endif // __CSTINTVAR_H
