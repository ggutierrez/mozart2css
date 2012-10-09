#ifndef __MODPROP_H
#define __MODPROP_H

#include "../mozartcore.hh"

namespace mozart {
namespace builtins {

// Notes:
// - All the constraints are posted in vm->getCurrentSpace()
//   TODO: can I assume that the current space is the same space each
//   variable belongs to?
class ModProp: public Module {
public:
  ModProp(): Module("Prop") {}

  static Gecode::IntRelType atomToRelType(VM vm, In r) {
    atom_t a = getArgument<atom_t>(vm,r,MOZART_STR("Atom"));
    if (a == vm->coreatoms.irt_eq) return Gecode::IRT_EQ;
    if (a == vm->coreatoms.irt_nq) return Gecode::IRT_NQ;
    if (a == vm->coreatoms.irt_lq) return Gecode::IRT_LQ;
    if (a == vm->coreatoms.irt_le) return Gecode::IRT_LE;
    if (a == vm->coreatoms.irt_gq) return Gecode::IRT_GQ;
    if (a == vm->coreatoms.irt_gr) return Gecode::IRT_GR;
    raiseTypeError(vm,MOZART_STR("InvalidRelationType"),r);
    return Gecode::IRT_GR;
  }

  static Gecode::IntConLevel atomToIntConLevel(VM vm, In l) {
    atom_t a = getArgument<atom_t>(vm,l,MOZART_STR("Atom"));
    if (a == vm->coreatoms.icl_val) return Gecode::ICL_VAL;
    if (a == vm->coreatoms.icl_bnd) return Gecode::ICL_BND;
    if (a == vm->coreatoms.icl_dom) return Gecode::ICL_DOM;
    if (a == vm->coreatoms.icl_def) return Gecode::ICL_DEF;
    raiseTypeError(vm,MOZART_STR("InvalidIntegerConsistencyLevel"),l);
    return Gecode::ICL_DEF;
  }
  
  class Rel: public Builtin<Rel> {
  public:
    Rel(): Builtin("rel") {}

    void operator()(VM vm, In x0, In r, In x1, In icl) {
      assert(vm->getCurrentSpace()->hasConstraintSpace());
      GecodeSpace& home = vm->getCurrentSpace()->getCstSpace();
      Gecode::IntVar& vx0 = IntVarLike(x0).intVar(vm);
      Gecode::IntVar& vx1 = IntVarLike(x1).intVar(vm);
      Gecode::IntRelType rt = atomToRelType(vm,r);
      Gecode::rel(home,vx0,rt,vx1);
      //auto consistency = getArgument<atom_t>(vm,icl,MOZART_STR("Atom"));

      // Gecode::rel(home,vx0,)
    }
  };
}; // class ModProp
} // namespace builtins
} // namespace mozart
#endif // __MODPROP_H