#ifdef VM_HAS_CSS

#include "mozart.hh"

#include <gtest/gtest.h>

#include "testutils.hh"

using namespace mozart;

class CstTest : public MozartTest {};

TEST_F(CstTest, IntVarLike) {
  nativeint x = -5;
  UnstableNode xNode = SmallInt::build(vm,x);
  EXPECT_TRUE(IntVarLike(xNode).isIntVarLike(vm));
}

TEST_F(CstTest, Creation) {
  UnstableNode mx = SmallInt::build(vm,2);
  UnstableNode x = CstIntVar::build(vm,mx,mx);
  EXPECT_TRUE(IntVarLike(x).isIntVarLike(vm));

}

TEST_F(CstTest, Build) {
  Space *currentSpace = vm->getCurrentSpace();
  GecodeSpace& cst =  currentSpace->getCstSpace();
  cst.dumpSpaceInformation();

}

#endif
