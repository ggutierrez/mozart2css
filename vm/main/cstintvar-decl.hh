#ifndef __CSTINTVAR_DECL_H
#define __CSTINTVAR_DECL_H

#include "mozartcore-decl.hh"

namespace mozart {

class CstIntVar;

#ifndef MOZART_GENERATOR
#include "CstIntVar-implem-decl.hh"
#endif

class CstIntVar: public WithHome,
  public DataType<CstIntVar>,
  public Transient,
  public WithVariableBehavior<5> {
public:
  typedef SelfType<CstIntVar>::Self Self;

  // TODO: uuid is missing

  // TODO: getTypeAtomg is missing

  CstIntVar(VM vm, size_t index)
    : WithHome(vm), _varIndex(index) {}

  // Constructor from min and max elements in the domain
  inline
  CstIntVar(VM vm, RichNode min, RichNode max);

  // Constructor from a list (?) describing the domain
  CstIntVar(VM vm, RichNode domain)
    : WithHome(vm), _varIndex(0) {
      // TODO
      assert(false);
  }

  // TODO: Ask Sebastien about this constructor. The docs says that is needed
  // if yes, what is its semantics?
  // CstIntVar(VM vm, SpaceRef home)
  //   : WithHome(home), _varIndex(0) {}

  CstIntVar(VM vm, GR gr, Self from)
    : WithHome(vm,gr,from->home()), _varIndex(from->_varIndex) {}

  inline
  static bool validAsElement(nativeint x);

public:
  Gecode::IntVar& getVar(void) {
    return home()->getCstSpace().intVar(_varIndex);
  }
public:
  // IntVarLike interface
  bool isIntVarLike(Self self, VM vm) {
    return true;
  }

  Gecode::IntVar& intVar(Self self, VM vm) {
    return getVar();
  }

  inline
  UnstableNode min(Self self, VM vm);

  inline
  UnstableNode max(Self self, VM vm);

  inline
  UnstableNode value(Self self, VM vm);

  inline
  UnstableNode isIn(Self self, VM vm, RichNode right);
public:
  // ConstraintVar interface
  inline
  bool assigned(Self self, VM vm) {
    return getVar().assigned();
  }
public:
  // Miscellaneous
  void printReprToStream(Self self, VM vm, std::ostream& out, int depth) {
    out << getVar();
  }
private:
  // The actual representation of a constraint integer variable is a 
  // Gecode::IntVar, here we store the index of an object of that class
  // inside an array stored by a Gecode::Space
  size_t _varIndex;
};// End class CstIntVar

#ifndef MOZART_GENERATOR
#include "CstIntVar-implem-decl-after.hh"
#endif

} // End namespace mozart

#endif // __CSTINTVAR_DECL_H
