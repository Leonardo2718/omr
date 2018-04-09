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


#include "HeaderCFile.hpp"

void
HeaderCFile::prolog(string headerName)
   {
   CFile::prolog();
   _s << "#ifndef " << headerName << "_INCL\n";
   _s << "#define " << headerName << "_INCL\n\n";
   }

void
HeaderCFile::epilog(string className)
   {
   _s << "\n";
   indent() << "#endif // defined(" << className << "_INCL)\n";
   CFile::epilog();
   }

void
HeaderCFile::fieldsRecursive(JBClass *clazz)
   {
   // generate super class fields first so they are at consistent offsets in this class (ensures overlay)
   if (clazz->hasSuper())
      {
      comment("Fields defined by " + clazz->super->name);
      fieldsRecursive(clazz->super);
      }

   _s << "\n";
   for (int f=0;f < clazz->numFields;f++)
      fieldDefinition(clazz->fields+f);
   }

void
HeaderCFile::callbackFieldsRecursive(JBClass *clazz)
   {
   // generate super class callback fields first so they are at consistent offsets in this class (ensures overlay)
   if (clazz->hasSuper())
      {
      comment("Fields defined by " + clazz->super->name);
      callbackFieldsRecursive(clazz->super);
      }

   _s << "\n";
   for (int f=0;f < clazz->numFunctions;f++)
      {
      JBFunction *func = clazz->functions+f;
      if (func->flags & IS_CALLBACK)
         callbackDefinition(func);
      }
   }

void
HeaderCFile::callbackPrototype(JBFunction *func, JBClass *clazz)
   {
   indent() << "void " << clazz->shortName << "_setCallback_" << func->name << "(" << typeName(clazz->type) << "object, void *callback);\n";
   }

void
HeaderCFile::functionPrototype(JBFunction *func, JBClass *clazz, string extraQualifiers)
   {
   indent() << prototypeQualifiers(func) << " " << extraQualifiers << " " << functionDeclarationText(func, clazz) << ";\n";
   }

string
HeaderCFile::prototypeQualifiers(JBFunction *func)
   {
   return "extern";
   }
