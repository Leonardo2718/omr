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

#if !defined(SOURCEFILE_INCL)
#define SOURCEFILE_INCL

#include <assert.h>
#include <fstream>
#include <iostream>

using namespace std;

class SourceFile
   {
   public:
   SourceFile()
      : _indent(0)
      { }

   void open(std::string dir, std::string & name, std::string & extension)
      {
      _s.open(dir + "/" + name + extension, ios::out | ios::trunc);
      }

   void close()
      {
      _s.close();
      }

   // overload << so SourceFile objects can be used like streams
   template <class T> SourceFile & operator <<(const T &x)
      {
      _s << x;
      return *this;
      }

   virtual void tabstop(int width=4)
      {
      _indent += width;
      }

   virtual void backtab(int width=4)
      {
      _indent -= width;
      if (_indent < 0)
         _indent = 0;
      }

   SourceFile & indent()
      {
      _s << std::string(_indent, ' ');
      return (*this);
      }

   void line(string s)
      {
      indent() << s << "\n";
      }

   virtual void prolog();
   virtual void epilog() { }

   virtual std::string startCopyrightComment() { return ""; }
   virtual std::string copyrightLineStart() { return ""; }
   virtual std::string endCopyrightComment() { return ""; }

   virtual void comment(std::string & text) { }

   protected:
   ofstream _s;
   int      _indent;
   };

#endif // !defined(SOURCEFILE_INCL)
