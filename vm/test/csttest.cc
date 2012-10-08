#ifdef VM_HAS_CSS

#include "mozart.hh"

#include <climits>
#include <gtest/gtest.h>

#include "testutils.hh"

using namespace mozart;

class CstTest : public MozartTest {};

TEST_F(CstTest, IntVarLike) {
  nativeint x = -5;
  UnstableNode xNode = SmallInt::build(vm,x);
  EXPECT_TRUE(IntVarLike(xNode).isIntVarLike(vm));

  // The following test only makes sense in 64 bits architectures
  // where a nativeint can store integer bigger than INT_MIN
  nativeint out = INT_MIN + 1;
  EXPECT_FALSE(CstIntVar::validAsElement(out));
  UnstableNode outNode = SmallInt::build(vm,out);
  EXPECT_FALSE(IntVarLike(outNode).isIntVarLike(vm));

  EXPECT_RAISE(MOZART_STR("IntVarLike"),
               CstIntVar::build(vm,outNode,outNode));
}

TEST_F(CstTest, Creation) {
  UnstableNode mn = SmallInt::build(vm,2);
  UnstableNode mx = SmallInt::build(vm,10);
  UnstableNode x = CstIntVar::build(vm,mn,mx);
  EXPECT_TRUE(IntVarLike(x).isIntVarLike(vm));

  UnstableNode minNode = IntVarLike(x).min(vm);
  EXPECT_EQ_INT(2,minNode);

  UnstableNode maxNode = IntVarLike(x).max(vm);
  EXPECT_EQ_INT(10,maxNode);

}

TEST_F(CstTest, Build) {
  Space *currentSpace = vm->getCurrentSpace();
  GecodeSpace& cst =  currentSpace->getCstSpace();
  cst.dumpSpaceInformation();

}

#endif
