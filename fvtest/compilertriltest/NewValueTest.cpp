/*******************************************************************************
 * Copyright (c) 2017, 2019 IBM Corp. and others
 *
 * This program and the accompanying materials are made available under
 * the terms of the Eclipse Public License 2.0 which accompanies this
 * distribution and is available at http://eclipse.org/legal/epl-2.0
 * or the Apache License, Version 2.0 which accompanies this distribution
 * and is available at https://www.apache.org/licenses/LICENSE-2.0.
 *
 * This Source Code may also be made available under the following Secondary
 * Licenses when the conditions for such availability set forth in the
 * Eclipse Public License, v. 2.0 are satisfied: GNU General Public License,
 * version 2 with the GNU Classpath Exception [1] and GNU General Public
 * License, version 2 with the OpenJDK Assembly Exception [2].
 *
 * [1] https://www.gnu.org/software/classpath/license.html
 * [2] http://openjdk.java.net/legal/assembly-exception.html
 *
 * SPDX-License-Identifier: EPL-2.0 OR Apache-2.0 OR GPL-2.0 WITH Classpath-exception-2.0 OR LicenseRef-GPL-2.0 WITH Assembly-exception
 *******************************************************************************/

#include "JitTest.hpp"
#include "default_compiler.hpp"
#include "il/Node.hpp"
#include "infra/ILWalk.hpp"
#include "ras/IlVerifier.hpp"
#include "ras/IlVerifierHelpers.hpp"
#include "SharedVerifiers.hpp" //for NoAndIlVerifier

class NewValueVerifier : public TR::IlVerifier
   {
   public:
   int32_t verify(TR::ResolvedMethodSymbol *sym)
      {
    //   for(TR::PreorderNodeIterator iter(sym->getFirstTreeTop(), sym->comp()); iter.currentTree(); ++iter)
    //      {
    //      int32_t rtn = verifyNode(iter.currentNode());
    //      if(rtn)
    //         return rtn;
    //      }
      return 0;
      }
   protected:
      virtual int32_t verifyNode(TR::Node *node) { return 0; }
   };

class NewValueTest : public TRTest::JitOptTest
   {

   public:
   NewValueTest()
      {
      /* Add an optimization.
       * You can add as many optimizations as you need, in order,
       * using `addOptimization`, or add a group using
       * `addOptimizations(omrCompilationStrategies[warm])`.
       * This could also be done in test cases themselves.
       */
    //   addOptimization(OMR::treeSimplification);
      }

   };

TEST_F(NewValueTest, LoweringTest) {
    auto inputTrees = 
        "(method return=NoType args=[Address]"
        "  (block"
        "    (newvalue"
        "      (aconst 0x0)"
        "      (fconst 3.14)"
        "      (aload parm=0)"
        "    )))";

    auto trees = parseString(inputTrees);

    ASSERT_NOTNULL(trees);

    Tril::DefaultCompiler compiler(trees);
    NewValueVerifier verifier;

    ASSERT_EQ(0, compiler.compileWithVerifier(&verifier)) << "Compilation failed unexpectedly\n" << "Input trees: " << inputTrees;

    // auto entry_point = compiler.getEntryPoint<TypeParam(*)(TypeParam)>();

    // // Invoke the compiled method, and assert the output is correct.
    // auto values = dataForType<TypeParam>();
    // for (auto it = values.begin(); it != values.end(); ++it)
    //    {
    //    EXPECT_EQ(std::abs(*it), entry_point(*it));
    //    }
}