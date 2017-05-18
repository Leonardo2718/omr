################################################################################
##
## (c) Copyright IBM Corp. 2017, 2017
##
##  This program and the accompanying materials are made available
##  under the terms of the Eclipse Public License v1.0 and
##  Apache License v2.0 which accompanies this distribution.
##
##      The Eclipse Public License is available at
##      http://www.eclipse.org/legal/epl-v10.html
##
##      The Apache License v2.0 is available at
##      http://www.opensource.org/licenses/apache2.0.php
##
## Contributors:
##    Multiple authors (IBM Corp.) - initial implementation and documentation
################################################################################

cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

project(OMRCompiler CXX)

set(OMR_COMPILER_INCLUDE_DIRECTORIES
    ${CMAKE_CURRENT_LIST_DIR}/x/amd64
    ${CMAKE_CURRENT_LIST_DIR}/x
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/..
)

set(OMR_COMPILER_RTTI False)
set(OMR_COMPILER_STRICT_ALIASING False)
#set(OMR_COMPILER_THREADSAFE_STATICS False)
set(OMR_COMPILER_FRAME_POINTER False)

set(OMR_COMPILER_CXX_BASE_FLAGS
    -fno-rtti
    -fno-threadsafe-statics
    -fomit-frame-pointer
    -fasynchronous-unwind-tables
    -fno-dollars-in-identifiers
    -fno-strict-aliasing
    -pthread
)
set(OMR_COMPILER_CXX_ERROR_FLAGS
    -Wno-deprecated
    -Wno-enum-compare
    -Wno-invalid-offsetof
    -Wno-write-strings
    -Wreturn-type
)
set(OMR_COMPILER_CXX_PLATFORM_FLAGS
    -m64
)

