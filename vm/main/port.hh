// Copyright © 2012, Université catholique de Louvain
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

#ifndef __PORT_H
#define __PORT_H

#include "mozartcore.hh"

#ifndef MOZART_GENERATOR

namespace mozart {

//////////
// Port //
//////////

#include "Port-implem.hh"

Port::Port(VM vm, UnstableNode& stream): WithHome(vm) {
  _stream = ReadOnlyVariable::build(vm);
  stream.copy(vm, _stream);
}

Port::Port(VM vm, GR gr, Self from): WithHome(vm, gr, from->home()) {
  gr->copyUnstableNode(_stream, from->_stream);
}

void Port::send(RichNode self, VM vm, RichNode value) {
  // TODO Send to a parent space (no, the following test is not right)
  if (!isHomedInCurrentSpace(vm))
    raise(vm, MOZART_STR("globalState"), MOZART_STR("cell"));

  auto newStream = ReadOnlyVariable::build(vm);
  auto cons = buildCons(vm, value, newStream);
  UnstableNode oldStream = std::move(_stream);
  _stream = std::move(newStream);
  BindableReadOnly(oldStream).bindReadOnly(vm, cons);
}

UnstableNode Port::sendReceive(RichNode self, VM vm, RichNode value) {
  // TODO Send to a parent space (no, the following test is not right)
  if (!isHomedInCurrentSpace(vm))
    raise(vm, MOZART_STR("globalState"), MOZART_STR("cell"));

  auto result = OptVar::build(vm);
  auto pair = buildTuple(vm, vm->coreatoms.sharp, value, result);
  send(self, vm, pair);
  return result;
}

}

#endif // MOZART_GENERATOR

#endif // __PORT_H
