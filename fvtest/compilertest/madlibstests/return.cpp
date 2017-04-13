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

class ReturnInjector : public TR::IlInjector
   {
   public:
   ReturnInjector(TR::TypeDictionary* d) : TR::IlInjector(d) {}
   bool injectIL() override;
   };

bool ReturnInjector::injectIL()
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

class ReturnTemplateInjector : public InjectorWithFiller
   {
   public:
   ReturnTemplateInjector(TR::TypeDictionary* d, NodeFiller filler) : InjectorWithFiller(d, filler) {}
   bool injectIL() override;
   };

bool ReturnTemplateInjector::injectIL()
   {
   createBlocks(1);
   returnValue(blank()); // leave a blank to be filled by a "filler" function
   return true;
   }

class TestWithFiller : public ::testing::TestWithParam<NodeFiller>
   {
   };

TEST_P(TestWithFiller, ReturnValueTest)
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

// instantiate test instances
INSTANTIATE_TEST_CASE_P(OpcodeTest,
                        TestWithFiller,
                        ::testing::Values( ConstantFiller<int32_t, 3>,      // generate Int32 constant 3
                                           ConstantFiller<int32_t, 4>,      // generate Int32 constant 4
                                           ParameterFiller<0, int32_t>,     // load parameter 0 as Int32
                                           ParameterFiller<0, TR::Int32> ));// load parameter 0 as Int32