# common.mk
list(APPEND OMR_COMPILER_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/compile/OSRData.cpp
    ${CMAKE_CURRENT_LIST_DIR}/compile/Method.cpp
    ${CMAKE_CURRENT_LIST_DIR}/compile/VirtualGuard.cpp
    ${CMAKE_CURRENT_LIST_DIR}/control/OMROptions.cpp
    ${CMAKE_CURRENT_LIST_DIR}/control/OptimizationPlan.cpp
    ${CMAKE_CURRENT_LIST_DIR}/control/OMRRecompilation.cpp
    ${CMAKE_CURRENT_LIST_DIR}/env/ExceptionTable.cpp
    ${CMAKE_CURRENT_LIST_DIR}/infra/Assert.cpp
    ${CMAKE_CURRENT_LIST_DIR}/infra/BitVector.cpp
    ${CMAKE_CURRENT_LIST_DIR}/infra/Checklist.cpp
    ${CMAKE_CURRENT_LIST_DIR}/infra/HashTab.cpp
    ${CMAKE_CURRENT_LIST_DIR}/infra/IGBase.cpp
    ${CMAKE_CURRENT_LIST_DIR}/infra/IGNode.cpp
    ${CMAKE_CURRENT_LIST_DIR}/infra/ILWalk.cpp
    ${CMAKE_CURRENT_LIST_DIR}/infra/InterferenceGraph.cpp
    ${CMAKE_CURRENT_LIST_DIR}/infra/OMRMonitor.cpp
    ${CMAKE_CURRENT_LIST_DIR}/infra/OMRMonitorTable.cpp
    ${CMAKE_CURRENT_LIST_DIR}/infra/Random.cpp
    ${CMAKE_CURRENT_LIST_DIR}/infra/Timer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/infra/TreeServices.cpp
    ${CMAKE_CURRENT_LIST_DIR}/infra/OMRCfg.cpp
    ${CMAKE_CURRENT_LIST_DIR}/infra/SimpleRegex.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ilgen/IlGenRequest.cpp
    ${CMAKE_CURRENT_LIST_DIR}/il/symbol/OMRSymbol.cpp
    ${CMAKE_CURRENT_LIST_DIR}/il/OMRBlock.cpp
    ${CMAKE_CURRENT_LIST_DIR}/compile/OMRSymbolReferenceTable.cpp
    ${CMAKE_CURRENT_LIST_DIR}/compile/OMRAliasBuilder.cpp
    ${CMAKE_CURRENT_LIST_DIR}/il/symbol/OMRAutomaticSymbol.cpp
    ${CMAKE_CURRENT_LIST_DIR}/il/symbol/OMRLabelSymbol.cpp
    ${CMAKE_CURRENT_LIST_DIR}/il/symbol/OMRMethodSymbol.cpp
    ${CMAKE_CURRENT_LIST_DIR}/il/symbol/OMRParameterSymbol.cpp
    ${CMAKE_CURRENT_LIST_DIR}/il/symbol/OMRRegisterMappedSymbol.cpp
    ${CMAKE_CURRENT_LIST_DIR}/il/symbol/OMRResolvedMethodSymbol.cpp
    ${CMAKE_CURRENT_LIST_DIR}/il/symbol/OMRStaticSymbol.cpp
    ${CMAKE_CURRENT_LIST_DIR}/il/OMRNode.cpp
    ${CMAKE_CURRENT_LIST_DIR}/il/NodePool.cpp
    ${CMAKE_CURRENT_LIST_DIR}/il/NodeUtils.cpp
    ${CMAKE_CURRENT_LIST_DIR}/il/OMRIL.cpp
    ${CMAKE_CURRENT_LIST_DIR}/il/OMRSymbolReference.cpp
    ${CMAKE_CURRENT_LIST_DIR}/il/Aliases.cpp
    ${CMAKE_CURRENT_LIST_DIR}/compile/OMRCompilation.cpp
    ${CMAKE_CURRENT_LIST_DIR}/compile/TLSCompilationManager.cpp
    ${CMAKE_CURRENT_LIST_DIR}/env/OMRCPU.cpp
    ${CMAKE_CURRENT_LIST_DIR}/env/OMRObjectModel.cpp
    ${CMAKE_CURRENT_LIST_DIR}/env/OMRArithEnv.cpp
    ${CMAKE_CURRENT_LIST_DIR}/env/OMRClassEnv.cpp
    ${CMAKE_CURRENT_LIST_DIR}/env/OMRDebugEnv.cpp
    ${CMAKE_CURRENT_LIST_DIR}/env/OMRVMEnv.cpp
    ${CMAKE_CURRENT_LIST_DIR}/env/SegmentProvider.cpp
    ${CMAKE_CURRENT_LIST_DIR}/env/SystemSegmentProvider.cpp
    ${CMAKE_CURRENT_LIST_DIR}/env/Region.cpp
    ${CMAKE_CURRENT_LIST_DIR}/env/StackMemoryRegion.cpp
    ${CMAKE_CURRENT_LIST_DIR}/env/OMRPersistentInfo.cpp
    ${CMAKE_CURRENT_LIST_DIR}/env/TRMemory.cpp
    ${CMAKE_CURRENT_LIST_DIR}/env/TRPersistentMemory.cpp
    ${CMAKE_CURRENT_LIST_DIR}/env/VerboseLog.cpp
    ${CMAKE_CURRENT_LIST_DIR}/il/OMRDataTypes.cpp
    ${CMAKE_CURRENT_LIST_DIR}/il/OMRTreeTop.cpp
    ${CMAKE_CURRENT_LIST_DIR}/il/OMRILOps.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ras/CallStack.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ras/CFGChecker.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ras/Debug.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ras/DebugCounter.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ras/IgnoreLocale.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ras/LimitFile.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ras/LogTracer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ras/OptionsDebug.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ras/PPCOpNames.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ras/Tree.cpp
    ${CMAKE_CURRENT_LIST_DIR}/optimizer/AsyncCheckInsertion.cpp
    ${CMAKE_CURRENT_LIST_DIR}/optimizer/BackwardBitVectorAnalysis.cpp
    ${CMAKE_CURRENT_LIST_DIR}/optimizer/BackwardIntersectionBitVectorAnalysis.cpp
    ${CMAKE_CURRENT_LIST_DIR}/optimizer/BackwardUnionBitVectorAnalysis.cpp
    ${CMAKE_CURRENT_LIST_DIR}/optimizer/BitVectorAnalysis.cpp
    ${CMAKE_CURRENT_LIST_DIR}/optimizer/CatchBlockRemover.cpp
    ${CMAKE_CURRENT_LIST_DIR}/optimizer/CFGSimplifier.cpp
    ${CMAKE_CURRENT_LIST_DIR}/optimizer/CompactLocals.cpp
    ${CMAKE_CURRENT_LIST_DIR}/optimizer/CopyPropagation.cpp
    ${CMAKE_CURRENT_LIST_DIR}/optimizer/DataFlowAnalysis.cpp
    ${CMAKE_CURRENT_LIST_DIR}/optimizer/DeadStoreElimination.cpp
    ${CMAKE_CURRENT_LIST_DIR}/optimizer/DeadTreesElimination.cpp
    ${CMAKE_CURRENT_LIST_DIR}/optimizer/DebuggingCounters.cpp
    ${CMAKE_CURRENT_LIST_DIR}/optimizer/Delayedness.cpp
    ${CMAKE_CURRENT_LIST_DIR}/optimizer/Dominators.cpp
    ${CMAKE_CURRENT_LIST_DIR}/optimizer/DominatorVerifier.cpp
    ${CMAKE_CURRENT_LIST_DIR}/optimizer/DominatorsChk.cpp
    ${CMAKE_CURRENT_LIST_DIR}/optimizer/Earliestness.cpp
    ${CMAKE_CURRENT_LIST_DIR}/optimizer/ExpressionsSimplification.cpp
    ${CMAKE_CURRENT_LIST_DIR}/optimizer/FieldPrivatizer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/optimizer/GeneralLoopUnroller.cpp
    ${CMAKE_CURRENT_LIST_DIR}/optimizer/GlobalAnticipatability.cpp
    ${CMAKE_CURRENT_LIST_DIR}/optimizer/GlobalRegisterAllocator.cpp
    ${CMAKE_CURRENT_LIST_DIR}/optimizer/Inliner.cpp
    ${CMAKE_CURRENT_LIST_DIR}/optimizer/RematTools.cpp
    ${CMAKE_CURRENT_LIST_DIR}/optimizer/InductionVariable.cpp
    ${CMAKE_CURRENT_LIST_DIR}/optimizer/IntersectionBitVectorAnalysis.cpp
    ${CMAKE_CURRENT_LIST_DIR}/optimizer/Isolatedness.cpp
    ${CMAKE_CURRENT_LIST_DIR}/optimizer/IsolatedStoreElimination.cpp
    ${CMAKE_CURRENT_LIST_DIR}/optimizer/Latestness.cpp
    ${CMAKE_CURRENT_LIST_DIR}/optimizer/LiveOnAllPaths.cpp
    ${CMAKE_CURRENT_LIST_DIR}/optimizer/LiveVariableInformation.cpp
    ${CMAKE_CURRENT_LIST_DIR}/optimizer/Liveness.cpp
    ${CMAKE_CURRENT_LIST_DIR}/optimizer/LoadExtensions.cpp
    ${CMAKE_CURRENT_LIST_DIR}/optimizer/LocalAnalysis.cpp
    ${CMAKE_CURRENT_LIST_DIR}/optimizer/LocalAnticipatability.cpp
    ${CMAKE_CURRENT_LIST_DIR}/optimizer/LocalLiveRangeReducer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/optimizer/LocalReordering.cpp
    ${CMAKE_CURRENT_LIST_DIR}/optimizer/LocalTransparency.cpp
    ${CMAKE_CURRENT_LIST_DIR}/optimizer/LoopCanonicalizer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/optimizer/LoopReducer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/optimizer/LoopReplicator.cpp
    ${CMAKE_CURRENT_LIST_DIR}/optimizer/LoopVersioner.cpp
    ${CMAKE_CURRENT_LIST_DIR}/optimizer/OMRLocalCSE.cpp
    ${CMAKE_CURRENT_LIST_DIR}/optimizer/LocalDeadStoreElimination.cpp
    ${CMAKE_CURRENT_LIST_DIR}/optimizer/LocalOpts.cpp
    ${CMAKE_CURRENT_LIST_DIR}/optimizer/OMROptimization.cpp
    ${CMAKE_CURRENT_LIST_DIR}/optimizer/OMROptimizationManager.cpp
    ${CMAKE_CURRENT_LIST_DIR}/optimizer/OMRTransformUtil.cpp
    ${CMAKE_CURRENT_LIST_DIR}/optimizer/OMROptimizer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/optimizer/OrderBlocks.cpp
    ${CMAKE_CURRENT_LIST_DIR}/optimizer/OSRDefAnalysis.cpp
    ${CMAKE_CURRENT_LIST_DIR}/optimizer/PartialRedundancy.cpp
    ${CMAKE_CURRENT_LIST_DIR}/optimizer/PreExistence.cpp
    ${CMAKE_CURRENT_LIST_DIR}/optimizer/PrefetchInsertion.cpp
    ${CMAKE_CURRENT_LIST_DIR}/optimizer/Reachability.cpp
    ${CMAKE_CURRENT_LIST_DIR}/optimizer/ReachingBlocks.cpp
    ${CMAKE_CURRENT_LIST_DIR}/optimizer/ReachingDefinitions.cpp
    ${CMAKE_CURRENT_LIST_DIR}/optimizer/RedundantAsyncCheckRemoval.cpp
    ${CMAKE_CURRENT_LIST_DIR}/optimizer/RegisterAnticipatability.cpp
    ${CMAKE_CURRENT_LIST_DIR}/optimizer/RegisterAvailability.cpp
    ${CMAKE_CURRENT_LIST_DIR}/optimizer/RegisterCandidate.cpp
    ${CMAKE_CURRENT_LIST_DIR}/optimizer/ReorderIndexExpr.cpp
    ${CMAKE_CURRENT_LIST_DIR}/optimizer/ShrinkWrapping.cpp
    ${CMAKE_CURRENT_LIST_DIR}/optimizer/SinkStores.cpp
    ${CMAKE_CURRENT_LIST_DIR}/optimizer/StripMiner.cpp
    ${CMAKE_CURRENT_LIST_DIR}/optimizer/VPConstraint.cpp
    ${CMAKE_CURRENT_LIST_DIR}/optimizer/VPHandlers.cpp
    ${CMAKE_CURRENT_LIST_DIR}/optimizer/VPHandlersCommon.cpp
    ${CMAKE_CURRENT_LIST_DIR}/optimizer/OMRValuePropagation.cpp
    ${CMAKE_CURRENT_LIST_DIR}/optimizer/ValuePropagationCommon.cpp
    ${CMAKE_CURRENT_LIST_DIR}/optimizer/OMRSimplifier.cpp
    ${CMAKE_CURRENT_LIST_DIR}/optimizer/OMRSimplifierHelpers.cpp
    ${CMAKE_CURRENT_LIST_DIR}/optimizer/OMRSimplifierHandlers.cpp
    ${CMAKE_CURRENT_LIST_DIR}/optimizer/RegDepCopyRemoval.cpp
    ${CMAKE_CURRENT_LIST_DIR}/optimizer/StructuralAnalysis.cpp
    ${CMAKE_CURRENT_LIST_DIR}/optimizer/Structure.cpp
    ${CMAKE_CURRENT_LIST_DIR}/optimizer/TranslateTable.cpp
    ${CMAKE_CURRENT_LIST_DIR}/optimizer/TrivialDeadBlockRemover.cpp
    ${CMAKE_CURRENT_LIST_DIR}/optimizer/UnionBitVectorAnalysis.cpp
    ${CMAKE_CURRENT_LIST_DIR}/optimizer/UseDefInfo.cpp
    ${CMAKE_CURRENT_LIST_DIR}/optimizer/ValueNumberInfo.cpp
    ${CMAKE_CURRENT_LIST_DIR}/optimizer/VirtualGuardCoalescer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/optimizer/VirtualGuardHeadMerger.cpp
    ${CMAKE_CURRENT_LIST_DIR}/codegen/OMRAheadOfTimeCompile.cpp
    ${CMAKE_CURRENT_LIST_DIR}/codegen/Analyser.cpp
    ${CMAKE_CURRENT_LIST_DIR}/codegen/CodeGenPrep.cpp
    ${CMAKE_CURRENT_LIST_DIR}/codegen/CodeGenGC.cpp
    ${CMAKE_CURRENT_LIST_DIR}/codegen/CodeGenRA.cpp
    ${CMAKE_CURRENT_LIST_DIR}/codegen/FrontEnd.cpp
    ${CMAKE_CURRENT_LIST_DIR}/codegen/OMRGCRegisterMap.cpp
    ${CMAKE_CURRENT_LIST_DIR}/codegen/OMRGCStackAtlas.cpp
    ${CMAKE_CURRENT_LIST_DIR}/codegen/OMRLinkage.cpp
    ${CMAKE_CURRENT_LIST_DIR}/codegen/LiveRegister.cpp
    ${CMAKE_CURRENT_LIST_DIR}/codegen/OutOfLineCodeSection.cpp
    ${CMAKE_CURRENT_LIST_DIR}/codegen/OMRRegisterDependency.cpp
    ${CMAKE_CURRENT_LIST_DIR}/codegen/Relocation.cpp
    ${CMAKE_CURRENT_LIST_DIR}/codegen/ScratchRegisterManager.cpp
    ${CMAKE_CURRENT_LIST_DIR}/codegen/StorageInfo.cpp
    ${CMAKE_CURRENT_LIST_DIR}/codegen/OMRTreeEvaluator.cpp
    ${CMAKE_CURRENT_LIST_DIR}/codegen/PreInstructionSelection.cpp
    ${CMAKE_CURRENT_LIST_DIR}/codegen/NodeEvaluation.cpp
    ${CMAKE_CURRENT_LIST_DIR}/codegen/OMRSnippet.cpp
    ${CMAKE_CURRENT_LIST_DIR}/codegen/OMRUnresolvedDataSnippet.cpp
    ${CMAKE_CURRENT_LIST_DIR}/codegen/OMRSnippetGCMap.cpp
    ${CMAKE_CURRENT_LIST_DIR}/codegen/OMRCodeGenerator.cpp
    ${CMAKE_CURRENT_LIST_DIR}/codegen/OMRCodeGenPhase.cpp
    ${CMAKE_CURRENT_LIST_DIR}/codegen/OMRMemoryReference.cpp
    ${CMAKE_CURRENT_LIST_DIR}/codegen/OMRMachine.cpp
    ${CMAKE_CURRENT_LIST_DIR}/codegen/OMRRegister.cpp
    ${CMAKE_CURRENT_LIST_DIR}/codegen/OMRRealRegister.cpp
    ${CMAKE_CURRENT_LIST_DIR}/codegen/OMRRegisterPair.cpp
    ${CMAKE_CURRENT_LIST_DIR}/codegen/OMRInstruction.cpp
    ${CMAKE_CURRENT_LIST_DIR}/env/FEBase.cpp
    ${CMAKE_CURRENT_LIST_DIR}/env/JitConfig.cpp
    ${CMAKE_CURRENT_LIST_DIR}/control/CompilationController.cpp
    ${CMAKE_CURRENT_LIST_DIR}/optimizer/FEInliner.cpp
    ${CMAKE_CURRENT_LIST_DIR}/runtime/Runtime.cpp
    ${CMAKE_CURRENT_LIST_DIR}/runtime/Trampoline.cpp
    ${CMAKE_CURRENT_LIST_DIR}/control/CompileMethod.cpp
    ${CMAKE_CURRENT_LIST_DIR}/env/OMRIO.cpp
    ${CMAKE_CURRENT_LIST_DIR}/env/OMRKnownObjectTable.cpp
    ${CMAKE_CURRENT_LIST_DIR}/env/Globals.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ilgen/IlInjector.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ilgen/IlBuilder.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ilgen/MethodBuilder.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ilgen/ThunkBuilder.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ilgen/BytecodeBuilder.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ilgen/TypeDictionary.cpp
    ${CMAKE_CURRENT_LIST_DIR}/ilgen/VirtualMachineOperandStack.cpp
    ${CMAKE_CURRENT_LIST_DIR}/runtime/Alignment.cpp
    ${CMAKE_CURRENT_LIST_DIR}/runtime/CodeCacheTypes.cpp
    ${CMAKE_CURRENT_LIST_DIR}/runtime/OMRCodeCache.cpp
    ${CMAKE_CURRENT_LIST_DIR}/runtime/OMRCodeCacheManager.cpp
    ${CMAKE_CURRENT_LIST_DIR}/runtime/OMRCodeCacheMemorySegment.cpp
    ${CMAKE_CURRENT_LIST_DIR}/runtime/OMRCodeCacheConfig.cpp
    ${CMAKE_CURRENT_LIST_DIR}/env/OMRCompilerEnv.cpp
    ${CMAKE_CURRENT_LIST_DIR}/env/PersistentAllocator.cpp
)

