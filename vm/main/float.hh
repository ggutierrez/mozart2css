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

#ifndef __FLOAT_H
#define __FLOAT_H

#include <tuple>
#include <memory>
#include "mozartcore.hh"

#ifndef MOZART_GENERATOR

namespace mozart {

///////////
// Float //
///////////

#include "Float-implem.hh"

void Float::create(double& self, VM vm, GR gr, Self from) {
  self = from.get().value();
}

bool Float::equals(VM vm, Self right) {
  return value() == right.get().value();
}

int Float::compare(Self self, VM vm, RichNode right) {
  auto rightFloatValue = getArgument<double>(vm, right, MOZART_STR("float"));
  return (value() < rightFloatValue) ? -1 : (value() > rightFloatValue) ? 1 : 0;
}

bool Float::equalsFloat(Self self, VM vm, double right) {
  return value() == right;
}

UnstableNode Float::opposite(Self self, VM vm) {
  return Float::build(vm, -value());
}

UnstableNode Float::add(Self self, VM vm, RichNode right) {
  return addValue(self, vm,
                  getArgument<double>(vm, right, MOZART_STR("float")));
}

UnstableNode Float::addValue(Self self, VM vm, double b) {
  return Float::build(vm, value() + b);
}

UnstableNode Float::subtract(Self self, VM vm, RichNode right) {
  return subtractValue(self, vm,
                       getArgument<double>(vm, right, MOZART_STR("float")));
}

UnstableNode Float::subtractValue(Self self, VM vm, double b) {
  return Float::build(vm, value() - b);
}

UnstableNode Float::multiply(Self self, VM vm, RichNode right) {
  return multiplyValue(self, vm,
                       getArgument<double>(vm, right, MOZART_STR("float")));
}

UnstableNode Float::multiplyValue(Self self, VM vm, double b) {
  return Float::build(vm, value() * b);
}

UnstableNode Float::divide(Self self, VM vm, RichNode right) {
  return divideValue(self, vm,
                     getArgument<double>(vm, right, MOZART_STR("float")));
}

UnstableNode Float::divideValue(Self self, VM vm, double b) {
  return Float::build(vm, value() / b);
}

UnstableNode Float::div(Self self, VM vm, RichNode right) {
  raiseTypeError(vm, MOZART_STR("Integer"), self);
}

UnstableNode Float::mod(Self self, VM vm, RichNode right) {
  raiseTypeError(vm, MOZART_STR("Integer"), self);
}

namespace internal {

struct FloatToStringHelper {
  std::unique_ptr<nchar[]> string;
  size_t length;
  size_t minusSigns[2];

  explicit FloatToStringHelper(double d) : minusSigns{~(size_t)0u, ~(size_t)0u} {
    char cStr[16];
    size_t actualLength = snprintf(cStr, sizeof(cStr), "%.5g", d);
    string.reset(new nchar[actualLength + 2]);
    length = 0;

    bool hasDot = false, hasDigit = false;
    size_t* curMinusSign = minusSigns;

    for (size_t i = 0; i < actualLength; ++ i) {
      switch (cStr[i]) {
        case '+':
          continue;

        case '-':
          *curMinusSign++ = length;
          break;

        case 'e':
          if (!hasDot)
            string[length++] = '.';
          if (!hasDigit)
            string[length++] = '0';
          hasDigit = hasDot = true;
          break;

        case '.':
          hasDot = true;

        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
          if (hasDot)
            hasDigit = true;
          break;

        default:
          break;
      }

      string[length++] = cStr[i];
    }

    if (!hasDot)
      string[length++] = '.';
    if (!hasDigit)
      string[length++] = '0';
  }

  UnstableNode buildStringAt(VM vm, size_t from, size_t to) const {
    return String::build(vm, newLString(vm, string.get() + from, to - from));
  }
};

}

void Float::toString(Self self, VM vm, std::basic_ostream<nchar>& sink) {
  const internal::FloatToStringHelper helper (value());
  sink.write(helper.string.get(), helper.length);
}

nativeint Float::vsLength(Self self, VM vm) {
  return (nativeint) internal::FloatToStringHelper(value()).length;
}

}

#endif // MOZART_GENERATOR

#endif // __FLOAT_H
