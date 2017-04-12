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

#include "compile/Method.hpp"
#include "compile/CompilationTypes.hpp"
#include "ilgen/IlGeneratorMethodDetails_inlines.hpp"
#include "ilgen/TypeDictionary.hpp"
#include "ilgen/IlInjector.hpp"
#include "gtest/gtest.h"

extern "C" uint8_t *compileMethod(TR::IlGeneratorMethodDetails &, TR_Hotness, int32_t &);

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