if(OMR_COMPILER_TARGET_ARCHITECTURE STREQUAL "x")
    list(APPEND OMR_COMPILER_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/x/codegen/BinaryCommutativeAnalyser.cpp
        ${CMAKE_CURRENT_LIST_DIR}/x/codegen/BinaryEvaluator.cpp
        ${CMAKE_CURRENT_LIST_DIR}/x/codegen/CompareAnalyser.cpp
        ${CMAKE_CURRENT_LIST_DIR}/x/codegen/ConstantDataSnippet.cpp
        ${CMAKE_CURRENT_LIST_DIR}/x/codegen/ControlFlowEvaluator.cpp
        ${CMAKE_CURRENT_LIST_DIR}/x/codegen/DataSnippet.cpp
        ${CMAKE_CURRENT_LIST_DIR}/x/codegen/DivideCheckSnippet.cpp
        ${CMAKE_CURRENT_LIST_DIR}/x/codegen/FPBinaryArithmeticAnalyser.cpp
        ${CMAKE_CURRENT_LIST_DIR}/x/codegen/FPCompareAnalyser.cpp
        ${CMAKE_CURRENT_LIST_DIR}/x/codegen/FPTreeEvaluator.cpp
        ${CMAKE_CURRENT_LIST_DIR}/x/codegen/SIMDTreeEvaluator.cpp
        ${CMAKE_CURRENT_LIST_DIR}/x/codegen/HelperCallSnippet.cpp
        ${CMAKE_CURRENT_LIST_DIR}/x/codegen/IA32LinkageUtils.cpp
        ${CMAKE_CURRENT_LIST_DIR}/x/codegen/IntegerMultiplyDecomposer.cpp
        ${CMAKE_CURRENT_LIST_DIR}/x/codegen/OMRMemoryReference.cpp
        ${CMAKE_CURRENT_LIST_DIR}/x/codegen/OpBinary.cpp
        ${CMAKE_CURRENT_LIST_DIR}/x/codegen/OpNames.cpp
        ${CMAKE_CURRENT_LIST_DIR}/x/codegen/OutlinedInstructions.cpp
        ${CMAKE_CURRENT_LIST_DIR}/x/codegen/RegisterRematerialization.cpp
        ${CMAKE_CURRENT_LIST_DIR}/x/codegen/SubtractAnalyser.cpp
        ${CMAKE_CURRENT_LIST_DIR}/x/codegen/Trampoline.cpp
        ${CMAKE_CURRENT_LIST_DIR}/x/codegen/OMRTreeEvaluator.cpp
        ${CMAKE_CURRENT_LIST_DIR}/x/codegen/UnaryEvaluator.cpp
        ${CMAKE_CURRENT_LIST_DIR}/x/codegen/X86BinaryEncoding.cpp
        ${CMAKE_CURRENT_LIST_DIR}/x/codegen/X86Debug.cpp
        ${CMAKE_CURRENT_LIST_DIR}/x/codegen/X86FPConversionSnippet.cpp
        ${CMAKE_CURRENT_LIST_DIR}/x/codegen/OMRInstruction.cpp
        ${CMAKE_CURRENT_LIST_DIR}/x/codegen/OMRX86Instruction.cpp
        ${CMAKE_CURRENT_LIST_DIR}/x/codegen/OMRMachine.cpp
        ${CMAKE_CURRENT_LIST_DIR}/x/codegen/OMRLinkage.cpp
        ${CMAKE_CURRENT_LIST_DIR}/x/codegen/OMRRegister.cpp
        ${CMAKE_CURRENT_LIST_DIR}/x/codegen/OMRRealRegister.cpp
        ${CMAKE_CURRENT_LIST_DIR}/x/codegen/OMRRegisterDependency.cpp
        ${CMAKE_CURRENT_LIST_DIR}/x/codegen/OMRSnippet.cpp
        ${CMAKE_CURRENT_LIST_DIR}/x/codegen/X86SystemLinkage.cpp
        ${CMAKE_CURRENT_LIST_DIR}/x/codegen/XMMBinaryArithmeticAnalyser.cpp
        ${CMAKE_CURRENT_LIST_DIR}/x/codegen/OMRCodeGenerator.cpp
        ${CMAKE_CURRENT_LIST_DIR}/x/codegen/OMRRegisterIterator.cpp
        ${CMAKE_CURRENT_LIST_DIR}/x/env/OMRDebugEnv.cpp
        ${CMAKE_CURRENT_LIST_DIR}/x/env/OMRCPU.cpp
    )

    if(OMR_COMPILER_TARGET_SUBARCHITECTURE STREQUAL "amd64")
        list(APPEND OMR_COMPILER_SOURCES
            ${CMAKE_CURRENT_LIST_DIR}/x/amd64/codegen/OMRCodeGenerator.cpp
            ${CMAKE_CURRENT_LIST_DIR}/x/amd64/codegen/OMRMachine.cpp
            ${CMAKE_CURRENT_LIST_DIR}/x/amd64/codegen/OMRMemoryReference.cpp
            ${CMAKE_CURRENT_LIST_DIR}/x/amd64/codegen/OMRRealRegister.cpp
            ${CMAKE_CURRENT_LIST_DIR}/x/amd64/codegen/OMRTreeEvaluator.cpp
            ${CMAKE_CURRENT_LIST_DIR}/x/amd64/codegen/AMD64SystemLinkage.cpp
        )
    elseif(OMR_COMPILER_TARGET_SUBARCHITECTURE STREQUAL "i386")
        list(APPEND OMR_COMPILER_SOURCES
            ${CMAKE_CURRENT_LIST_DIR}/x/i386/codegen/OMRCodeGenerator.cp
            ${CMAKE_CURRENT_LIST_DIR}/x/i386/codegen/OMRMachine.cpp
            ${CMAKE_CURRENT_LIST_DIR}/x/i386/codegen/OMRRealRegister.cpp
            ${CMAKE_CURRENT_LIST_DIR}/x/i386/codegen/OMRTreeEvaluator.cpp
            ${CMAKE_CURRENT_LIST_DIR}/x/i386/codegen/IA32SystemLinkage.cpp
        )
    endif()
endif()
