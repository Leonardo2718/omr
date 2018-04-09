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

#include "CPP_Binding.hpp"
#include "HeaderCPPFile.hpp"
#include "SourceCPPFile.hpp"

using namespace std;


void
CPPBinding::generate()
   {
   for ( auto it = _api.classesBegin(); it != _api.classesEnd(); ++it )
      {
      JBClass *clazz = *it;

      generateFilesForClass(clazz);
      }

   generateMainHeader();
   }

void
CPPBinding::generateFilesForClass(JBClass *clazz)
   {
   // each class has one header
   generateHeader(clazz);

   // and one source file
   generateSource(clazz);
   }

void
CPPBinding::generateHeader(JBClass *clazz)
   {
   HeaderCPPFile header;
   string ext(".hpp");
   header.open(_dir + "/jitbuilder/release/cpp/include", clazz->name, ext);
   header.prolog(clazz->name);

   if (clazz->hasExtrasHeader())
      header.includeFile(clazz->name + "ExtrasOutsideClass.hpp");

   if (clazz->hasSuper())
      header.includeFile(clazz->super->name + ".hpp");

   header.startNamespace();

   // typedefs so everyone knows about them
   for ( auto it = _api.classesBegin(); it != _api.classesEnd(); ++it )
      {
      JBClass *clazz = *it;
      header.typedefClass(clazz);
      }
   
   header.beginClass(clazz);
   header.fields(clazz);

   for (auto it = clazz->functionsBegin(); it != clazz->functionsEnd(); ++it)
      {
      JBFunction *func = *it;
      header.functionPrototype(func, clazz, "");
      }

   for (auto superIt = clazz->supersBegin(); superIt != clazz->supersEnd(); ++superIt)
      {
      JBClass *super = *superIt;
      for (auto it = super->functionsBegin(); it != super->functionsEnd(); ++it)
         {
         JBFunction *func = (*it);
         if (!func->isOveridden() && !func->isConstructor() && !func->isCreator() && !func->isInitializer() && !func->isDestructor() && !func->isCallback())
            header.functionPrototype(func, clazz, "");
         }
      }

   if (clazz->hasExtrasHeader())
      header.includeFile(clazz->name + "ExtrasInsideClass.hpp");

   header.endClass(clazz);

   header.endNamespace();

   header.epilog(clazz->name);
   header.close();
   }

void
CPPBinding::generateSource(JBClass *clazz)
   {
   SourceCPPFile source;
   string ext(".cpp");
   source.open(_dir + "/build/jitbuilder/client/cpp", clazz->name, ext);
   source.prolog();

   source.includeFile("client/cpp/Callbacks.hpp");
   source.includeFile("client/cpp/Macros.hpp");

   // just include everything so all types are known
   for (auto it = _api.classesBegin(); it != _api.classesEnd(); ++it)
      {
      JBClass *otherClazz = *it;
      source.includeFile("release/cpp/include/" + otherClazz->name + ".hpp");
      }

   source.startNamespace();

   for (auto it = clazz->functionsBegin(); it != clazz->functionsEnd(); ++it)
      {
      JBFunction *func = *it;
      if (func->isCreator())
         source.creatorBody(func, clazz);
      else if (func->isInitializer())
         source.initializerBody(func, clazz);
      else if (func->isDestructor())
         source.destructorBody(func, clazz);
      else if (!func->isCallback())
         source.functionBody(func, clazz, clazz);
      }

   for (auto superIt = clazz->supersBegin(); superIt != clazz->supersEnd(); ++superIt)
      {
      JBClass *super = *superIt;
      for (auto it = super->functionsBegin(); it != super->functionsEnd(); ++it)
         {
         JBFunction *func = (*it);
         if (!func->isOveridden() && !func->isConstructor() && !func->isCreator() && !func->isInitializer() && !func->isDestructor() && !func->isCallback())
            source.functionBody(func, super, clazz);
         }
      }

   source.endNamespace();
   source.close();
   }

void
CPPBinding::generateMainHeader()
   {
   HeaderCPPFile jitHeader;
   string name("JitBuilder");
   string ext(".hpp");
   jitHeader.open(_dir+"/jitbuilder/release/cpp/include", name, ext);
   jitHeader.prolog(name);

   jitHeader.includeSystemFile("stdint.h");

   jitHeader.blankLine();
   jitHeader.line("#define TOSTR(x)     #x");
   jitHeader.line("#define LINETOSTR(x) TOSTR(x)");
   jitHeader.blankLine();

   // include all class files as a convenience
   for (auto it = _api.classesBegin(); it != _api.classesEnd(); ++it)
      {
      JBClass *clazz = *it;
      jitHeader.includeFile(clazz->name + ".hpp");
      }

   jitHeader.blankLine();
   jitHeader.usingNamespace("OMR::JitBuilder");

   for (auto it = _api.functionsBegin(); it != _api.functionsEnd(); ++it)
      {
      JBFunction *func = *it;
      jitHeader.functionPrototype(func, NULL, "");
      }

   jitHeader.epilog(name);
   }
