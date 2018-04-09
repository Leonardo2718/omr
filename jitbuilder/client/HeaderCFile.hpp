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

#if !defined(HEADERCFILE_INCL)
#define HEADERCFILE_INCL

#include "CFile.hpp"

class HeaderCFile : public CFile
   {
public:
   virtual void prolog(string className);
   virtual void epilog(string className);
   virtual void fieldsRecursive(JBClass *clazz);
   virtual void callbackFieldsRecursive(JBClass *clazz);
   virtual void callbackPrototype(JBFunction *func, JBClass *clazz);
   virtual void functionPrototype(JBFunction *func, JBClass *clazz, string extraQualifiers);

protected:
   virtual string prototypeQualifiers(JBFunction *func);
   };

#endif // !defined(HEADERCFILE_INCL)
