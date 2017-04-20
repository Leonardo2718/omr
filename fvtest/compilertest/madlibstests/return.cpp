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

//~ sanity test ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

DEFINE_SIMPLE_INJECTOR(ReturnInjector)
   {
   createBlocks(1);
   returnNoValue();
   return true;
   }

TEST(OpcodeTest, ReturnTest)
   {
   TR::TypeDictionary types;

   ReturnInjector returnInjector(&types);

   auto* NoType = returnInjector.typeDictionary()->toIlType<void>();
   TR::ResolvedMethod compilee("", "", "", 0, nullptr, NoType, 0, &returnInjector);
   TR::IlGeneratorMethodDetails details(&compilee);

   int32_t rc = 0;
   auto entry = reinterpret_cast<void(*)()>(compileMethod(details, warm, rc));
   entry();
   }

//~ test using blank/filler ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

DEFINE_INJECTOR_WITH_BLANKS(ReturnTemplateInjector, 1)
   {
   createBlocks(1);
   returnValue(blank<0>()); // leave a blank to be filled by a "filler" function 0
   return true;
   }

class ReturnWithFiller: public TestWithFiller<1> {};

TEST_P(ReturnWithFiller, ReturnValueTest)
   {
   TR::TypeDictionary types;

   ReturnTemplateInjector returnInjector(&types, GetParam()); // filler passed as test parameter

   auto* Int32 = returnInjector.typeDictionary()->toIlType<int32_t>();
   constexpr auto numberOfArguments = 1;
   TR::IlType* argTypes[numberOfArguments] = {Int32};
   TR::ResolvedMethod compilee("", "", "", numberOfArguments, argTypes, Int32, 0, &returnInjector);
   TR::IlGeneratorMethodDetails details(&compilee);

   int32_t rc = 0;
   auto entry = reinterpret_cast<int32_t(*)(int32_t)>(compileMethod(details, warm, rc));
   ASSERT_EQ(0, rc) << "Compilation failed.";
   std::cerr << entry(5) << "\n";   // not ideal for a test but good enough for
                                    // proof of concept
   }

/*
 * Instantiate test instances
 *
 * 1) will fill with Int32 constant 3
 * 2) will fill with Int32 constant 4
 * 3) will fill with load of first parameter as an Int32
 * 4) will fill with load of first parameter as an Int32
 */
INSTANTIATE_TEST_CASE_P(OpcodeTest,
                        ReturnWithFiller,
                        ::testing::Values( ReturnWithFiller::FillerArray{ConstantFiller<int32_t, 3>},
                                           ReturnWithFiller::FillerArray{ConstantFiller<int32_t, 4>},
                                           ReturnWithFiller::FillerArray{ParameterFiller<0, int32_t>},
                                           ReturnWithFiller::FillerArray{ParameterFiller<0, TR::Int32>} ));
