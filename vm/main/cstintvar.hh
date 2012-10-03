#ifndef __CSTINTVAR_H
#define __CSTINTVAR_H

#include "mozartcore.hh"

#ifndef MOZART_GENERATOR

namespace mozart {
#include "CstIntVar-implem.hh"

void CstIntVar::create(size_t& self, VM vm, GR gr, Self from) {
  self = from.get().value();
}

} // mozart

#endif // MOZART_GENERATOR

#endif // __CSTINTVAR_H
