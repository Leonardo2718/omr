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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <dlfcn.h>
#include <errno.h>

#include "Jit.hpp"
#include "ilgen/TypeDictionary.hpp"
#include "ilgen/MethodBuilder.hpp"

/*
 * The purpose of this test case is to demonstrate the behaviour caused by
 * generating an address comparison with an integer operand.
 *
 * Specifically, this test causes causes Value Propagation to fail with the
 * following assert message
 *
 * ```
 * Assertion failed at ../compiler/optimizer/VPHandlers.cpp:10042: !cannotFallThrough
 *       Cannot branch or fall through
 * ```
 *
 * This kind of illformed IL should be detected and reported, at the very least
 * in debug builds.
 */

struct Element
   {
   struct Element *next;
   int16_t key;
   int32_t val;
   };

typedef void (JoinFunctionType)(Element *, Element *);

class JoinMethod : public TR::MethodBuilder
   {
   public:
   JoinMethod(TR::TypeDictionary *);
   virtual bool buildIL();
   };

JoinMethod::JoinMethod(TR::TypeDictionary *d)
   : MethodBuilder(d)
   {
   DefineLine(LINETOSTR(__LINE__));
   DefineFile(__FILE__);

   DefineName("join");
   TR::IlType *pElementType = d->PointerTo("Element");
   DefineParameter("elem1", pElementType);
   DefineParameter("elem2", pElementType);
   DefineReturnType(NoType);
   }

bool
JoinMethod::buildIL()
   {

   /*
    * The following comparison results in the generation of an address
    * comparison node with an integer child node, which is illformed.
    *
    * ```
    * acmpeq
    *    iconst 0        -> WRONG
    *    aloadi
    *       aload elem1
    * ```
    */

   auto test1 =
         EqualTo(
            LoadIndirect("Element", "next",
                         Load("elem1")),
            ConstInt32(0));

   /*
    * In this example, value propagation asserts when attempting to propate the
    *
    * ```
    * aloadi
    *    aload elem1
    * ```
    *
    * to the following block inside the conditional statement.
    */

   TR::IlBuilder* thenPath = nullptr;
   IfThen(&thenPath, test1);
   thenPath->StoreIndirect("Element", "next",
   thenPath->              Load("elem1"),
   thenPath->              Load("elem2"));

   Return();

   return true;
   }

class JoinTypeDictionary : public TR::TypeDictionary
   {
   public:
   JoinTypeDictionary() :
      TR::TypeDictionary()
      {
      TR::IlType *ElementType = DEFINE_STRUCT(Element);
      TR::IlType *pElementType = PointerTo("Element");
      DEFINE_FIELD(Element, next, pElementType);
      DEFINE_FIELD(Element, val, Int32);
      CLOSE_STRUCT(Element);
      }
   };

int main() {
   initializeJit();

   JoinTypeDictionary types;
   JoinMethod method(&types);
   uint8_t *entry=0;
   int32_t rc = compileMethodBuilder(&method, &entry);
   if (rc != 0)
      {
      fprintf(stderr,"FAIL: compilation error %d\n", rc);
      exit(-2);
      }

   auto join = (JoinFunctionType*)entry;

   Element elem1 = {NULL, 0};
   Element elem2 = {NULL, 0};
   join(&elem1, &elem2);

   if (elem1.next != &elem2)
      {
      fprintf(stderr,"FAIL: compiled method did not correctly join to linked list elements\n");
      fprintf(stderr,"\telem1.next = %p\n", elem1.next);
      fprintf(stderr,"\texpected (&elem2) = %p\n", &elem2);
      exit(-3);
      }

   shutdownJit();

   return 0;
}

