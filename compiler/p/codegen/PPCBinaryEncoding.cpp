/*******************************************************************************
 * Copyright (c) 2000, 2020 IBM Corp. and others
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

#include <stdint.h>
#include <stdio.h>
#include <algorithm>
#include "codegen/CodeGenerator.hpp"
#include "env/FrontEnd.hpp"
#include "codegen/InstOpCode.hpp"
#include "codegen/Instruction.hpp"
#include "codegen/Machine.hpp"
#include "codegen/MemoryReference.hpp"
#include "codegen/RealRegister.hpp"
#include "codegen/Relocation.hpp"
#include "compile/Compilation.hpp"
#include "compile/ResolvedMethod.hpp"
#include "env/CompilerEnv.hpp"
#ifdef J9_PROJECT_SPECIFIC
#include "env/CHTable.hpp"
#endif
#include "env/Processors.hpp"
#include "env/TRMemory.hpp"
#include "env/jittypes.h"
#include "il/DataTypes.hpp"
#include "il/LabelSymbol.hpp"
#include "il/Node.hpp"
#include "il/Node_inlines.hpp"
#include "infra/Assert.hpp"
#include "infra/Bit.hpp"
#include "infra/List.hpp"
#include "p/codegen/GenerateInstructions.hpp"
#include "p/codegen/PPCInstruction.hpp"
#include "p/codegen/PPCOpsDefines.hpp"
#include "runtime/Runtime.hpp"

class TR_OpaqueMethodBlock;
namespace TR { class Register; }

static bool reversedConditionalBranchOpCode(TR::InstOpCode::Mnemonic op, TR::InstOpCode::Mnemonic *rop);

static bool isValidInSignExtendedField(uint32_t value, uint32_t mask)
   {
   uint32_t signMask = ~(mask >> 1);

   return (value & signMask) == 0 || (value & signMask) == signMask;
   }

static bool canUseAsVsxRegister(TR::RealRegister *reg)
   {
   switch (reg->getKind())
      {
      case TR_FPR:
      case TR_VRF:
      case TR_VSX_SCALAR:
      case TR_VSX_VECTOR:
         return true;

      default:
         return false;
      }
   }

static void fillFieldRT(TR::Instruction *instr, uint32_t *cursor, TR::RealRegister *reg)
   {
   TR_ASSERT_FATAL_WITH_INSTRUCTION(instr, reg, "Attempt to fill RT field with null register");
   TR_ASSERT_FATAL_WITH_INSTRUCTION(instr, reg->getKind() == TR_GPR, "Attempt to fill RT field with %s, which is not a GPR", reg->getRegisterName(instr->cg()->comp()));
   reg->setRegisterFieldRT(cursor);
   }

static void fillFieldFRT(TR::Instruction *instr, uint32_t *cursor, TR::RealRegister *reg)
   {
   TR_ASSERT_FATAL_WITH_INSTRUCTION(instr, reg, "Attempt to fill FRT field with null register");
   TR_ASSERT_FATAL_WITH_INSTRUCTION(instr, reg->getKind() == TR_FPR, "Attempt to fill FRT field with %s, which is not an FPR", reg->getRegisterName(instr->cg()->comp()));
   reg->setRegisterFieldRT(cursor);
   }

static void fillFieldVRT(TR::Instruction *instr, uint32_t *cursor, TR::RealRegister *reg)
   {
   TR_ASSERT_FATAL_WITH_INSTRUCTION(instr, reg, "Attempt to fill VRT field with null register");
   TR_ASSERT_FATAL_WITH_INSTRUCTION(instr, reg->getKind() == TR_VRF, "Attempt to fill VRT field with %s, which is not a VR", reg->getRegisterName(instr->cg()->comp()));
   reg->setRegisterFieldRT(cursor);
   }

static void fillFieldXT(TR::Instruction *instr, uint32_t *cursor, TR::RealRegister *reg)
   {
   TR_ASSERT_FATAL_WITH_INSTRUCTION(instr, reg, "Attempt to fill XT field with null register");
   TR_ASSERT_FATAL_WITH_INSTRUCTION(instr, canUseAsVsxRegister(reg), "Attempt to fill XT field with %s, which is not a VSR", reg->getRegisterName(instr->cg()->comp()));
   reg->setRegisterFieldXT(cursor);
   }

static void fillFieldRS(TR::Instruction *instr, uint32_t *cursor, TR::RealRegister *reg)
   {
   TR_ASSERT_FATAL_WITH_INSTRUCTION(instr, reg, "Attempt to fill RS field with null register");
   TR_ASSERT_FATAL_WITH_INSTRUCTION(instr, reg->getKind() == TR_GPR, "Attempt to fill RS field with %s, which is not a GPR", reg->getRegisterName(instr->cg()->comp()));
   reg->setRegisterFieldRS(cursor);
   }

static void fillFieldXS(TR::Instruction *instr, uint32_t *cursor, TR::RealRegister *reg)
   {
   TR_ASSERT_FATAL_WITH_INSTRUCTION(instr, reg, "Attempt to fill XS field with null register");
   TR_ASSERT_FATAL_WITH_INSTRUCTION(instr, canUseAsVsxRegister(reg), "Attempt to fill XS field with %s, which is not a VSR", reg->getRegisterName(instr->cg()->comp()));
   reg->setRegisterFieldXS(cursor);
   }

static void fillFieldRA(TR::Instruction *instr, uint32_t *cursor, TR::RealRegister *reg)
   {
   TR_ASSERT_FATAL_WITH_INSTRUCTION(instr, reg, "Attempt to fill RA field with null register");
   TR_ASSERT_FATAL_WITH_INSTRUCTION(instr, reg->getKind() == TR_GPR, "Attempt to fill RA field with %s, which is not a GPR", reg->getRegisterName(instr->cg()->comp()));
   reg->setRegisterFieldRA(cursor);
   }

static void fillFieldFRB(TR::Instruction *instr, uint32_t *cursor, TR::RealRegister *reg)
   {
   TR_ASSERT_FATAL_WITH_INSTRUCTION(instr, reg, "Attempt to fill FRB field with null register");
   TR_ASSERT_FATAL_WITH_INSTRUCTION(instr, reg->getKind() == TR_FPR, "Attempt to fill FRB field with %s, which is not an FPR", reg->getRegisterName(instr->cg()->comp()));
   reg->setRegisterFieldFRB(cursor);
   }

static void fillFieldVRB(TR::Instruction *instr, uint32_t *cursor, TR::RealRegister *reg)
   {
   TR_ASSERT_FATAL_WITH_INSTRUCTION(instr, reg, "Attempt to fill VRB field with null register");
   TR_ASSERT_FATAL_WITH_INSTRUCTION(instr, reg->getKind() == TR_VRF, "Attempt to fill VRB field with %s, which is not a VR", reg->getRegisterName(instr->cg()->comp()));
   reg->setRegisterFieldRB(cursor);
   }

static void fillFieldXB(TR::Instruction *instr, uint32_t *cursor, TR::RealRegister *reg)
   {
   TR_ASSERT_FATAL_WITH_INSTRUCTION(instr, reg, "Attempt to fill XB field with null register");
   TR_ASSERT_FATAL_WITH_INSTRUCTION(instr, canUseAsVsxRegister(reg), "Attempt to fill XB field with %s, which is not a VSR", reg->getRegisterName(instr->cg()->comp()));
   reg->setRegisterFieldXB(cursor);
   }

static void fillFieldBI(TR::Instruction *instr, uint32_t *cursor, TR::RealRegister *reg)
   {
   TR_ASSERT_FATAL_WITH_INSTRUCTION(instr, reg, "Attempt to fill BI field with null register");
   TR_ASSERT_FATAL_WITH_INSTRUCTION(instr, reg->getKind() == TR_CCR, "Attempt to fill BI field with %s, which is not a CCR", reg->getRegisterName(instr->cg()->comp()));
   reg->setRegisterFieldBI(cursor);
   }

static void fillFieldBF(TR::Instruction *instr, uint32_t *cursor, TR::RealRegister *reg)
   {
   TR_ASSERT_FATAL_WITH_INSTRUCTION(instr, reg, "Attempt to fill BF field with null register");
   TR_ASSERT_FATAL_WITH_INSTRUCTION(instr, reg->getKind() == TR_CCR, "Attempt to fill BF field with %s, which is not a CCR", reg->getRegisterName(instr->cg()->comp()));
   reg->setRegisterFieldRT(cursor);
   }

static void fillFieldBFA(TR::Instruction *instr, uint32_t *cursor, TR::RealRegister *reg)
   {
   TR_ASSERT_FATAL_WITH_INSTRUCTION(instr, reg, "Attempt to fill BFA field with null register");
   TR_ASSERT_FATAL_WITH_INSTRUCTION(instr, reg->getKind() == TR_CCR, "Attempt to fill BFA field with %s, which is not a CCR", reg->getRegisterName(instr->cg()->comp()));
   reg->setRegisterFieldRA(cursor);
   }

static void fillFieldU(TR::Instruction *instr, uint32_t *cursor, uint32_t val)
   {
   TR_ASSERT_FATAL_WITH_INSTRUCTION(instr, (val & 0xfu) == val, "0x%x is out-of-range for U field", val);
   *cursor |= val << 12;
   }

static void fillFieldBFW(TR::Instruction *instr, uint32_t *cursor, uint32_t val)
   {
   TR_ASSERT_FATAL_WITH_INSTRUCTION(instr, (val & 0xfu) == val, "0x%x is out-of-range for BF/W field", val);
   *cursor |= ((val ^ 0x8) & 0x8) << 13;
   *cursor |= (val & 0x7) << 23;
   }

static void fillFieldFLM(TR::Instruction *instr, uint32_t *cursor, uint32_t val)
   {
   TR_ASSERT_FATAL_WITH_INSTRUCTION(instr, (val & 0xffu) == val, "0x%x is out-of-range for FLM field", val);
   *cursor |= val << 17;
   }

static void fillFieldFXM(TR::Instruction *instr, uint32_t *cursor, uint32_t val)
   {
   TR_ASSERT_FATAL_WITH_INSTRUCTION(instr, (val & 0xffu) == val, "0x%x is out-of-range for FXM field", val);
   *cursor |= val << 12;
   }

static void fillFieldFXM1(TR::Instruction *instr, uint32_t *cursor, uint32_t val)
   {
   TR_ASSERT_FATAL_WITH_INSTRUCTION(instr, populationCount(val), "0x%x is invalid for FXM field, expecting exactly 1 bit set", val);
   fillFieldFXM(instr, cursor, val);
   }

static void fillFieldSI(TR::Instruction *instr, uint32_t *cursor, uint32_t val)
   {
   TR_ASSERT_FATAL_WITH_INSTRUCTION(instr, isValidInSignExtendedField(val, 0xffffu), "0x%x is out-of-range for SI field", val);
   *cursor |= val & 0xffff;
   }

static void fillFieldSI5(TR::Instruction *instr, uint32_t *cursor, uint32_t val)
   {
   TR_ASSERT_FATAL_WITH_INSTRUCTION(instr, isValidInSignExtendedField(val, 0x1fu), "0x%x is out-of-range for SI(5) field", val);
   *cursor |= (val & 0x1f) << 11;
   }

uint8_t *
OMR::Power::Instruction::generateBinaryEncoding()
   {
   uint8_t *instructionStart = self()->cg()->getBinaryBufferCursor();
   uint8_t *cursor = instructionStart;
   TR::InstOpCode& opCode = self()->getOpCode();

   cursor = opCode.copyBinaryToBuffer(cursor);
   self()->fillBinaryEncodingFields(reinterpret_cast<uint32_t*>(cursor));

   cursor += opCode.getBinaryLength();

   TR_ASSERT_FATAL_WITH_INSTRUCTION(
      self(),
      (cursor - instructionStart) <= getEstimatedBinaryLength(),
      "Estimated binary length was %u bytes, but actual length was %u bytes",
      getEstimatedBinaryLength(),
      static_cast<uint32_t>(cursor - instructionStart)
   );

   self()->setBinaryLength(cursor - instructionStart);
   self()->setBinaryEncoding(instructionStart);

   return cursor;
   }

void
OMR::Power::Instruction::fillBinaryEncodingFields(uint32_t *cursor)
   {
   switch (self()->getOpCode().getFormat())
      {
      case FORMAT_NONE:
         break;

      case FORMAT_DIRECT:
         // TODO: Split genop into two instructions depending on version of Power in use
         if (self()->getOpCodeValue() == TR::InstOpCode::genop)
            {
            TR::RealRegister *r = self()->cg()->machine()->getRealRegister(TR::Compiler->target.cpu.id() > TR_PPCp6 ? TR::RealRegister::gr2 : TR::RealRegister::gr1);
            fillFieldRA(self(), cursor, r);
            fillFieldRS(self(), cursor, r);
            }
         break;

      default:
         TR_ASSERT_FATAL_WITH_INSTRUCTION(self(), false, "Format %d cannot be binary encoded by Instruction", self()->getOpCode().getFormat());
      }
   }

int32_t
OMR::Power::Instruction::estimateBinaryLength(int32_t currentEstimate)
   {
   int8_t maxLength = self()->getOpCode().getMaxBinaryLength();

   self()->setEstimatedBinaryLength(maxLength);
   return currentEstimate + maxLength;
   }

uint8_t *TR::PPCAlignmentNopInstruction::generateBinaryEncoding()
   {
   bool trace = cg()->comp()->getOption(TR_TraceCG);
   uint32_t currentMisalign = reinterpret_cast<uintptr_t>(cg()->getBinaryBufferCursor()) % _alignment;

   if (currentMisalign)
      {
      uint32_t nopsToAdd = (_alignment - currentMisalign) / PPC_INSTRUCTION_LENGTH;

      // For performance reasons, the last nop added might be different than the others, e.g. on P6
      // and above a group-ending nop is typically used. Since we add nops in reverse order, we add
      // this special nop first. All other padding instructions will be regular nops.
      TR::Instruction *lastNop = generateInstruction(cg(), getOpCodeValue(), getNode(), self());
      lastNop->setEstimatedBinaryLength(PPC_INSTRUCTION_LENGTH);

      if (trace)
         traceMsg(cg()->comp(), "Expanding alignment nop %p into %u instructions: [ %p ", self(), nopsToAdd, lastNop);

      for (uint32_t i = 1; i < nopsToAdd; i++)
         {
         TR::Instruction *nop = generateInstruction(cg(), TR::InstOpCode::nop, getNode(), self());
         nop->setEstimatedBinaryLength(PPC_INSTRUCTION_LENGTH);

         if (trace)
            traceMsg(cg()->comp(), "%p ", nop);
         }

      if (trace)
         traceMsg(cg()->comp(), "]\n");
      }
   else
      {
      if (trace)
         traceMsg(cg()->comp(), "Eliminating alignment nop %p, since the next instruction is already aligned\n", self());
      }

   cg()->addAccumulatedInstructionLengthError(getEstimatedBinaryLength() - currentMisalign);

   // When the trace log prints the list of instructions after binary encoding, we don't want this
   // instruction to show up any more. Removing it from the linked list of instructions does this
   // without affecting this instruction's next pointer, so the binary encoding loop can continue
   // and encode the actual nops we emitted as if nothing happened.
   self()->remove();

   return cg()->getBinaryBufferCursor();
   }

int32_t TR::PPCAlignmentNopInstruction::estimateBinaryLength(int32_t currentEstimate)
   {
   self()->setEstimatedBinaryLength(_alignment - PPC_INSTRUCTION_LENGTH);
   return currentEstimate + self()->getEstimatedBinaryLength();
   }

uint8_t TR::PPCAlignmentNopInstruction::getBinaryLengthLowerBound()
   {
   return 0;
   }

void TR::PPCLabelInstruction::fillBinaryEncodingFields(uint32_t *cursor)
   {
   TR::LabelSymbol *label = getLabelSymbol();

   switch (getOpCode().getFormat())
      {
      case FORMAT_NONE:
         if (getOpCodeValue() == TR::InstOpCode::label)
            label->setCodeLocation(reinterpret_cast<uint8_t*>(cursor));
         break;

      case FORMAT_I_FORM:
         if (label->getCodeLocation())
            cg()->apply24BitLabelRelativeRelocation(reinterpret_cast<int32_t*>(cursor), label);
         else
            cg()->addRelocation(new (cg()->trHeapMemory()) TR::LabelRelative24BitRelocation(reinterpret_cast<uint8_t*>(cursor), label));
         break;

      default:
         TR_ASSERT_FATAL_WITH_INSTRUCTION(self(), false, "Format %d cannot be binary encoded by PPCLabelInstruction", getOpCode().getFormat());
      }
   }

int32_t TR::PPCLabelInstruction::estimateBinaryLength(int32_t currentEstimate)
   {
   if (getOpCodeValue() == TR::InstOpCode::label)
      getLabelSymbol()->setEstimatedCodeLocation(currentEstimate);

   return self()->TR::Instruction::estimateBinaryLength(currentEstimate);
   }

// TODO This should probably be refactored and moved onto OMR::Power::InstOpCode
static bool reversedConditionalBranchOpCode(TR::InstOpCode::Mnemonic op, TR::InstOpCode::Mnemonic *rop)
   {
   switch (op)
      {
      case TR::InstOpCode::bdnz:
         *rop = TR::InstOpCode::bdz;
         return(false);
      case TR::InstOpCode::bdz:
         *rop = TR::InstOpCode::bdnz;
         return(false);
      case TR::InstOpCode::beq:
         *rop = TR::InstOpCode::bne;
         return(false);
      case TR::InstOpCode::beql:
         *rop = TR::InstOpCode::bne;
         return(true);
      case TR::InstOpCode::bge:
         *rop = TR::InstOpCode::blt;
         return(false);
      case TR::InstOpCode::bgel:
         *rop = TR::InstOpCode::blt;
         return(true);
      case TR::InstOpCode::bgt:
         *rop = TR::InstOpCode::ble;
         return(false);
      case TR::InstOpCode::bgtl:
         *rop = TR::InstOpCode::ble;
         return(true);
      case TR::InstOpCode::ble:
         *rop = TR::InstOpCode::bgt;
         return(false);
      case TR::InstOpCode::blel:
         *rop = TR::InstOpCode::bgt;
         return(true);
      case TR::InstOpCode::blt:
         *rop = TR::InstOpCode::bge;
         return(false);
      case TR::InstOpCode::bltl:
         *rop = TR::InstOpCode::bge;
         return(true);
      case TR::InstOpCode::bne:
         *rop = TR::InstOpCode::beq;
         return(false);
      case TR::InstOpCode::bnel:
         *rop = TR::InstOpCode::beq;
         return(true);
      case TR::InstOpCode::bnun:
         *rop = TR::InstOpCode::bun;
         return(false);
      case TR::InstOpCode::bun:
         *rop = TR::InstOpCode::bnun;
         return(false);
      default:
         TR_ASSERT(0, "New PPC conditional branch opcodes have to have corresponding reversed opcode: %d\n", (int32_t)op);
         *rop = TR::InstOpCode::bad;
         return(false);
      }
   }

void TR::PPCConditionalBranchInstruction::expandIntoFarBranch()
   {
   TR_ASSERT_FATAL_WITH_INSTRUCTION(self(), getLabelSymbol(), "Cannot expand conditional branch without a label");

   if (comp()->getOption(TR_TraceCG))
      traceMsg(comp(), "Expanding conditional branch instruction %p into a far branch\n", self());

   TR::InstOpCode::Mnemonic newOpCode;
   bool wasLinkForm = reversedConditionalBranchOpCode(getOpCodeValue(), &newOpCode);

   setOpCodeValue(newOpCode);

   TR::LabelSymbol *skipBranchLabel = generateLabelSymbol(cg());
   skipBranchLabel->setEstimatedCodeLocation(getEstimatedBinaryLocation() + 4);

   TR::Instruction *branchInstr = generateLabelInstruction(cg(), wasLinkForm ? TR::InstOpCode::bl : TR::InstOpCode::b, getNode(), getLabelSymbol(), self());
   branchInstr->setEstimatedBinaryLength(4);

   TR::Instruction *labelInstr = generateLabelInstruction(cg(), TR::InstOpCode::label, getNode(), skipBranchLabel, branchInstr);
   labelInstr->setEstimatedBinaryLength(0);

   setLabelSymbol(skipBranchLabel);
   setEstimatedBinaryLength(4);
   reverseLikeliness();
   _farRelocation = true;
   }

void TR::PPCConditionalBranchInstruction::fillBinaryEncodingFields(uint32_t *cursor)
   {
   switch (getOpCode().getFormat())
      {
      case FORMAT_B_FORM:
         {
         TR::LabelSymbol *label = getLabelSymbol();
         TR_ASSERT_FATAL_WITH_INSTRUCTION(self(), label, "B-form conditional branch has no label");

         if (label->getCodeLocation())
            cg()->apply16BitLabelRelativeRelocation(reinterpret_cast<int32_t*>(cursor), label);
         else
            cg()->addRelocation(new (cg()->trHeapMemory()) TR::LabelRelative16BitRelocation(reinterpret_cast<uint8_t*>(cursor), label));
         break;
         }

      case FORMAT_XL_FORM_BRANCH:
         TR_ASSERT_FATAL_WITH_INSTRUCTION(self(), !getLabelSymbol(), "XL-form conditional branch has a label");
         break;

      default:
         TR_ASSERT_FATAL_WITH_INSTRUCTION(self(), false, "Format %d cannot be binary encoded by PPCConditionalBranchInstruction", getOpCode().getFormat());
      }

   fillFieldBI(self(), cursor, toRealRegister(_conditionRegister));
   if (haveHint())
      {
      if (getOpCode().setsCTR())
         *cursor |= (getLikeliness() ? PPCOpProp_BranchLikelyMaskCtr : PPCOpProp_BranchUnlikelyMaskCtr);
      else
         *cursor |= (getLikeliness() ? PPCOpProp_BranchLikelyMask : PPCOpProp_BranchUnlikelyMask);
      }
   }

int32_t TR::PPCConditionalBranchInstruction::estimateBinaryLength(int32_t currentEstimate)
   {
   TR_ASSERT_FATAL_WITH_INSTRUCTION(self(), getOpCode().getMaxBinaryLength() == PPC_INSTRUCTION_LENGTH, "Format %d cannot be binary encoded by PPCConditionalBranchInstruction", getOpCode().getFormat());

   // Conditional branches can be expanded into a conditional branch around an unconditional branch if the target label
   // is out of range for a simple bc instruction. This is done by expandFarConditionalBranches, which runs after binary
   // length estimation but before binary encoding and will call PPCConditionalBranchInstruction::expandIntoFarBranch to
   // expand the branch into two instructions. For this reason, we conservatively assume that any conditional branch
   // could be expanded to ensure that the binary length estimates are correct.
   setEstimatedBinaryLength(PPC_INSTRUCTION_LENGTH * 2);
   setEstimatedBinaryLocation(currentEstimate);

   return currentEstimate + getEstimatedBinaryLength();
   }

void TR::PPCAdminInstruction::fillBinaryEncodingFields(uint32_t *cursor)
   {
   TR_ASSERT_FATAL_WITH_INSTRUCTION(self(), getOpCode().getFormat() == FORMAT_NONE, "Format %d cannot be binary encoded by PPCAdminInstruction", getOpCode().getFormat());

   if (getOpCodeValue() == TR::InstOpCode::fence)
      {
      TR_ASSERT_FATAL_WITH_INSTRUCTION(self(), _fenceNode, "Fence instruction is missing a fence node");
      TR_ASSERT_FATAL_WITH_INSTRUCTION(self(), _fenceNode->getRelocationType() == TR_EntryRelative32Bit, "Unhandled relocation type %u", _fenceNode->getRelocationType());

      for (int i = 0; i < _fenceNode->getNumRelocations(); i++)
         *static_cast<uint32_t*>(_fenceNode->getRelocationDestination(i)) = cg()->getCodeLength();
      }
   else
      {
      TR_ASSERT_FATAL_WITH_INSTRUCTION(self(), !_fenceNode, "Non-fence instruction has a fence node %p", _fenceNode);
      }
   }

void
TR::PPCImmInstruction::addMetaDataForCodeAddress(uint8_t *cursor)
   {

   if (needsAOTRelocation())
      {
         switch(getReloKind())
            {
            case TR_AbsoluteHelperAddress:
               cg()->addExternalRelocation(new (cg()->trHeapMemory()) TR::ExternalRelocation(cursor, (uint8_t *)getSymbolReference(), TR_AbsoluteHelperAddress, cg()), __FILE__, __LINE__, getNode());
               break;
            case TR_RamMethod:
               if (comp()->getOption(TR_UseSymbolValidationManager))
                  {
                  cg()->addExternalRelocation(
                     new (comp()->trHeapMemory()) TR::ExternalRelocation(
                        cursor,
                        (uint8_t *)comp()->getJittedMethodSymbol()->getResolvedMethod()->resolvedMethodAddress(),
                        (uint8_t *)TR::SymbolType::typeMethod,
                        TR_SymbolFromManager,
                        cg()),
                     __FILE__,
                     __LINE__,
                     getNode());
                  }
               else
                  {
                  cg()->addExternalRelocation(new (cg()->trHeapMemory()) TR::ExternalRelocation(cursor, NULL, TR_RamMethod, cg()), __FILE__, __LINE__, getNode());
                  }
               break;
            case TR_BodyInfoAddress:
               cg()->addExternalRelocation(new (cg()->trHeapMemory()) TR::ExternalRelocation(cursor, 0, TR_BodyInfoAddress, cg()), __FILE__, __LINE__, getNode());
               break;
            default:
               TR_ASSERT(false, "Unsupported AOT relocation type specified.");
            }
      }

   TR::Compilation *comp = cg()->comp();
   if (std::find(comp->getStaticPICSites()->begin(), comp->getStaticPICSites()->end(), this) != comp->getStaticPICSites()->end())
      {
      // none-HCR: low-tag to invalidate -- BE or LE is relevant
      //
      void *valueToHash = *(void**)(cursor - (comp->target().is64Bit()?4:0));
      void *addressToPatch = comp->target().is64Bit()?
         (comp->target().cpu.isBigEndian()?cursor:(cursor-4)) : cursor;
      cg()->jitAddPicToPatchOnClassUnload(valueToHash, addressToPatch);
      }

   if (std::find(comp->getStaticHCRPICSites()->begin(), comp->getStaticHCRPICSites()->end(), this) != comp->getStaticHCRPICSites()->end())
      {
      // HCR: whole pointer replacement.
      //
      void **locationToPatch = (void**)(cursor - (comp->target().is64Bit()?4:0));
      cg()->jitAddPicToPatchOnClassRedefinition(*locationToPatch, locationToPatch);
      cg()->addExternalRelocation(new (cg()->trHeapMemory()) TR::ExternalRelocation((uint8_t *)locationToPatch, (uint8_t *)*locationToPatch, TR_HCR, cg()), __FILE__,__LINE__, getNode());
      }

   }

void TR::PPCImmInstruction::fillBinaryEncodingFields(uint32_t *cursor)
   {
   addMetaDataForCodeAddress(reinterpret_cast<uint8_t*>(cursor));

   switch (getOpCode().getFormat())
      {
      case FORMAT_DD:
         *cursor = getSourceImmediate();
         break;

      default:
         TR_ASSERT_FATAL_WITH_INSTRUCTION(self(), false, "Format %d cannot be binary encoded by PPCImmInstruction", getOpCode().getFormat());
      }
   }

void TR::PPCImm2Instruction::fillBinaryEncodingFields(uint32_t* cursor)
   {
   uint32_t imm1 = getSourceImmediate();
   uint32_t imm2 = getSourceImmediate2();

   switch (getOpCode().getFormat())
      {
      case FORMAT_MTFSFI:
         fillFieldU(self(), cursor, imm1);
         fillFieldBFW(self(), cursor, imm2);
         break;

      default:
         TR_ASSERT_FATAL_WITH_INSTRUCTION(self(), false, "Format %d cannot be binary encoded by PPCImm2Instruction", getOpCode().getFormat());
      }
   }

void TR::PPCSrc1Instruction::fillBinaryEncodingFields(uint32_t *cursor)
   {
   TR::RealRegister *src = toRealRegister(getSource1Register());
   uint32_t imm = getSourceImmediate();

   switch (getOpCode().getFormat())
      {
      case FORMAT_MTFSF:
         fillFieldFRB(self(), cursor, src);
         fillFieldFLM(self(), cursor, imm);
         break;

      case FORMAT_RS:
         fillFieldRS(self(), cursor, src);
         break;

      case FORMAT_RA_SI:
         fillFieldRA(self(), cursor, src);
         fillFieldSI(self(), cursor, imm);
         break;

      case FORMAT_RA_SI5:
         fillFieldRA(self(), cursor, src);
         fillFieldSI5(self(), cursor, imm);
         break;

      case FORMAT_RS_FXM:
         fillFieldRS(self(), cursor, src);
         fillFieldFXM(self(), cursor, imm);
         break;

      case FORMAT_RS_FXM1:
         fillFieldRS(self(), cursor, src);
         fillFieldFXM1(self(), cursor, imm);
         break;

      default:
         TR_ASSERT_FATAL_WITH_INSTRUCTION(self(), false, "Format %d cannot be binary encoded by PPCSrc1Instruction", getOpCode().getFormat());
      }
   }

void TR::PPCTrg1Instruction::fillBinaryEncodingFields(uint32_t *cursor)
   {
   TR::RealRegister *trg = toRealRegister(getTargetRegister());

   switch (getOpCode().getFormat())
      {
      case FORMAT_RT:
         fillFieldRT(self(), cursor, trg);
         break;

      default:
         TR_ASSERT_FATAL_WITH_INSTRUCTION(self(), false, "Format %d cannot be binary encoded by PPCTrg1Instruction", getOpCode().getFormat());
      }
   }

void TR::PPCTrg1Src1Instruction::fillBinaryEncodingFields(uint32_t *cursor)
   {
   TR::RealRegister *trg = toRealRegister(getTargetRegister());
   TR::RealRegister *src = toRealRegister(getSource1Register());

   switch (getOpCode().getFormat())
      {
      case FORMAT_RA_RS:
         fillFieldRA(self(), cursor, trg);
         fillFieldRS(self(), cursor, src);
         break;

      case FORMAT_RT_RA:
         fillFieldRT(self(), cursor, trg);
         fillFieldRA(self(), cursor, src);
         break;

      case FORMAT_FRT_FRB:
         fillFieldFRT(self(), cursor, trg);
         fillFieldFRB(self(), cursor, src);
         break;

      case FORMAT_BF_BFA:
         fillFieldBF(self(), cursor, trg);
         fillFieldBFA(self(), cursor, src);
         break;

      case FORMAT_RA_XS:
         fillFieldRA(self(), cursor, trg);
         fillFieldXS(self(), cursor, src);
         break;

      case FORMAT_XT_RA:
         fillFieldXT(self(), cursor, trg);
         fillFieldRA(self(), cursor, src);
         break;

      case FORMAT_RT_BFA:
         fillFieldRT(self(), cursor, trg);
         fillFieldBFA(self(), cursor, src);
         break;

      case FORMAT_VRT_VRB:
         fillFieldVRT(self(), cursor, trg);
         fillFieldVRB(self(), cursor, src);
         break;

      case FORMAT_XT_XB:
         fillFieldXT(self(), cursor, trg);
         fillFieldXB(self(), cursor, src);
         break;

      default:
         TR_ASSERT_FATAL_WITH_INSTRUCTION(self(), false, "Format %d cannot be binary encoded by PPCTrg1Src1Instruction", getOpCode().getFormat());
      }
   }

int32_t TR::PPCTrg1ImmInstruction::estimateBinaryLength(int32_t currentEstimate)
   {
   setEstimatedBinaryLength(PPC_INSTRUCTION_LENGTH);
   return currentEstimate + getEstimatedBinaryLength();
   }

static TR::Instruction *loadReturnAddress(TR::Node * node, uintptr_t value, TR::Register *trgReg, TR::Instruction *cursor)
   {
   return cursor->cg()->loadAddressConstantFixed(node, value, trgReg, cursor);
   }


void
TR::PPCTrg1ImmInstruction::addMetaDataForCodeAddress(uint8_t *cursor)
   {
   TR::Compilation *comp = cg()->comp();

   if (std::find(comp->getStaticPICSites()->begin(), comp->getStaticPICSites()->end(), this) != comp->getStaticPICSites()->end())
      {
      TR::Node *node = getNode();
      cg()->jitAddPicToPatchOnClassUnload((void *)(comp->target().is64Bit()?node->getLongInt():node->getInt()), (void *)cursor);
      }

   if (std::find(comp->getStaticMethodPICSites()->begin(), comp->getStaticMethodPICSites()->end(), this) != comp->getStaticMethodPICSites()->end())
      {
      TR::Node *node = getNode();
      cg()->jitAddPicToPatchOnClassUnload((void *) (cg()->fe()->createResolvedMethod(cg()->trMemory(), (TR_OpaqueMethodBlock *) (comp->target().is64Bit()?node->getLongInt():node->getInt()), comp->getCurrentMethod())->classOfMethod()), (void *)cursor);
      }
   }


uint8_t *TR::PPCTrg1ImmInstruction::generateBinaryEncoding()
   {

   uint8_t *instructionStart = cg()->getBinaryBufferCursor();
   uint8_t *cursor           = instructionStart;
   cursor = getOpCode().copyBinaryToBuffer(instructionStart);
   insertTargetRegister(toPPCCursor(cursor));

   if (getOpCodeValue() == TR::InstOpCode::mfocrf)
      {
      *((int32_t *)cursor) |= (getSourceImmediate()<<12);
      if ((cg()->comp()->target().cpu.id() >= TR_PPCgp) &&
          ((getSourceImmediate() & (getSourceImmediate() - 1)) == 0))
         // convert to PPC AS single field form
         *((int32_t *)cursor) |= 0x00100000;
      }
   else if (getOpCodeValue() == TR::InstOpCode::mfcr)
      {
      if ((cg()->comp()->target().cpu.id() >= TR_PPCgp) &&
          ((getSourceImmediate() & (getSourceImmediate() - 1)) == 0))
         // convert to PPC AS single field form
         *((int32_t *)cursor) |= (getSourceImmediate()<<12) | 0x00100000;
      else
         TR_ASSERT(getSourceImmediate() == 0xFF, "Bad field mask on mfcr");
      }
   else
      {
      insertImmediateField(toPPCCursor(cursor));
      }

   addMetaDataForCodeAddress(cursor);

   cursor += PPC_INSTRUCTION_LENGTH;
   setBinaryLength(PPC_INSTRUCTION_LENGTH);
   setBinaryEncoding(instructionStart);
   return cursor;
   }


void
TR::PPCTrg1Src1ImmInstruction::addMetaDataForCodeAddress(uint8_t *cursor)
   {
   TR::Compilation *comp = cg()->comp();

   if (std::find(comp->getStaticPICSites()->begin(), comp->getStaticPICSites()->end(), this) != comp->getStaticPICSites()->end())
      {
      cg()->jitAddPicToPatchOnClassUnload((void *)(getSourceImmPtr()), (void *)cursor);
      }
   if (std::find(comp->getStaticMethodPICSites()->begin(), comp->getStaticMethodPICSites()->end(), this) != comp->getStaticMethodPICSites()->end())
      {
      cg()->jitAddPicToPatchOnClassUnload((void *) (cg()->fe()->createResolvedMethod(cg()->trMemory(), (TR_OpaqueMethodBlock *) (getSourceImmPtr()), comp->getCurrentMethod())->classOfMethod()), (void *)cursor);
      }
   }


uint8_t *TR::PPCTrg1Src1ImmInstruction::generateBinaryEncoding()
   {

   uint8_t *instructionStart = cg()->getBinaryBufferCursor();
   uint8_t *cursor           = instructionStart;
   cursor = getOpCode().copyBinaryToBuffer(instructionStart);
   insertTargetRegister(toPPCCursor(cursor));
   insertSource1Register(toPPCCursor(cursor));
   if (getOpCodeValue() == TR::InstOpCode::srawi || getOpCodeValue() == TR::InstOpCode::srawi_r ||
       getOpCodeValue() == TR::InstOpCode::sradi || getOpCodeValue() == TR::InstOpCode::sradi_r ||
       getOpCodeValue() == TR::InstOpCode::extswsli)
      {
      insertShiftAmount(toPPCCursor(cursor));
      }
   else if (getOpCodeValue() == TR::InstOpCode::dtstdg)
      {
      setSourceImmediate(getSourceImmediate() << 10);
      insertImmediateField(toPPCCursor(cursor));
      }
   else
      {
      insertImmediateField(toPPCCursor(cursor));
      }

   addMetaDataForCodeAddress(cursor);

   cursor += PPC_INSTRUCTION_LENGTH;
   setBinaryLength(PPC_INSTRUCTION_LENGTH);
   setBinaryEncoding(instructionStart);
   return cursor;
   }


static void insertMaskField(uint32_t *instruction, TR::InstOpCode::Mnemonic op, int64_t lmask)
   {
   int32_t encoding;
   // A mask is is a string of 1 bits surrounded by a string of 0 bits.
   // For word instructions it is specified through its start and stop bit
   // numbers.  Note - the mask is considered circular so the start bit
   // number may be greater than the stop bit number.
   // Examples:     input     start   stop
   //              00FFFF00      8     23
   //              00000001     31     31
   //              80000001     31      0
   //              FFFFFFFF      0     31  (somewhat arbitrary)
   //              00000000      ?      ?  (illegal)
   //
   // For doubleword instructions only one of the start bit or stop bit is
   // specified and the other is implicit in the instruction.  The bit
   // number is strangely encoded in that the low order bit 5 comes first
   // and the high order bits after.  The field is in bit positions 21-26.

   // For these instructions the immediate is not a mask but a 1-bit immediate operand
   if (op == TR::InstOpCode::cmprb)
      {
      // populate 1-bit L field
      encoding = (((uint32_t)lmask) & 0x1) << 21;
      *instruction |= encoding;
      return;
      }

   // For these instructions the immediate is not a mask but a 2-bit immediate operand
   if (op == TR::InstOpCode::xxpermdi ||
       op == TR::InstOpCode::xxsldwi)
      {
      encoding = (((uint32_t)lmask) & 0x3) << 8;
      *instruction |= encoding;
      return;
      }

   if (op == TR::InstOpCode::addex ||
       op == TR::InstOpCode::addex_r)
      {
      encoding = (((uint32_t)lmask) & 0x3) << 9;
      *instruction |= encoding;
      return;
      }

   // For these instructions the immediate is not a mask but a 4-bit immediate operand
   if (op == TR::InstOpCode::vsldoi)
      {
      encoding = (((uint32_t)lmask) & 0xf)<< 6;
      *instruction |= encoding;
      return;
      }

   TR::InstOpCode       opCode(op);

   if (opCode.isCRLogical())
      {
      encoding = (((uint32_t) lmask) & 0xffffffff);
      *instruction |= encoding;
      return;
      }

   TR_ASSERT(lmask, "A mask of 0 cannot be encoded");

   if (opCode.isDoubleWord())
      {
      int bitnum;

      if (opCode.useMaskEnd())
	 {
         TR_ASSERT(contiguousBits(lmask) &&
		((lmask & CONSTANT64(0x8000000000000000)) != 0) &&
		((lmask == -1) || ((lmask & 0x1) == 0)),
		"Bad doubleword mask for ME encoding");
         bitnum = leadingOnes(lmask) - 1;
	 }
      else
	 {
         bitnum = leadingZeroes(lmask);
	 // assert on cases like 0xffffff00000000ff
         TR_ASSERT((bitnum != 0) || (lmask == -1) || ((lmask & 0x1) == 0) ||
                             (op!=TR::InstOpCode::rldic   &&
                              op!=TR::InstOpCode::rldimi  &&
                              op!=TR::InstOpCode::rldic_r &&
                              op!=TR::InstOpCode::rldimi_r),
                "Cannot handle wrap-around, check mask for correctness");
	 }
      encoding = ((bitnum&0x1f)<<6) | ((bitnum&0x20));

      }
   else // single word
      {
      // special case the 3-bit rounding mode fields
      if (op == TR::InstOpCode::drrnd || op == TR::InstOpCode::dqua)
         {
         encoding = (lmask << 9) & 0x600;
         }
      else
         {
         int32_t mask = lmask&0xffffffff;
         int32_t maskBegin;
         int32_t maskEnd;

         maskBegin = leadingZeroes(~mask & (2*mask));
         maskBegin = (maskBegin + (maskBegin != 32)) & 0x1f;
         maskEnd  = leadingZeroes(mask & ~(2*mask));
         encoding = 32*maskBegin + maskEnd << 1; // shift encrypted mask into position
         }
      }
   *instruction |= encoding;
   }

uint8_t *TR::PPCTrg1Src1Imm2Instruction::generateBinaryEncoding()
   {
   uint8_t *instructionStart = cg()->getBinaryBufferCursor();
   uint8_t *cursor           = instructionStart;
   cursor = getOpCode().copyBinaryToBuffer(instructionStart);
   insertTargetRegister(toPPCCursor(cursor));
   insertSource1Register(toPPCCursor(cursor));
   insertShiftAmount(toPPCCursor(cursor));
   insertMaskField(toPPCCursor(cursor), getOpCodeValue(), getLongMask());
   cursor += PPC_INSTRUCTION_LENGTH;
   setBinaryLength(PPC_INSTRUCTION_LENGTH);
   setBinaryEncoding(instructionStart);
   return cursor;
   }


uint8_t *TR::PPCTrg1Src2Instruction::generateBinaryEncoding()
   {
   uint8_t *instructionStart = cg()->getBinaryBufferCursor();
   uint8_t *cursor           = instructionStart;

   cursor = getOpCode().copyBinaryToBuffer(instructionStart);
   insertTargetRegister(toPPCCursor(cursor));
   insertSource1Register(toPPCCursor(cursor));
   insertSource2Register(toPPCCursor(cursor));
   cursor += PPC_INSTRUCTION_LENGTH;
   setBinaryLength(PPC_INSTRUCTION_LENGTH);
   setBinaryEncoding(instructionStart);
   return cursor;
   }

uint8_t *TR::PPCTrg1Src2ImmInstruction::generateBinaryEncoding()
   {
   uint8_t *instructionStart = cg()->getBinaryBufferCursor();
   uint8_t *cursor           = instructionStart;
   cursor = getOpCode().copyBinaryToBuffer(instructionStart);
   insertTargetRegister(toPPCCursor(cursor));
   insertSource1Register(toPPCCursor(cursor));
   insertSource2Register(toPPCCursor(cursor));
   insertMaskField(toPPCCursor(cursor), getOpCodeValue(), getLongMask());
   cursor += PPC_INSTRUCTION_LENGTH;
   setBinaryLength(PPC_INSTRUCTION_LENGTH);
   setBinaryEncoding(instructionStart);

   return cursor;
   }

uint8_t *TR::PPCTrg1Src3Instruction::generateBinaryEncoding()
   {
   uint8_t *instructionStart = cg()->getBinaryBufferCursor();
   uint8_t *cursor           = instructionStart;
   cursor = getOpCode().copyBinaryToBuffer(instructionStart);
   insertTargetRegister(toPPCCursor(cursor));
   insertSource1Register(toPPCCursor(cursor));
   insertSource2Register(toPPCCursor(cursor));
   insertSource3Register(toPPCCursor(cursor));
   cursor += PPC_INSTRUCTION_LENGTH;
   setBinaryLength(PPC_INSTRUCTION_LENGTH);
   setBinaryEncoding(instructionStart);
   return cursor;
   }

uint8_t *TR::PPCSrc2Instruction::generateBinaryEncoding()
   {
   uint8_t *instructionStart = cg()->getBinaryBufferCursor();
   uint8_t *cursor           = instructionStart;
   cursor = getOpCode().copyBinaryToBuffer(instructionStart);
   insertSource1Register(toPPCCursor(cursor));
   insertSource2Register(toPPCCursor(cursor));

   cursor += PPC_INSTRUCTION_LENGTH;
   setBinaryLength(PPC_INSTRUCTION_LENGTH);
   setBinaryEncoding(instructionStart);
   return cursor;
   }


uint8_t *TR::PPCMemSrc1Instruction::generateBinaryEncoding()
   {
   uint8_t *instructionStart = cg()->getBinaryBufferCursor();
   uint8_t *cursor           = instructionStart;

   getMemoryReference()->mapOpCode(this);

   cursor = getOpCode().copyBinaryToBuffer(instructionStart);

   insertSourceRegister(toPPCCursor(cursor));
   cursor = getMemoryReference()->generateBinaryEncoding(this, cursor, cg());
   setBinaryLength(cursor-instructionStart);
   setBinaryEncoding(instructionStart);
   cg()->addAccumulatedInstructionLengthError(getEstimatedBinaryLength() - getBinaryLength());
   return cursor;
   }

uint8_t *TR::PPCMemInstruction::generateBinaryEncoding()
   {
   uint8_t *instructionStart = cg()->getBinaryBufferCursor();
   uint8_t *cursor           = instructionStart;
   getMemoryReference()->mapOpCode(this);
   cursor = getOpCode().copyBinaryToBuffer(instructionStart);
   cursor = getMemoryReference()->generateBinaryEncoding(this, cursor, cg());
   setBinaryLength(cursor-instructionStart);
   setBinaryEncoding(instructionStart);
   cg()->addAccumulatedInstructionLengthError(getEstimatedBinaryLength() - getBinaryLength());
   return cursor;
   }
int32_t TR::PPCMemSrc1Instruction::estimateBinaryLength(int32_t currentEstimate)
   {
   setEstimatedBinaryLength(getMemoryReference()->estimateBinaryLength(*cg()));
   return(currentEstimate + getEstimatedBinaryLength());
   }

uint8_t *TR::PPCTrg1MemInstruction::generateBinaryEncoding()
   {
   uint8_t *instructionStart = cg()->getBinaryBufferCursor();
   uint8_t *cursor           = instructionStart;

   getMemoryReference()->mapOpCode(this);

   cursor = getOpCode().copyBinaryToBuffer(instructionStart);

   insertTargetRegister(toPPCCursor(cursor));
   // Set hint bit if there is any
   // The control for the different values is done through asserts in the constructor
   if (haveHint())
   {
      *(int32_t *)instructionStart |=  getHint();
   }

   cursor = getMemoryReference()->generateBinaryEncoding(this, cursor, cg());
   setBinaryLength(cursor-instructionStart);
   setBinaryEncoding(instructionStart);
   cg()->addAccumulatedInstructionLengthError(getEstimatedBinaryLength() - getBinaryLength());
   return cursor;
   }

int32_t TR::PPCTrg1MemInstruction::estimateBinaryLength(int32_t currentEstimate)
   {
   setEstimatedBinaryLength(getMemoryReference()->estimateBinaryLength(*cg()));
   return currentEstimate + getEstimatedBinaryLength();
   }

uint8_t *TR::PPCControlFlowInstruction::generateBinaryEncoding()
   {
   uint8_t *instructionStart = cg()->getBinaryBufferCursor();
   uint8_t *cursor           = instructionStart;
   cursor = getOpCode().copyBinaryToBuffer(instructionStart);
   setBinaryLength(0);
   return cursor;
   }


int32_t TR::PPCControlFlowInstruction::estimateBinaryLength(int32_t currentEstimate)
   {
   switch(getOpCodeValue())
      {
      case TR::InstOpCode::iflong:
      case TR::InstOpCode::setbool:
      case TR::InstOpCode::idiv:
      case TR::InstOpCode::ldiv:
      case TR::InstOpCode::iselect:
         if (useRegPairForResult())
            {
            if (!useRegPairForCond())
               setEstimatedBinaryLength(PPC_INSTRUCTION_LENGTH * 6);
            else
               setEstimatedBinaryLength(PPC_INSTRUCTION_LENGTH * 8);
            }
         else
            {
            if (!useRegPairForCond())
               setEstimatedBinaryLength(PPC_INSTRUCTION_LENGTH * 4);
            else
               setEstimatedBinaryLength(PPC_INSTRUCTION_LENGTH * 6);
            }
         break;
      case TR::InstOpCode::setbflt:
         setEstimatedBinaryLength(PPC_INSTRUCTION_LENGTH * 5);
         break;
      case TR::InstOpCode::setblong:
      case TR::InstOpCode::flcmpg:
      case TR::InstOpCode::flcmpl:
      case TR::InstOpCode::irem:
      case TR::InstOpCode::lrem:
      case TR::InstOpCode::d2i:
         setEstimatedBinaryLength(PPC_INSTRUCTION_LENGTH * 6);
         break;
      case TR::InstOpCode::d2l:
         if (cg()->comp()->target().is64Bit())
            setEstimatedBinaryLength(PPC_INSTRUCTION_LENGTH * 6);
       else
            setEstimatedBinaryLength(PPC_INSTRUCTION_LENGTH * 8);
         break;
      case TR::InstOpCode::lcmp:
         setEstimatedBinaryLength(PPC_INSTRUCTION_LENGTH * 11);
         break;
      default:
         TR_ASSERT(false,"unknown control flow instruction (estimateBinaryLength)");
      }
   return currentEstimate + getEstimatedBinaryLength();
   }

#ifdef J9_PROJECT_SPECIFIC
uint8_t *TR::PPCVirtualGuardNOPInstruction::generateBinaryEncoding()
   {
   uint8_t    *cursor           = cg()->getBinaryBufferCursor();
   TR::LabelSymbol *label        = getLabelSymbol();
   int32_t     length = 0;

   _site->setLocation(cursor);
   if (label->getCodeLocation() == NULL)
      {
      _site->setDestination(cursor);
      cg()->addRelocation(new (cg()->trHeapMemory()) TR::LabelAbsoluteRelocation((uint8_t *) (&_site->getDestination()), label));

#ifdef DEBUG
   if (debug("traceVGNOP"))
      printf("####> virtual location = %p, label (relocation) = %p\n", cursor, label);
#endif
      }
   else
      {
       _site->setDestination(label->getCodeLocation());
#ifdef DEBUG
   if (debug("traceVGNOP"))
      printf("####> virtual location = %p, label location = %p\n", cursor, label->getCodeLocation());
#endif
      }

   setBinaryEncoding(cursor);
   if (cg()->sizeOfInstructionToBePatched(this) == 0 ||
       // AOT needs an explicit nop, even if there are patchable instructions at this site because
       // 1) Those instructions might have AOT data relocations (and therefore will be incorrectly patched again)
       // 2) We might want to re-enable the code path and unpatch, in which case we would have to know what the old instruction was
         cg()->comp()->compileRelocatableCode())
      {
      TR::InstOpCode opCode(TR::InstOpCode::nop);
      opCode.copyBinaryToBuffer(cursor);
      length = PPC_INSTRUCTION_LENGTH;
      }

   setBinaryLength(length);
   cg()->addAccumulatedInstructionLengthError(getEstimatedBinaryLength() - getBinaryLength());
   return cursor+length;
   }

int32_t TR::PPCVirtualGuardNOPInstruction::estimateBinaryLength(int32_t currentEstimate)
   {
   // This is a conservative estimation for reserving NOP space.
   setEstimatedBinaryLength(PPC_INSTRUCTION_LENGTH);
   return currentEstimate+PPC_INSTRUCTION_LENGTH;
   }
#endif
