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

extern "C" uint8_t *compileMethod(TR::IlGeneratorMethodDetails &, TR_Hotness, int32_t &);

using NodeFiller = std::function<TR::Node * (TR::IlInjector *)>;

template <int32_t N> TR::Node *
ConstantFiller(TR::IlInjector* injector) { return injector->iconst(N); }

template <int32_t N> TR::Node *
ParameterFiller(TR::IlInjector* injector) { return injector->parameter(N, injector->Int32); }

#endif // COMPILERTESTUTIL_HPP
