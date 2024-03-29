// Copyright © 2011, Université catholique de Louvain
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// *  Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// *  Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#ifndef __CALLABLES_DECL_H
#define __CALLABLES_DECL_H

#include "mozartcore-decl.hh"

#include "opcodes.hh"
#include "builtins-decl.hh"

namespace mozart {

//////////////////////
// BuiltinProcedure //
//////////////////////

class BuiltinProcedure;

#ifndef MOZART_GENERATOR
#include "BuiltinProcedure-implem-decl.hh"
#endif

class BuiltinProcedure: public DataType<BuiltinProcedure>, Copyable,
  StoredAs<builtins::BaseBuiltin*>, WithValueBehavior {
public:
  typedef SelfType<BuiltinProcedure>::Self Self;
private:
  typedef builtins::BaseBuiltin Builtin;
public:
  static atom_t getTypeAtom(VM vm) {
    return vm->getAtom(MOZART_STR("procedure"));
  }

  BuiltinProcedure(Builtin* builtin): _builtin(builtin) {}

  static void create(Builtin*& self, VM vm, Builtin* builtin) {
    self = builtin;
  }

  static void create(Builtin*& self, VM vm, Builtin& builtin) {
    self = &builtin;
  }

  inline
  static void create(Builtin*& self, VM vm, GR gr, Self from);

public:
  /**
   * Arity of this builtin
   */
  size_t getArity() {
    return _builtin->getArity();
  }

  inline
  bool equals(VM vm, Self right);

public:
  // BuiltinCallable interface

  bool isBuiltin(VM vm) {
    return true;
  }

  /**
   * Call the builtin
   * @param vm     Contextual VM
   * @param argc   Actual number of parameters
   * @param args   Actual parameters
   */
  inline
  void callBuiltin(Self self, VM vm, size_t argc, UnstableNode* args[]);

  template <class... Args>
  inline
  void callBuiltin(Self self, VM vm, Args&&... args);

  builtins::BaseBuiltin* getBuiltin(RichNode self, VM vm) {
    return _builtin;
  }

public:
  // Callable interface

  bool isCallable(Self self, VM vm) {
    return true;
  }

  bool isProcedure(Self self, VM vm) {
    return true;
  }

  inline
  size_t procedureArity(Self self, VM vm);

  inline
  void getCallInfo(Self self, VM vm, size_t& arity,
                   ProgramCounter& start, size_t& Xcount,
                   StaticArray<StableNode>& Gs,
                   StaticArray<StableNode>& Ks);

  inline
  void getDebugInfo(Self self, VM vm,
                    atom_t& printName, UnstableNode& debugData);
public:
  // Miscellaneous

  void printReprToStream(Self self, VM vm, std::ostream& out, int depth) {
    out << "<P/" << _builtin->getArity() << " " << _builtin->getName() << ">";
  }
private:
  Builtin* _builtin;
};

#ifndef MOZART_GENERATOR
#include "BuiltinProcedure-implem-decl-after.hh"
#endif

/////////////////
// Abstraction //
/////////////////

class Abstraction;

#ifndef MOZART_GENERATOR
#include "Abstraction-implem-decl.hh"
#endif

/**
 * Abstraction value, i.e., user-defined procedure
 */
class Abstraction: public DataType<Abstraction>, public WithHome,
  StoredWithArrayOf<StableNode> {
public:
  typedef SelfType<Abstraction>::Self Self;
public:
  static atom_t getTypeAtom(VM vm) {
    return vm->getAtom(MOZART_STR("procedure"));
  }

  inline
  Abstraction(VM vm, size_t Gc, StaticArray<StableNode> _Gs,
              RichNode body);

  inline
  Abstraction(VM vm, size_t Gc, StaticArray<StableNode> _Gs,
              GR gr, Self from);

public:
  size_t getArraySize() {
    return _Gc;
  }

  inline
  StaticArray<StableNode> getElementsArray(Self self);

public:
  // ArrayInitializer interface

  inline
  void initElement(Self self, VM vm, size_t index, RichNode value);

public:
  // Callable interface

  bool isCallable(Self self, VM vm) {
    return true;
  }

  bool isProcedure(Self self, VM vm) {
    return true;
  }

  inline
  size_t procedureArity(Self self, VM vm);

  /**
   * Get the information needed to call this abstraction
   * @param vm       Contextual VM
   * @param arity    Output: arity of this abstraction
   * @param start    Output: start of the code area
   * @param Xcount   Output: number of X registers used by the code area
   * @param Gs       Output: G registers
   * @param Ks       Output: K registers
   */
  inline
  void getCallInfo(Self self, VM vm, size_t& arity,
                   ProgramCounter& start, size_t& Xcount,
                   StaticArray<StableNode>& Gs,
                   StaticArray<StableNode>& Ks);

  inline
  void getDebugInfo(Self self, VM vm,
                    atom_t& printName, UnstableNode& debugData);

public:
  // Miscellaneous

  inline
  void printReprToStream(Self self, VM vm, std::ostream& out, int depth);

private:
  inline
  void ensureCodeAreaCacheValid(VM vm);

  __attribute__((noinline))
  inline
  void fillCodeAreaCache(VM vm);

private:
  StableNode _body;
  size_t _Gc;

  // cache for information of the code area
  bool _codeAreaCacheValid;
  size_t _arity;
  ProgramCounter _start;
  size_t _Xcount;
  StaticArray<StableNode> _Ks;
};

#ifndef MOZART_GENERATOR
#include "Abstraction-implem-decl-after.hh"
#endif

}

#endif // __CALLABLES_DECL_H
