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

#ifndef __GCTYPES_H
#define __GCTYPES_H

#include "mozartcore.hh"

#ifndef MOZART_GENERATOR

namespace mozart {

//////////////////
// GCedToStable //
//////////////////

#include "GCedToStable-implem.hh"

void GCedToStableBase::gCollect(GC gc, RichNode from, StableNode& to) const {
  StableNode* dest = from.as<GCedToStable>().dest();
  to.init(gc->vm, *dest);
}

void GCedToStableBase::gCollect(GC gc, RichNode from, UnstableNode& to) const {
  StableNode* dest = from.as<GCedToStable>().dest();
  to.copy(gc->vm, *dest);
}

////////////////////
// GCedToUnstable //
////////////////////

#include "GCedToUnstable-implem.hh"

void GCedToUnstableBase::gCollect(GC gc, RichNode from, StableNode& to) const {
  UnstableNode* dest = from.as<GCedToUnstable>().dest();
  to.init(gc->vm, *dest);
}

void GCedToUnstableBase::gCollect(GC gc, RichNode from, UnstableNode& to) const {
  UnstableNode* dest = from.as<GCedToUnstable>().dest();
  to.copy(gc->vm, *dest);
}

}

#endif // MOZART_GENERATOR

#endif // __GCTYPES_H
