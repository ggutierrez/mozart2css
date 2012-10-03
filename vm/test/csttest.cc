#ifdef VM_HAS_CSS

#include "mozart.hh"

#include <gtest/gtest.h>

#include "testutils.hh"

using namespace mozart;

class CstTest : public MozartTest {};

TEST_F(CstTest, Build) {
  Space *currentSpace = vm->getCurrentSpace();
  GecodeSpace& cst =  currentSpace->getCstSpace();
  cst.dumpSpaceInformation();

}

#endif
