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

#include <string>

#include "C_Binding.hpp"
#include "HeaderCFile.hpp"
#include "SourceCFile.hpp"

using namespace std;


void
CBinding::generate()
   {
   for ( auto it = _api.classesBegin(); it != _api.classesEnd(); ++it )
      {
      JBClass *clazz = *it;

      generateFilesForClass(clazz);
      }

   generateMainHeader();
   }

void
CBinding::generateFilesForClass(JBClass *clazz)
   {
   // each class has one header
   generateHeader(clazz);

   // and one source file
   generateSource(clazz);
   }

void
CBinding::generateHeader(JBClass *clazz)
   {
   HeaderCFile header;
   string ext(".h");
   header.open(_dir + "/jitbuilder/release/c", clazz->name, ext);
   header.prolog(clazz->name);
   header.includeFile("c/Common.h");

   // typedefs so everyone knows about them
   for ( auto it = _api.classesBegin(); it != _api.classesEnd(); ++it )
      {
      JBClass *clazz = *it;
      header.typedefClass(clazz);
      }
   
   if (clazz->hasSuper())
      header.includeFile("c/" + clazz->super->name + ".h");

   header.beginStruct(clazz->name);
   header.fieldsRecursive(clazz);
   header.callbackFieldsRecursive(clazz);
   header.endStruct(clazz->name);

   header.comment("Start of class function prototypes:");
   for (auto it = clazz->functionsBegin(); it != clazz->functionsEnd(); ++it)
      {
      JBFunction *func = *it;
      if ((func->flags & IS_CALLBACK) != 0)
         header.callbackPrototype(func, clazz);
      else
         header.functionPrototype(func, clazz, "");
      }

   header.epilog(clazz->name);
   header.close();
   }

void
CBinding::generateSource(JBClass *clazz)
   {
   SourceCFile source;
   string ext(".cpp");
   source.open(_dir + "/build/jitbuilder/client/c", clazz->name, ext);
   source.prolog();

   source.includeFile("c/Macros.hpp");
   source.startScope("extern \"C\"", false);

   // just include everything so all types are known
   for (auto it = _api.classesBegin(); it != _api.classesEnd(); ++it)
      {
      JBClass *otherClazz = *it;
      source.includeFile("c/" + otherClazz->name + ".h");
      }

   for (auto it = clazz->functionsBegin(); it != clazz->functionsEnd(); ++it)
      {
      JBFunction *func = *it;
      if ((func->flags & IS_CALLBACK) != 0)
         source.callbackBody(func, clazz);
      else if (func->isConstructor())
         source.constructorBody(func, clazz);
      else if (func->isCreator())
         source.creatorBody(func, clazz);
      else if (func->isDestructor())
         source.destructorBody(func, clazz);
      else
         source.functionBody(func, clazz);
      }

   source.endScope("", "extern \"C\"", false);
   source.close();
   }

void
CBinding::generateMainHeader()
   {
   HeaderCFile jitHeader;
   string name("jitbuilder");
   string ext(".h");
   jitHeader.open(_dir, name, ext);
   jitHeader.prolog(name);

   jitHeader.includeSystemFile("stdint.h");

   // include all class files as a convenience
   for (auto it = _api.classesBegin(); it != _api.classesEnd(); ++it)
      {
      JBClass *clazz = *it;
      jitHeader.includeFile(clazz->name + ".h");
      }

   jitHeader.blankLine();

   for (auto it = _api.functionsBegin(); it != _api.functionsEnd(); ++it)
      {
      JBFunction *func = *it;
      jitHeader.functionPrototype(func, NULL, "");
      }

   jitHeader.epilog(name);
   }
