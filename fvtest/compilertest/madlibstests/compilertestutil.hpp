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

extern "C" uint8_t *compileMethod(TR::IlGeneratorMethodDetails &, TR_Hotness, int32_t &);

using NodeFiller = std::function<TR::Node * (TR::IlInjector *)>;

class InjectorWithFiller : public TR::IlInjector
   {
   public:
   InjectorWithFiller(TR::TypeDictionary* d, NodeFiller filler) : TR::IlInjector(d), _filler(filler) {}

   TR::Node * blank() { return _filler(this); }

   protected:
   NodeFiller _filler;
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
