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

UnstableNode CstIntVar::min(Self self, VM vm) {
  int mn = home()->getCstSpace().intVar(_varIndex).min();
  return SmallInt::build(vm,mn);
}

UnstableNode CstIntVar::max(Self self, VM vm) {
  int mx = home()->getCstSpace().intVar(_varIndex).max();
  return SmallInt::build(vm,mx);
}

} // mozart

#endif // MOZART_GENERATOR

#endif // __CSTINTVAR_H
