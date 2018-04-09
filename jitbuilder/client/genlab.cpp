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
#include "JitBuilderAPI.hpp"
#include "C_Binding.hpp"
#include "CPP_Binding.hpp"

int
main(int argc, char *argv[])
   {
   if (argc < 2)
      {
      cerr << "Usage: genlab <OMR ROOT DIR>\n";
      exit(-1);
      }

   std::string omrDir(argv[1]);

   cout << "Generating C language binding...";
   CBinding cbinding(JitBuilderAPI::coreAPI(), omrDir);
   cbinding.generate();
   cout << "Done!\n\n";

   cout << "Generating C++ language binding...";
   CPPBinding cppbinding(JitBuilderAPI::coreAPI(), omrDir);
   cppbinding.generate();
   cout << "Done!\n\n";
   }
