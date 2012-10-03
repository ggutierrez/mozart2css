#ifndef __CSTINTVAR_DECL_H
#define __CSTINTVAR_DECL_H

#include "mozartcore-decl.hh"

namespace mozart {

class CstIntVar;

#ifndef MOZART_GENERATOR
#include "CstIntVar-implem-decl.hh"
#endif

class CstIntVar: public DataType<CstIntVar>, Copyable, StoredAs<size_t>, WithValueBehavior {
public:
  typedef SelfType<CstIntVar>::Self Self;

  // TODO: uuid is missing

  // TODO: getTypeAtomg is missing

  CstIntVar(size_t index) : _value(index) {}

  static void create(size_t& self, VM, size_t value) {
    self = value;
  }

  inline
  static void create(size_t& self, VM vm, GR gr, Self from);

  size_t value() const { return _value; }
public:
  // Miscellaneous
  
    //  void printReprToStream(Self self, VM vm, std::ostream& out, int depth) {
    //out << "IntVar index:" << _value;
    //}
private:
  const size_t _value;
};// End class CstIntVar

#ifndef MOZART_GENERATOR
#include "CstIntVar-implem-decl-after.hh"
#endif

} // End namespace mozart

#endif // __CSTINTVAR_DECL_H
