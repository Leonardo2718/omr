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

#if !defined(SOURCECFILE_INCL)
#define SOURCECFILE_INCL

#include "CFile.hpp"

class SourceCFile : public CFile
   {
public:
   virtual void prolog();
   virtual void callbackBody(JBFunction *func, JBClass *clazz);
   virtual void constructorBody(JBFunction *func, JBClass *clazz);
   virtual void creatorBody(JBFunction *func, JBClass *clazz);
   virtual void destructorBody(JBFunction *func, JBClass *clazz);
   virtual void functionBody(JBFunction *func, JBClass *clazz);

protected:
   virtual string argumentNameForCall(JBParameter & parm);
   virtual string argumentText(JBParameter & parm);
   virtual void argument0ForCall(JBParameter & parm);
   virtual void argumentNForCall(JBParameter & parm);
   virtual void callFunction(JBFunction *func, JBClass *clazz);
   virtual JBType rootType(JBType type);
   virtual string declarationQualifiers(JBFunction *func);
   virtual void endFunctionDeclaration();
   virtual JBParameter & findSizeParameter(JBParameter *parms, int32_t p);
   virtual void initializeFieldsRecursive(JBClass *implClazz, JBClass *clazz);
   virtual bool isArray(JBParameter &parm);
   virtual bool needsPreparation(JBParameter & parm);
   virtual bool needsRecovery(JBParameter & parm);
   virtual void prepareArgument(JBParameter & parm);
   virtual void prepareArrayArgument(JBParameter & parm, JBParameter & sizeParm);
   virtual void prepareVarArgs(uint32_t numParameters, JBParameter *parms, uint32_t varArg);
   virtual void varArgsForCall(uint32_t numParameters, JBParameter *parms, uint32_t varArg);
   virtual void recoverArgument(JBParameter & parm);
   virtual void recoverArrayArgument(JBParameter & parm, JBParameter & sizeParm);
   virtual void recoverVarArgs(uint32_t numParameters, JBParameter *parms, uint32_t varArg);
   virtual void returnValue(JBType type, string implObject);
   virtual void startFunctionDeclaration(JBFunction *func, JBClass *clazz);
   };

#endif // !defined(SOURCECFILE_INCL)
