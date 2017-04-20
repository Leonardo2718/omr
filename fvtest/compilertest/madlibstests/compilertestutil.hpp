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

#ifndef COMPILERTESTUTIL_HPP
#define COMPILERTESTUTIL_HPP

#include "compile/Method.hpp"
#include "compile/CompilationTypes.hpp"
#include "ilgen/IlGeneratorMethodDetails_inlines.hpp"
#include "ilgen/TypeDictionary.hpp"
#include "ilgen/IlInjector.hpp"
#include "gtest/gtest.h"

#include <functional>
#include <type_traits>
#include <array>

extern "C" uint8_t *compileMethod(TR::IlGeneratorMethodDetails &, TR_Hotness, int32_t &);

/**
 * @brief Macro for defining an IL injector class
 *
 * @param name is the name of the IL injector class that will be defined
 *
 * The body of the macro should contain the implementation of the `injectIL`
 * function.
 *
 * Example:
 *
 *    DEFINE_SIMPLE_INJECTOR(SimpleReturn)
 *       {
 *       createBlocks(1);
 *       returnNoValue();
 *       return true;
 *       }
 */
#define DEFINE_SIMPLE_INJECTOR(name) \
   struct name : public TR::IlInjector \
      { \
      name(TR::TypeDictionary* d) : TR::IlInjector(d) {} \
      bool injectIL() override; \
      }; \
   inline bool name::injectIL()

//~ blank/filler mechanism ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

using NodeFiller = std::function<TR::Node * (TR::IlInjector *)>;

template <std::size_t N>
using NodeFillerArray = std::array<NodeFiller, N>;

/**
 * @brief IlInjector class supporting the use of blanks/fillers
 *
 * The template parameter `N` is the number of fillers expected by the instance.
 */
template <std::size_t N>
class InjectorWithFillers : public TR::IlInjector
   {
   public:

   // type of the array holding the fillers
   using FillerArray = NodeFillerArray<N>;

   InjectorWithFillers(TR::TypeDictionary* d, FillerArray fillers) : TR::IlInjector(d), _fillers(fillers) {}

   /**
    * @brief Bank injector
    *
    * Leaves a "blank" that will be filled by a "filler" function.
    */
   template <std::size_t I>
   TR::Node * blank()
      {
      static_assert(I < N, "index is greater than number of expected fillers.");
      return _fillers[I](this);
      }

   protected:
   FillerArray _fillers;
   };

/**
 * @brief Macro for defining IL injector class that uses blanks/fillers
 *
 * @param name is the name of the IL injector class that will be defined
 * @param blankCount is the number of blanks/fillers expected by the injector
 *
 * The body of the macro should contain the implementation of the `injectIL`
 * function.
 *
 * Example:
 *
 *    DEFINE_INJECTOR_WITH_BLANKS(SimpleReturn, 1)
 *       {
 *       createBlocks(1);
 *       returnValue(blank<0>());
 *       return true;
 *       }
 */
#define DEFINE_INJECTOR_WITH_BLANKS(name, blankCount) \
   struct name : public InjectorWithFillers<blankCount> \
      { \
      name(TR::TypeDictionary* d, FillerArray fillers) : InjectorWithFillers<blankCount>(d, fillers) {} \
      bool injectIL() override; \
      }; \
   inline bool name::injectIL()

/**
 * @brief Test fixture base class for IlInjector tests using blanks/fillers
 *
 * The template parameter `N` is the number of fillers expected by the instance.
 *
 * It publically inherits from `::testing::TestWithParam< NodeFillerArray<N> >`
 * to allow tests to be instantiated by gtests `INSTANTIATE_TEST_CASE_P` macro.
 */
template <std::size_t N>
class TestWithFiller : public ::testing::TestWithParam< NodeFillerArray<N> >
   {
   public:
   using FillerArray = NodeFillerArray<N>;
   };

//~ ConstantFiller ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/**
 * @brief Fills a "blank" with a constant
 *
 * Will fill a "blank" with a constant node for the specified type and value.
 *
 * Example:
 *
 *    ConstantFiller<int32_t, 3> // generates an `iconst 3` node
 */

template <typename T, T N>
typename std::enable_if<std::is_integral<T>::value && sizeof(T) == 1, TR::Node *>::type
ConstantFiller(TR::IlInjector* injector) { return injector->bconst(N); }

template <typename T, T N>
typename std::enable_if<std::is_integral<T>::value && sizeof(T) == 2, TR::Node *>::type
ConstantFiller(TR::IlInjector* injector) { return injector->sconst(N); }

template <typename T, T N>
typename std::enable_if<std::is_integral<T>::value && sizeof(T) == 4, TR::Node *>::type
ConstantFiller(TR::IlInjector* injector) { return injector->iconst(N); }

template <typename T, T N>
typename std::enable_if<std::is_integral<T>::value && sizeof(T) == 8, TR::Node *>::type
ConstantFiller(TR::IlInjector* injector) { return injector->lconst(N); }

template <typename T, T N>
typename std::enable_if<std::is_floating_point<T>::value && sizeof(T) == 4, TR::Node *>::type
ConstantFiller(TR::IlInjector* injector) { return injector->fconst(N); }

template <typename T, T N>
typename std::enable_if<std::is_floating_point<T>::value && sizeof(T) == 8, TR::Node *>::type
ConstantFiller(TR::IlInjector* injector) { return injector->dconst(N); }

template <typename T, T N>
typename std::enable_if<std::is_pointer<T>::value, TR::Node *>::type
ConstantFiller(TR::IlInjector* injector) { return injector->aconst(N); }

//~ ParameterFiller ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/**
 * @brief Fills a "blank" with a parameter load
 *
 * Will fill a "blank" with a load of the specified parameter as the specified type.
 *
 * Example:
 *
 *    ParameterFiller<0, int32_t> // generates `iload` of the first parameter
 */

template <int32_t N, typename T> TR::Node *
ParameterFiller(TR::IlInjector* injector) { return injector->parameter(N, injector->typeDictionary()->toIlType<T>()); }

template <int32_t N, TR::DataTypes T> TR::Node *
ParameterFiller(TR::IlInjector* injector) { return injector->parameter(N, injector->typeDictionary()->PrimitiveType(T)); }

#endif // COMPILERTESTUTIL_HPP
