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

#if !defined(CBINDING_INCL)
#define CBINDING_INCL

#include "JitBuilderLanguageBinding.hpp"
#include "JitBuilderAPI.hpp"
#include "CFile.hpp"

class CBinding : public JitBuilderLanguageBinding
   {
public:
   CBinding(JitBuilderAPI & api, std::string directory)
      : JitBuilderLanguageBinding(api, directory)
      { }

   virtual void generate();

protected:
   virtual void generateFilesForClass(JBClass *clazz);
   virtual void generateHeader(JBClass *clazz);
   virtual void generateSource(JBClass *clazz);
   virtual void generateMainHeader();
   };

#endif // !defined(CBINDING_INCL)
