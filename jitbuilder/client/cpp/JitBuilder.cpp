/*******************************************************************************
 * Copyright (c) 2014, 2018 IBM Corp. and others
 *
 * This program and the accompanying materials are made available under
 * the terms of the Eclipse Public License 2.0 which accompanies this
 * distribution and is available at https://www.eclipse.org/legal/epl-2.0/
 * or the Apache License, Version 2.0 which accompanies this distribution and
 * is available at https://www.apache.org/licenses/LICENSE-2.0.
 *
 * This Source Code may also be made available under the following
 * Secondary Licenses when the conditions for such availability set
 * forth in the Eclipse Public License, v. 2.0 are satisfied: GNU
 * General Public License, version 2 with the GNU Classpath
 * Exception [1] and GNU General Public License, version 2 with the
 * OpenJDK Assembly Exception [2].
 *
 * [1] https://www.gnu.org/software/classpath/license.html
 * [2] http://openjdk.java.net/legal/assembly-exception.html
 *
 * SPDX-License-Identifier: EPL-2.0 OR Apache-2.0 OR GPL-2.0 WITH Classpath-exception-2.0 OR LicenseRef-GPL-2.0 WITH Assembly-exception
 *******************************************************************************/

#include "ilgen/BytecodeBuilder.hpp"
#include "ilgen/IlBuilder.hpp"
#include "ilgen/MethodBuilder.hpp"
#include "ilgen/ThunkBuilder.hpp"
#include "ilgen/VirtualMachineState.hpp"
#include "ilgen/VirtualMachineRegister.hpp"
#include "ilgen/VirtualMachineRegisterInStruct.hpp"
#include "ilgen/VirtualMachineOperandArray.hpp"
#include "ilgen/VirtualMachineOperandStack.hpp"

#include "release/cpp/include/JitBuilder.hpp"
//#include "release/cpp/include/MethodBuilder.hpp"
//#include "release/cpp/include/VirtualMachineState.hpp"

extern bool internal_initializeJitWithOptions(char *options);
extern bool internal_initializeJit();
extern int32_t internal_compileMethodBuilder(TR::MethodBuilder *mb, void **entry);
extern void internal_shutdownJit();


void *
getImpl_VirtualMachineState(void *clientObj)
   {
   return (reinterpret_cast<OMR::JitBuilder::VirtualMachineState *>(clientObj))->_impl;
   }

static void * (*IlBuilderClientAllocator)(void *) = NULL;
static void * (*BytecodeBuilderClientAllocator)(void *) = NULL;
static void * (*MethodBuilderClientAllocator)(void *) = NULL;
static void * (*ThunkBuilderClientAllocator)(void *) = NULL;
static void * (*VirtualMachineStateClientAllocator)(void *) = NULL;
static void * (*VirtualMachineRegisterClientAllocator)(void *) = NULL;
static void * (*VirtualMachineRegisterInStructClientAllocator)(void *) = NULL;
static void * (*VirtualMachineOperandArrayClientAllocator)(void *) = NULL;
static void * (*VirtualMachineOperandStackClientAllocator)(void *) = NULL;

