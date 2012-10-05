#ifndef __CSTINTVAR_DECL_H
#define __CSTINTVAR_DECL_H

#include <climits>
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
    : WithHome(vm,gr,from->home()), _varIndex(0) {}

  static bool validAsElement(nativeint x) {
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
public:
  // IntVarLike interface
  bool isIntVarLike(Self self, VM vm) {
    return true;
  }

  UnstableNode intVar(Self self, VM vm) {
    // TODO: Is there a nicer way to return an unstable node containing this?
    return CstIntVar::build(vm,_varIndex);
  }

  int min(Self self, VM vm) {
    return home()->getCstSpace().intVar(_varIndex).min();
  }

  int max(Self self, VM vm) {
    return home()->getCstSpace().intVar(_varIndex).max();
  }
public:
  // Miscellaneous
  
    //  void printReprToStream(Self self, VM vm, std::ostream& out, int depth) {
    //out << "IntVar index:" << _value;
    //}
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
