/*******************************************************************************
 * Copyright (c) 2018, 2018 IBM Corp. and others
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
 * SPDX-License-Identifier: EPL-2.0 OR Apache-2.0
 *******************************************************************************/

#if !defined(SOURCECPPFILE_INCL)
#define SOURCECPPFILE_INCL

#include "CPPFile.hpp"

class SourceCPPFile : public CPPFile
   {
public:
   virtual void prolog();
   virtual void creatorBody(JBFunction *func, JBClass *clazz);
   virtual void initializerBody(JBFunction *func, JBClass *clazz);
   virtual void destructorBody(JBFunction *func, JBClass *clazz);
   virtual void functionBody(JBFunction *func, JBClass *implClazz, JBClass *clazz);

protected:
   virtual string argumentNameForCall(JBParameter & parm);
   virtual string argumentText(JBParameter & parm);
   virtual string argument0ForCall(JBParameter & parm);
   virtual string argumentNForCall(JBParameter & parm);
   virtual void callFunction(JBFunction *func, JBClass *implClazz, JBClass *clazz);
   virtual string callFunctionString(JBFunction *func, JBClass *implClazz, JBClass *clazz, bool isStatement=true);
   virtual string classSpecifier(JBClass *clazz);
   virtual string constructorCall(JBFunction *func, JBClass *clazz);
   virtual string declarationQualifiers(JBFunction *func);
   virtual void endFunctionDeclaration();
   virtual JBParameter & findSizeParameter(JBParameter *parms, int32_t p);
   virtual void handleCallbacks(JBClass *clazz);
   virtual void initializeFields(JBClass *implClazz, JBClass *clazz);
   virtual void initializeFieldsRecursive(JBClass *implClazz, JBClass *clazz);
   virtual bool isArray(JBParameter &parm);
   virtual bool needsPreparation(JBParameter & parm);
   virtual bool needsRecovery(JBParameter & parm);
   virtual void prepareArgument(JBParameter & parm);
   virtual void prepareArrayArgument(JBParameter & parm, JBParameter & sizeParm);
   virtual void prepareVarArgs(uint32_t numParameters, JBParameter *parms, uint32_t varArg);
   virtual JBType rootType(JBType type);
   virtual string varArgsForCall(uint32_t numParameters, JBParameter *parms, uint32_t varArg);
   virtual void recoverArgument(JBParameter & parm);
   virtual void recoverArrayArgument(JBParameter & parm, JBParameter & sizeParm);
   virtual void recoverVarArgs(uint32_t numParameters, JBParameter *parms, uint32_t varArg);
   virtual void returnValue(JBFunction *func, JBClass *clazz, string implObject);
   virtual void startFunctionDeclaration(JBFunction *func, JBClass *clazz);
   };

#endif // !defined(SOURCECPPFILE_INCL)
