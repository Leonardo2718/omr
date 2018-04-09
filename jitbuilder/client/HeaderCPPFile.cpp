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


#include "HeaderCPPFile.hpp"

void
HeaderCPPFile::prolog(string headerName)
   {
   CPPFile::prolog();
   _s << "#ifndef " << headerName << "_INCL\n";
   _s << "#define " << headerName << "_INCL\n\n";
   }

void
HeaderCPPFile::epilog(string className)
   {
   _s << "\n";
   indent() << "#endif // defined(" << className << "_INCL)\n";
   CPPFile::epilog();
   }

void
HeaderCPPFile::fields(JBClass *clazz)
   {
   for (int f=0;f < clazz->numFields;f++)
      fieldDefinition(clazz->fields+f);
   }

void
HeaderCPPFile::functionPrototype(JBFunction *func, JBClass *clazz, string extraQualifiers)
   {
   string prequel = "";
   string qualifiers = prototypeQualifiers(func);
   if (qualifiers.length() > 0)
      prequel += prototypeQualifiers(func) + " ";

   if (extraQualifiers.length() > 0)
      prequel += extraQualifiers + " ";

   indent() << prequel << functionDeclarationText(func, clazz) << ";\n";
   }

string
HeaderCPPFile::prototypeQualifiers(JBFunction *func)
   {
   if (func->isCallback() || func->isVirtual())
      return "virtual";
   return "";
   }
