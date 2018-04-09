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
 * * SPDX-License-Identifier: EPL-2.0 OR Apache-2.0
 *******************************************************************************/

#if !defined(CPPFILE_INCL)
#define CPPFILE_INCL

#include "SourceFile.hpp"
#include "JitBuilderAPI.hpp"

using namespace std;

class CPPFile : public SourceFile
   {
public:
   virtual string startCopyrightComment();
   virtual string copyrightLineStart();
   virtual string endCopyrightComment();

   virtual bool isArrayClassType(JBType type);
   virtual string implTypeCast(JBType type);
   virtual bool hasImpl(JBType type);
   virtual string typeName(JBType type);
   virtual string bareTypeName(JBType type);
   virtual string classSpecifier(JBClass *clazz) { return string(""); }
   virtual string implName(JBType type);
   virtual string functionName(JBFunction *func, JBClass *clazz);
   virtual string implFunctionName(JBFunction *func);
   virtual string implClassName(JBClass *clazz);
   virtual string fieldName(JBField *field);
   virtual string fieldQualifiers(JBField *field);
   virtual string functionDeclarationText(JBFunction *func, JBClass *clazz);

   // functions that write directly to the stream
   virtual void beginClass(JBClass *clazz);
   virtual void blankLine();
   virtual void comment(string text);
   virtual void endNamespace();
   virtual void endScope(string scope="", string comment="", bool doDedent=true, bool isStatement=false);
   virtual void endClass(JBClass *clazz);
   virtual void fieldDefinition(JBField *field);
   virtual void includeFile(string fileName);
   virtual void includeSystemFile(string fileName);
   virtual void startNamespace();
   virtual void startScope(string scope, bool doIndent=true);
   virtual void typedefClass(JBClass *clazz);
   virtual void usingNamespace(string theNamespace);
   };

#endif // !defined(CPPFILE_INCL)
