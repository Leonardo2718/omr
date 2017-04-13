/*******************************************************************************
 *
 * (c) Copyright IBM Corp. 2017, 2017
 *
 *  This program and the accompanying materials are made available
 *  under the terms of the Eclipse Public License v1.0 and
 *  Apache License v2.0 which accompanies this distribution.
 *
 *      The Eclipse Public License is available at
 *      http://www.eclipse.org/legal/epl-v10.html
 *
 *      The Apache License v2.0 is available at
 *      http://www.opensource.org/licenses/apache2.0.php
 *
 * Contributors:
 *    Multiple authors (IBM Corp.) - initial implementation and documentation
 ******************************************************************************/

#include "compilertestutil.hpp"

DEFINE_INJECTOR_WITH_BLANKS(AddTestInjector,2)
   {
   createBlocks(1);
   returnValue(
      createWithoutSymRef(TR::iadd, 2,
         blank<0>(),   // leave iadd arguments blank
         blank<1>())); // (to be filled by filler functions)
   return true;
   }

class AddWithFiller : public TestWithFiller<2> {};

TEST_P(AddWithFiller, SimpleAddTest)
   {
   TR::TypeDictionary types;

   AddTestInjector addInjector(&types, GetParam());

   auto* Int32 = addInjector.typeDictionary()->toIlType<int32_t>();
   constexpr auto numberOfArguments = 2;
   TR::IlType* argTypes[numberOfArguments] = {Int32, Int32};
   TR::ResolvedMethod compilee("", "", "", numberOfArguments, argTypes, Int32, 0, &addInjector);
   TR::IlGeneratorMethodDetails details(&compilee);

   int32_t rc = 0;
   auto entry = reinterpret_cast<int32_t(*)(int32_t, int32_t)>(compileMethod(details, warm, rc));
   ASSERT_EQ(0, rc) << "Compilation failed.";
   ASSERT_EQ(3, entry(1, 2)); // not exactly robust since fillers could change
                              // the expected result but good enough for this
                              // proof of concept
   }

/*
 * Instantiate tests
 *
 * The instances will fill the arguments to `iadd` as follows:
 *
 * 1) Int32 constants 1 and 2
 * 2) Int32 constant 1 and an Int32 load of the second argument
 * 3) an Int32 load of the first argument and Int32 constant 2
 * 4) an Int32 load of the first argument and an Int32 load of the second argument
 */
INSTANTIATE_TEST_CASE_P(OpcodeTest,
                        AddWithFiller,
                        ::testing::Values( AddWithFiller::FillerArray{ConstantFiller<int32_t, 1>, ConstantFiller<int32_t, 2>},
                                           AddWithFiller::FillerArray{ConstantFiller<int32_t, 1>, ParameterFiller<1, int32_t>},
                                           AddWithFiller::FillerArray{ParameterFiller<0, int32_t>, ConstantFiller<int32_t, 2>},
                                           AddWithFiller::FillerArray{ParameterFiller<0, TR::Int32>, ParameterFiller<1, int32_t>} ));
