/*******************************************************************************
 * Copyright (c) 2017, 2019 IBM Corp. and others
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
 * SPDX-License-Identifier: EPL-2.0 OR Apache-2.0 OR GPL-2.0 WITH Classpath-exception-2.0 OR LicenseRef-GPL-2.0 WITH Assembly-exception
 *******************************************************************************/

#include "omrTest.h"
#include "JitTest.hpp"

extern "C" {
int omr_main_entry(int argc, char **argv, char **envp);
}

OMRPortLibrary TRTest::TestWithPortLib::PortLib;
omrthread_t TRTest::TestWithPortLib::current_thread = NULL;

int SkipCounter::skipCount[SkipReason::NumSkipReasons_];

/**
 * @brief Global test environment to initialize and shutdown the port library
 */
class JitTestEnvironment: public ::testing::Environment {
   public:
   virtual void SetUp() {
      TRTest::TestWithPortLib::initPortLib();
   }

   virtual void TearDown() {
      TRTest::TestWithPortLib::shutdownPortLib();
   }
};

class SkippedTestListener: public ::testing::EmptyTestEventListener {
   public:
   virtual void OnTestProgramEnd(const UnitTest& unit_test) {
      int total_skips = 0;
      printf("[  SKIPPED  ]\n");
      for (int i = 0; i < static_cast<int>(SkipReason::NumSkipReasons_); ++i) {
         printf("  %6d %s\n", SkipCounter::skipCount[i], skipReasonStrings[i]);
         total_skips += SkipCounter::skipCount[i];
      }
      printf("  %6d Total\n", total_skips);
   }
};

int omr_main_entry(int argc, char **argv, char **envp) {
   ::testing::InitGoogleTest(&argc, argv);
   OMREventListener::setDefaultTestListener();
   ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();
  listeners.Append(new SkippedTestListener);
   ::testing::AddGlobalTestEnvironment(new JitTestEnvironment);
   return RUN_ALL_TESTS();
}