extern "C"
{

void *
OMR::IlBuilder::allocateClientObject(TR::IlBuilder *impl)
   {
   if (IlBuilderClientAllocator)
      return IlBuilderClientAllocator(impl);
   return new OMR::JitBuilder::IlBuilder(impl);
   }

void *
OMR::BytecodeBuilder::allocateClientObject(TR::BytecodeBuilder *impl)
   {
   if (BytecodeBuilderClientAllocator)
      return BytecodeBuilderClientAllocator(impl);
   return new OMR::JitBuilder::BytecodeBuilder(impl);
   }

void *
OMR::MethodBuilder::allocateClientObject(TR::MethodBuilder *impl)
   {
   if (MethodBuilderClientAllocator)
      return MethodBuilderClientAllocator(impl);
   return new OMR::JitBuilder::MethodBuilder(impl);
   }

void *
OMR::ThunkBuilder::allocateClientObject(TR::ThunkBuilder *impl)
   {
   if (ThunkBuilderClientAllocator)
      return ThunkBuilderClientAllocator(impl);
   return new OMR::JitBuilder::ThunkBuilder(impl);
   }

void *
OMR::VirtualMachineState::allocateClientObject(TR::VirtualMachineState *impl)
   {
   if (VirtualMachineStateClientAllocator)
      return VirtualMachineStateClientAllocator(impl);
   return new OMR::JitBuilder::VirtualMachineState(impl);
   }

void *
OMR::VirtualMachineRegister::allocateClientObject(TR::VirtualMachineRegister *impl)
   {
   if (VirtualMachineRegisterClientAllocator)
      return VirtualMachineRegisterClientAllocator(impl);
   return new OMR::JitBuilder::VirtualMachineRegister(impl);
   }

void *
OMR::VirtualMachineRegisterInStruct::allocateClientObject(TR::VirtualMachineRegisterInStruct *impl)
   {
   if (VirtualMachineRegisterInStructClientAllocator)
      return VirtualMachineRegisterInStructClientAllocator(impl);
   return new OMR::JitBuilder::VirtualMachineRegisterInStruct(impl);
   }

void *
OMR::VirtualMachineOperandArray::allocateClientObject(TR::VirtualMachineOperandArray *impl)
   {
   if (VirtualMachineOperandArrayClientAllocator)
      return VirtualMachineOperandArrayClientAllocator(impl);
   return new OMR::JitBuilder::VirtualMachineOperandArray(impl);
   }

void *
OMR::VirtualMachineOperandStack::allocateClientObject(TR::VirtualMachineOperandStack *impl)
   {
   if (VirtualMachineOperandStackClientAllocator)
      return VirtualMachineOperandStackClientAllocator(impl);
   return new OMR::JitBuilder::VirtualMachineOperandStack(impl);
   }

bool
IlBuilderCallback_buildIL(void *clientObj)
   {
   OMR::JitBuilder::IlBuilder *b = (OMR::JitBuilder::IlBuilder *)clientObj;
   return b->buildIL();
   }

bool
MethodBuilderCallback_RequestFunction(void *clientObj, char *name)
   {
   OMR::JitBuilder::MethodBuilder *mb = (OMR::JitBuilder::MethodBuilder *)clientObj;
   return mb->RequestFunction((const char *)name);
   }

void
VirtualMachineStateCallback_Commit(void *clientObj, void *builderObj)
   {
   OMR::JitBuilder::VirtualMachineState *s = (OMR::JitBuilder::VirtualMachineState *)clientObj;
   OMR::JitBuilder::IlBuilder *b = (OMR::JitBuilder::IlBuilder *)builderObj;
   s->Commit(b);
   }

void
VirtualMachineStateCallback_Reload(void *clientObj, void *builderObj)
   {
   OMR::JitBuilder::VirtualMachineState *s = (OMR::JitBuilder::VirtualMachineState *)clientObj;
   OMR::JitBuilder::IlBuilder *b = (OMR::JitBuilder::IlBuilder *)builderObj;
   s->Reload(b);
   }

OMR::JitBuilder::VirtualMachineState *
VirtualMachineStateCallback_MakeCopy(void *clientObj)
   {
   OMR::JitBuilder::VirtualMachineState *s = (OMR::JitBuilder::VirtualMachineState *)clientObj;
   return s->MakeCopy();
   }

void
VirtualMachineStateCallback_MergeInto(void *clientObj, void *otherObj, void *builder_otherObj)
   {
   OMR::JitBuilder::VirtualMachineState *s = (OMR::JitBuilder::VirtualMachineState *)clientObj;
   OMR::JitBuilder::VirtualMachineState *other = (OMR::JitBuilder::VirtualMachineState *)otherObj;
   OMR::JitBuilder::IlBuilder *b_other = (OMR::JitBuilder::IlBuilder *)builder_otherObj;
   s->MergeInto(other, b_other);
   }
}

bool
OMR::JitBuilder::IlBuilder::buildIL()
   {
   return false;
   }

bool
OMR::JitBuilder::MethodBuilder::RequestFunction(const char *name)
   {
   return false;
   }

void
OMR::JitBuilder::VirtualMachineState::Commit(OMR::JitBuilder::IlBuilder *b)
   {
   }

void
OMR::JitBuilder::VirtualMachineState::Reload(OMR::JitBuilder::IlBuilder *b)
   {
   }

OMR::JitBuilder::VirtualMachineState *
OMR::JitBuilder::VirtualMachineState::MakeCopy()
   {
   return this; // no actual state in OMR::JitBuilderVirtualMachineState, so just use the same object
   }

void
OMR::JitBuilder::VirtualMachineState::MergeInto(OMR::JitBuilder::VirtualMachineState *s, OMR::JitBuilder::IlBuilder *b)
   {
   }

bool
initializeJitWithOptions(char *options)
   {
   return internal_initializeJitWithOptions(options);
   }

bool
initializeJit()
   {
   return internal_initializeJit();
   }

int32_t
compileMethodBuilder(OMR::JitBuilder::MethodBuilder *m, void **entry)
   {
   return internal_compileMethodBuilder((TR::MethodBuilder *)(m->_impl), entry);
   }

void
shutdownJit()
   {
   internal_shutdownJit();
   }
