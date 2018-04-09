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
#include <stdint.h>
#include "JitBuilderAPI.hpp"

namespace JitBuilderCoreAPI
{

/* Semantic versioning for the API in this file */
const int32_t MajorVersion = 0;
const int32_t MinorVersion = 0;
const int32_t PatchVersion = 0;


Class(	/* .doc          = */   "",
	/* .name         = */   TypeDictionary,
	/* .shortName    = */   "TD",
	/* .flags        = */   IS_CREATABLE | HAS_EXTRAS_HEADER,
	/* .super        = */   NULL,

	Fields(TypeDictionary,
		Field("_impl",         T_pointer,            IS_PUBLIC )
		Field("NoType",        T_IlType,             IS_PUBLIC | ASSIGN_AT_INIT)
		Field("Int8",          T_IlType,             IS_PUBLIC | ASSIGN_AT_INIT)
		Field("Int16",         T_IlType,             IS_PUBLIC | ASSIGN_AT_INIT)
		Field("Int32",         T_IlType,             IS_PUBLIC | ASSIGN_AT_INIT)
		Field("Int64",         T_IlType,             IS_PUBLIC | ASSIGN_AT_INIT)
		Field("Float",         T_IlType,             IS_PUBLIC | ASSIGN_AT_INIT)
		Field("Double",        T_IlType,             IS_PUBLIC | ASSIGN_AT_INIT)
		Field("Address",       T_IlType,             IS_PUBLIC | ASSIGN_AT_INIT)
		Field("VectorInt8",    T_IlType,             IS_PUBLIC | ASSIGN_AT_INIT)
		Field("VectorInt16",   T_IlType,             IS_PUBLIC | ASSIGN_AT_INIT)
		Field("VectorInt32",   T_IlType,             IS_PUBLIC | ASSIGN_AT_INIT)
		Field("VectorInt64",   T_IlType,             IS_PUBLIC | ASSIGN_AT_INIT)
		Field("VectorFloat",   T_IlType,             IS_PUBLIC | ASSIGN_AT_INIT)
		Field("VectorDouble",  T_IlType,             IS_PUBLIC | ASSIGN_AT_INIT)
		Field("Word",          T_IlType,             IS_PUBLIC | ASSIGN_AT_INIT)
		Field("pNoType",       T_IlType,             IS_PUBLIC | ASSIGN_AT_INIT)
		Field("pInt8",         T_IlType,             IS_PUBLIC | ASSIGN_AT_INIT)
		Field("pInt16",        T_IlType,             IS_PUBLIC | ASSIGN_AT_INIT)
		Field("pInt32",        T_IlType,             IS_PUBLIC | ASSIGN_AT_INIT)
		Field("pInt64",        T_IlType,             IS_PUBLIC | ASSIGN_AT_INIT)
		Field("pFloat",        T_IlType,             IS_PUBLIC | ASSIGN_AT_INIT)
		Field("pDouble",       T_IlType,             IS_PUBLIC | ASSIGN_AT_INIT)
		Field("pAddress",      T_IlType,             IS_PUBLIC | ASSIGN_AT_INIT)
		Field("pVectorInt8",   T_IlType,             IS_PUBLIC | ASSIGN_AT_INIT)
		Field("pVectorInt16",  T_IlType,             IS_PUBLIC | ASSIGN_AT_INIT)
		Field("pVectorInt32",  T_IlType,             IS_PUBLIC | ASSIGN_AT_INIT)
		Field("pVectorInt64",  T_IlType,             IS_PUBLIC | ASSIGN_AT_INIT)
		Field("pVectorFloat",  T_IlType,             IS_PUBLIC | ASSIGN_AT_INIT)
		Field("pVectorDouble", T_IlType,             IS_PUBLIC | ASSIGN_AT_INIT)
		Field("pWord",         T_IlType,             IS_PUBLIC | ASSIGN_AT_INIT)
	)

	Functions(TypeDictionary,
		Function(/* .doc            = */"",
			/* .name           = */	CREATOR,
			/* .flags          = */	IS_PUBLIC | IS_STATIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	1,
			/* .parms          = */	Parms(	Parm("impl", T_pointer ) ) )
		Function(/* .doc            = */"",
			/* .name           = */	INITIALIZER,
			/* .flags          = */	IS_PROTECTED,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	1,
			/* .parms          = */	Parms(	Parm("impl", T_pointer ) ) )
		Function(/* .doc            = */"",
			/* .name           = */	CONSTRUCTOR,
			/* .flags          = */	IS_PUBLIC | CREATE_IMPL,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	0,
			/* .parms          = */	Parms( ) )
		Function(/* .doc            = */"",
			/* .name           = */	DESTRUCTOR,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	0,
			/* .parms          = */	Parms( ) )
		Function(/* .doc            = */"",
			/* .name           = */	CloseStruct,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	1,
			/* .parms          = */	Parms(	Parm("structName", T_constString) ) )
		Function(/* .doc            = */"",
			/* .name           = */	CloseStructWithSize,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"WithSize",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	2,
			/* .parms          = */	Parms(	Parm("structName", T_constString),
							Parm("size",       T_unsignedInteger) ) )
		Function(/* .doc            = */"",
			/* .name           = */	CloseUnion,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	1,
			/* .parms          = */	Parms(	Parm("unionName", T_constString) ) )
		Function(/* .doc            = */"",
			/* .name           = */	DefineField,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	3,
			/* .parms          = */	Parms(	Parm("structName", T_constString),
							Parm("fieldName",  T_constString),
							Parm("type",       T_IlType ) ) )
		Function(/* .doc            = */"",
			/* .name           = */	DefineFieldAtOffset,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"AtOffset",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	4,
			/* .parms          = */	Parms(	Parm("structName", T_constString),
							Parm("fieldName",  T_constString),
							Parm("type",       T_IlType),
							Parm("offset",     T_unsignedInteger ) ) )
		Function(/* .doc            = */"",
			/* .name           = */	DefineStruct,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_IlType,
			/* .numParameters  = */	1,
			/* .parms          = */	Parms(	Parm("structName", T_constString) ) )
		Function(/* .doc            = */"",
			/* .name           = */	DefineUnion,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_IlType,
			/* .numParameters  = */	1,
			/* .parms          = */	Parms(	Parm("unionName", T_constString) ) )
		Function(/* .doc            = */"",
			/* .name           = */	GetFieldType,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_IlType,
			/* .numParameters  = */	2,
			/* .parms          = */	Parms(	Parm("structName", T_constString),
							Parm("fieldName",  T_constString) ) )
		Function(/* .doc            = */"",
			/* .name           = */	LookupStruct,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_IlType,
			/* .numParameters  = */	1,
			/* .parms          = */	Parms(	Parm("structName", T_constString) ) )
		Function(/* .doc            = */"",
			/* .name           = */	LookupUnion,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_IlType,
			/* .numParameters  = */	1,
			/* .parms          = */	Parms(	Parm("unionName", T_constString) ) )
		Function(/* .doc            = */"",
			/* .name           = */	OffsetOf,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_unsignedInteger,
			/* .numParameters  = */	2,
			/* .parms          = */	Parms(	Parm("structName", T_constString),
							Parm("fieldName",  T_constString) ) )
		Function(/* .doc            = */"",
			/* .name           = */	PointerTo,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_IlType,
			/* .numParameters  = */	1,
			/* .parms          = */	Parms(	Parm("baseType", T_IlType) ) )
		Function(/* .doc            = */"",
			/* .name           = */	PointerToStructName,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"StructName",
			/* .returnType     = */	T_IlType,
			/* .numParameters  = */	1,
			/* .parms          = */	Parms(	Parm("StructName", T_constString) ) )
		Function(/* .doc            = */"",
			/* .name           = */	UnionField,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	3,
			/* .parms          = */	Parms(	Parm("unionName", T_constString),
							Parm("fieldName", T_constString),
							Parm("fieldType", T_IlType     ) ) )
		Function(/* .doc            = */"",
			/* .name           = */	UnionFieldType,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_IlType,
			/* .numParameters  = */	2,
			/* .parms          = */	Parms(	Parm("unionName", T_constString),
							Parm("fieldName", T_constString) ) )
	)
) // Class(...TypeDictionary..)

Class(	/* .doc          = */   "",
	/* .name         = */   IlBuilder,
	/* .shortName    = */   "IB",
	/* .flags        = */   0,
	/* .super        = */   NO_SUPER,

	Fields(IlBuilder,
		Field("NoType",		T_IlType,        IS_PUBLIC | ASSIGN_AT_INIT)
		Field("Int8",		T_IlType,        IS_PUBLIC | ASSIGN_AT_INIT)
		Field("Int16",		T_IlType,        IS_PUBLIC | ASSIGN_AT_INIT)
		Field("Int32",		T_IlType,        IS_PUBLIC | ASSIGN_AT_INIT)
		Field("Int64",		T_IlType,        IS_PUBLIC | ASSIGN_AT_INIT)
		Field("Float",		T_IlType,        IS_PUBLIC | ASSIGN_AT_INIT)
		Field("Double",		T_IlType,        IS_PUBLIC | ASSIGN_AT_INIT)
		Field("Address",	T_IlType,        IS_PUBLIC | ASSIGN_AT_INIT)
		Field("VectorInt8",	T_IlType,        IS_PUBLIC | ASSIGN_AT_INIT)
		Field("VectorInt16",	T_IlType,        IS_PUBLIC | ASSIGN_AT_INIT)
		Field("VectorInt32",	T_IlType,        IS_PUBLIC | ASSIGN_AT_INIT)
		Field("VectorInt64",	T_IlType,        IS_PUBLIC | ASSIGN_AT_INIT)
		Field("VectorFloat",	T_IlType,        IS_PUBLIC | ASSIGN_AT_INIT)
		Field("VectorDouble",	T_IlType,        IS_PUBLIC | ASSIGN_AT_INIT)
		Field("Word",		T_IlType,        IS_PUBLIC | ASSIGN_AT_INIT)
		Field("_impl",		T_pointer,       IS_PUBLIC                   )
	)

	Functions(IlBuilder,
		Function(/* .doc            = */"",
			/* .name           = */	CREATOR,
			/* .flags          = */	IS_PROTECTED,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	0,
			/* .parms          = */	Parms( ) )
		Function(/* .doc            = */"",
			/* .name           = */	CREATOR,
			/* .flags          = */	IS_PROTECTED | IS_STATIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	1,
			/* .parms          = */	Parms(	Parm("impl", T_pointer  ) ) )
		Function(/* .doc            = */"",
			/* .name           = */	INITIALIZER,
			/* .flags          = */	IS_PROTECTED,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	1,
			/* .parms          = */	Parms(	Parm("impl", T_pointer ) ) )
		Function(/* .doc            = */"",
			/* .name           = */	DESTRUCTOR,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	0,
			/* .parms          = */	Parms( ) )
		Function(/* .doc            = */"",
			/* .name           = */	buildIL,
			/* .flags          = */	IS_PUBLIC | IS_CALLBACK,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_boolean,
			/* .numParameters  = */	0,
			/* .parms          = */	Parms( ) )

		Function(/* .doc            = */"",
			/* .name           = */	OrphanBuilder,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_IlBuilder,
			/* .numParameters  = */	0,
			/* .parms          = */	Parms( ) )
		Function(/* .doc            = */"",
			/* .name           = */	OrphanBytecodeBuilder,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_BytecodeBuilder,
			/* .numParameters  = */	2,
			/* .parms          = */	Parms(	Parm("bcIndex", T_int32),
							Parm("name",    T_string) ) )
		Function(/* .doc            = */"",
			/* .name           = */	Copy,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_IlValue,
			/* .numParameters  = */	1,
			/* .parms          = */	Parms(	Parm("value", T_IlValue) ) )
		Function(/* .doc            = */"",
			/* .name           = */	typeDictionary,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_TypeDictionary,
			/* .numParameters  = */	0,
			/* .parms          = */	Parms( ) )
		Function(/* .doc            = */	"",
			/* .name           = */	ConstInteger,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_IlValue,
			/* .numParameters  = */	2,
			/* .parms          = */	Parms(	Parm("type",  T_IlType ),
							Parm("value", T_int64) ) )

/* Helper to create Const services */
#define CONST(doc, name, type)                                          \
	Function(/* .doc           = */ doc,                            \
		/* .name           = */	name,                           \
		/* .flags          = */	IS_PUBLIC,                      \
		/* .overloadSuffix = */	"",                             \
		/* .returnType     = */	T_IlValue,                      \
		/* .numParameters  = */	1,                              \
		/* .parms          = */	Parms(	Parm("value", type) ) )

		CONST("", ConstAddress, T_pointer)
		CONST("", ConstDouble, T_double)
		CONST("", ConstFloat, T_float)
		CONST("", ConstInt8, T_int8)
		CONST("", ConstInt16, T_int16)
		CONST("", ConstInt32, T_int32)
		CONST("", ConstInt64, T_int64)
		CONST("", ConstString, T_string)
		CONST("", Const, T_pointer)

		Function(/* .doc           = */ "",
			/* .name           = */	NullAddress,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_IlValue,
			/* .numParameters  = */	0,
			/* .parms          = */	Parms( ) )

/* Helper to create Binary arithmetic services */

#define BINARY(doc, name)                                                    \
	Function(/* .doc           = */ doc,                                 \
		/* .name           = */ name,                                \
		/* .flags          = */	IS_PUBLIC,                           \
		/* .overloadSuffix = */	"",                                  \
		/* .returnType     = */	T_IlValue,                           \
		/* .numParameters  = */	2,                                   \
		/* .parms          = */	Parms(	Parm("left", T_IlValue),     \
						Parm("right", T_IlValue) ) )
#define BINARYWITHOVERFLOW(doc, name)                                                      \
	Function(/* .doc           = */ doc,                                               \
		/* .name           = */ name,                                              \
		/* .flags          = */	IS_PUBLIC,                                         \
		/* .overloadSuffix = */	"",                                                \
		/* .returnType     = */	T_IlValue,                                         \
		/* .numParameters  = */	3,                                                 \
		/* .parms          = */	Parms(	Parm("overflowHandler", T_IlBuilderByRef), \
						Parm("left", T_IlValue),                   \
						Parm("right", T_IlValue) ) )

		BINARY("", Add)
		BINARYWITHOVERFLOW("", AddWithOverflow)
		BINARYWITHOVERFLOW("", AddWithUnsignedOverflow)
		BINARY("", And)
		BINARY("", Div)
		Function(/* .doc           = */ "",
			/* .name           = */	IndexAt,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_IlValue,
			/* .numParameters  = */	3,
			/* .parms          = */	Parms(	Parm("dt", T_IlType),
							Parm("base", T_IlValue),
							Parm("index", T_IlValue) ) )
		BINARY("", Mul)
		BINARYWITHOVERFLOW("", MulWithOverflow)
		BINARY("", Or)
		BINARY("", Rem)
		BINARY("", ShiftL)
		BINARY("", ShiftR)
		BINARY("", Sub)
		BINARYWITHOVERFLOW("", SubWithOverflow)
		BINARYWITHOVERFLOW("", SubWithUnsignedOverflow)
		BINARY("", UnsignedShiftR)
		BINARY("", Xor)

		/* Compare services */
		BINARY("", EqualTo)
		BINARY("", LessOrEqualTo)
		BINARY("", LessThan)
		BINARY("", GreaterOrEqualTo)
		BINARY("", GreaterThan)
		BINARY("", NotEqualTo)
		BINARY("", UnsignedLessOrEqualTo)
		BINARY("", UnsignedLessThan)
		BINARY("", UnsignedGreaterOrEqualTo)
		BINARY("", UnsignedGreaterThan)

/* Helper to create Convert services */
#define CONVERT(doc, name)                                                   \
	Function(/* .doc           = */	doc,                                 \
		/* .name           = */ name,                                \
		/* .flags          = */	IS_PUBLIC,                           \
		/* .overloadSuffix = */	"",                                  \
		/* .returnType     = */	T_IlValue,                           \
		/* .numParameters  = */	2,                                   \
		/* .parms          = */	Parms(	Parm("type", T_IlType),      \
						Parm("value", T_IlValue) ) )

		CONVERT("", ConvertTo)
		CONVERT("", UnsignedConvertTo)

		/* Memory services */
		Function(/* .doc           = */	"",
			/* .name           = */ AtomicAdd,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_IlValue,
			/* .numParameters  = */	2,
			/* .parms          = */	Parms(	Parm("baseAddress", T_IlValue),
							Parm("value", T_IlValue) ) )
		Function(/* .doc           = */	"",
			/* .name           = */ CreateLocalArray,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_IlValue,
			/* .numParameters  = */	2,
			/* .parms          = */	Parms(	Parm("numElements", T_int32),
							Parm("elementType", T_IlType) ) )
		Function(/* .doc           = */	"",
			/* .name           = */ CreateLocalStruct,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_IlValue,
			/* .numParameters  = */	1,
			/* .parms          = */	Parms(	Parm("structType", T_IlType) ) )
		Function(/* .doc           = */	"",
			/* .name           = */ Load,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_IlValue,
			/* .numParameters  = */	1,
			/* .parms          = */	Parms(	Parm("name", T_string) ) )
		Function(/* .doc           = */	"",
			/* .name           = */ LoadAt,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_IlValue,
			/* .numParameters  = */	2,
			/* .parms          = */	Parms(	Parm("type", T_IlType),
							Parm("address", T_IlValue) ) )
		Function(/* .doc           = */	"",
			/* .name           = */ LoadIndirect,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_IlValue,
			/* .numParameters  = */	3,
			/* .parms          = */	Parms(	Parm("type", T_string),
							Parm("field", T_string),
							Parm("object", T_IlValue) ) )
		Function(/* .doc           = */	"",
			/* .name           = */ Store,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	2,
			/* .parms          = */	Parms(	Parm("name",  T_string),
							Parm("value", T_IlValue) ) )
		Function(/* .doc           = */	"",
			/* .name           = */ StoreAt,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	2,
			/* .parms          = */	Parms(	Parm("address", T_IlValue),
							Parm("value", T_IlValue) ) )
		Function(/* .doc           = */	"",
			/* .name           = */ StoreIndirect,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	4,
			/* .parms          = */	Parms(	Parm("type", T_string),
							Parm("field", T_string),
							Parm("object", T_IlValue),
							Parm("value", T_IlValue) ) )
		Function(/* .doc           = */	"",
			/* .name           = */ StoreOver,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	2,
			/* .parms          = */	Parms(	Parm("dest", T_IlValue),
							Parm("value", T_IlValue) ) )
		Function(/* .doc           = */	"",
			/* .name           = */ Transaction,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	3,
			/* .parms          = */	Parms(	Parm("persistentFailureBuilder", T_IlBuilderByRef),
							Parm("transientFailureBuilder", T_IlBuilderByRef),
							Parm("transactionBuilder", T_IlBuilderByRef) ) )
		Function(/* .doc           = */	"",
			/* .name           = */ TransactionAbort,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	0,
			/* .parms          = */	Parms( ) )
		Function(/* .doc           = */	"",
			/* .name           = */ StructFieldInstanceAddress,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_IlValue,
			/* .numParameters  = */	3,
			/* .parms          = */	Parms(	Parm("structName", T_string),
							Parm("fieldName",  T_string),
							Parm("obj",        T_IlValue) ))
		Function(/* .doc           = */	"",
			/* .name           = */ UnionFieldInstanceAddress,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_IlValue,
			/* .numParameters  = */	3,
			/* .parms          = */	Parms(	Parm("unionName", T_string),
							Parm("fieldName", T_string),
							Parm("obj",       T_IlValue) ))

		/* Vector memory services */
		Function(/* .doc           = */	"",
			/* .name           = */ VectorLoad,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_IlValue,
			/* .numParameters  = */	1,
			/* .parms          = */	Parms(	Parm("name", T_string) ) )
		Function(/* .doc           = */	"",
			/* .name           = */ VectorLoadAt,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_IlValue,
			/* .numParameters  = */	2,
			/* .parms          = */	Parms(	Parm("type", T_IlType),
							Parm("address", T_IlValue) ) )
		Function(/* .doc           = */	"",
			/* .name           = */ VectorStore,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	2,
			/* .parms          = */	Parms(	Parm("name",  T_string),
							Parm("value", T_IlValue) ) )
		Function(/* .doc           = */	"",
			/* .name           = */ VectorStoreAt,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	2,
			/* .parms          = */	Parms(	Parm("address", T_IlValue),
							Parm("value", T_IlValue) ) )

		/* Control flow services */
		Function(/* .doc           = */	"",
			/* .name           = */ AppendBuilder,
			/* .flags          = */	IS_PUBLIC | IS_OVERIDDEN,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	1,
			/* .parms          = */	Parms(	Parm("b", T_IlBuilder) ) )
		Function(/* .doc           = */	"",
			/* .name           = */ Call,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_IlValue,
			/* .numParameters  = */	4,
			/* .parms          = */	Parms(	Parm("name",      T_string),
							Parm("numArgs",   T_int32),
							Parm("...",       T_vararg),
							Parm("arguments", T_IlValue) ) )
		Function(/* .doc           = */	"",
			/* .name           = */ CallWithArgArray,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"WithArgArray",
			/* .returnType     = */	T_IlValue,
			/* .numParameters  = */	3,
			/* .parms          = */	Parms(	Parm("name",      T_string),
							Parm("numArgs",   T_int32),
							Parm("arguments", T_IlValueArray) ) )
		Function(/* .doc           = */	"",
			/* .name           = */ ComputedCall,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_IlValue,
			/* .numParameters  = */	4,
			/* .parms          = */	Parms(	Parm("name",      T_string),
							Parm("numArgs",   T_int32),
							Parm("...",       T_vararg),
							Parm("arguments", T_IlValue) ) )
		Function(/* .doc           = */	"",
			/* .name           = */ ComputedCallWithArgArray,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"WithArgArray",
			/* .returnType     = */	T_IlValue,
			/* .numParameters  = */	3,
			/* .parms          = */	Parms(	Parm("name",      T_string),
							Parm("numArgs",   T_int32),
							Parm("arguments", T_IlValueArray) ) )
		Function(/* .doc           = */	"",
			/* .name           = */ DoWhileLoop,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	2,
			/* .parms          = */	Parms(	Parm("exitCondition", T_string),
							Parm("body", T_IlBuilderByRef) ) )
		Function(/* .doc           = */	"",
			/* .name           = */ DoWhileLoopWithBreakAndContinue,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"WithBreakAndContinue",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	4,
			/* .parms          = */	Parms(	Parm("exitCondition", T_string),
							Parm("body", T_IlBuilderByRef),
							Parm("breakBuilder", T_IlBuilderByRef),
							Parm("continueBuilder", T_IlBuilderByRef) ) )
		Function(/* .doc           = */	"",
			/* .name           = */ DoWhileLoopWithBreak,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	3,
			/* .parms          = */	Parms(	Parm("exitCondition", T_string),
							Parm("body", T_IlBuilderByRef),
							Parm("breakBuilder", T_IlBuilderByRef) ) )
		Function(/* .doc           = */	"",
			/* .name           = */ DoWhileLoopWithContinue,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	3,
			/* .parms          = */	Parms(	Parm("exitCondition", T_string),
							Parm("body", T_IlBuilderByRef),
							Parm("continueBuilder", T_IlBuilderByRef) ) )
		Function(/* .doc           = */	"",
			/* .name           = */ Goto,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	1,
			/* .parms          = */	Parms(	Parm("b", T_IlBuilder) ) )
		Function(/* .doc           = */	"",
			/* .name           = */ GotoNew,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"New",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	1,
			/* .parms          = */	Parms(	Parm("b", T_IlBuilderByRef) ) )
		Function(/* .doc           = */	"",
			/* .name           = */ IfAnd,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	6,
			/* .parms          = */	Parms(	Parm("allTrueBuilder",  T_IlBuilderByRef),
							Parm("anyFalseBuilder", T_IlBuilderByRef),
							Parm("numTerms",        T_int32),
							Parm("...",             T_vararg),
							Parm("termBuilders",    T_IlBuilder),
							Parm("termValues",      T_IlValue) ) )

/* Helper to create IlBuilder Conditional services */
#define ILBUILDER_CONDITIONAL(doc, name)                                          \
	Function(/* .doc           = */	doc,                                      \
		/* .name           = */ name,                                     \
		/* .flags          = */	IS_PUBLIC,                                \
		/* .overloadSuffix = */	"",                                       \
		/* .returnType     = */	T_none,                                   \
		/* .numParameters  = */	3,                                        \
		/* .parms          = */	Parms(	Parm("target", T_IlBuilder),      \
						Parm("left", T_IlValue),          \
						Parm("right", T_IlValue) ) )      \
	Function(/* .doc           = */	doc,                                      \
		/* .name           = */ name##New,                                \
		/* .flags          = */	IS_PUBLIC,                                \
		/* .overloadSuffix = */	"New",                                    \
		/* .returnType     = */	T_none,                                   \
		/* .numParameters  = */	3,                                        \
		/* .parms          = */	Parms(	Parm("target", T_IlBuilderByRef), \
						Parm("left", T_IlValue),          \
						Parm("right", T_IlValue) ) )

		ILBUILDER_CONDITIONAL("", IfCmpEqual)
		ILBUILDER_CONDITIONAL("", IfCmpLessOrEqual)
		ILBUILDER_CONDITIONAL("", IfCmpLessThan)
		ILBUILDER_CONDITIONAL("", IfCmpGreaterOrEqual)
		ILBUILDER_CONDITIONAL("", IfCmpGreaterThan)
		ILBUILDER_CONDITIONAL("", IfCmpNotEqual)
		ILBUILDER_CONDITIONAL("", IfCmpUnsignedLessOrEqual)
		ILBUILDER_CONDITIONAL("", IfCmpUnsignedLessThan)
		ILBUILDER_CONDITIONAL("", IfCmpUnsignedGreaterOrEqual)
		ILBUILDER_CONDITIONAL("", IfCmpUnsignedGreaterThan)

		Function(/* .doc           = */	"",
			/* .name           = */ IfCmpEqualZero,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	2,
			/* .parms          = */	Parms(	Parm("target", T_IlBuilder),
							Parm("condition", T_IlValue) ) )
		Function(/* .doc           = */	"",
			/* .name           = */ IfCmpEqualZeroNew,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"New",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	2,
			/* .parms          = */	Parms(	Parm("target", T_IlBuilderByRef),
							Parm("condition", T_IlValue) ) )
		Function(/* .doc           = */	"",
			/* .name           = */ IfCmpNotEqualZero,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	2,
			/* .parms          = */	Parms(	Parm("target", T_IlBuilder),
							Parm("condition", T_IlValue) ) )
		Function(/* .doc           = */	"",
			/* .name           = */ IfCmpNotEqualZeroNew,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"New",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	2,
			/* .parms          = */	Parms(	Parm("target", T_IlBuilderByRef),
							Parm("condition", T_IlValue) ) )

		Function(/* .doc           = */	"",
			/* .name           = */ IfOr,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	6,
			/* .parms          = */	Parms(	Parm("anyTrueBuilder",  T_IlBuilderByRef),
							Parm("allFalseBuilder", T_IlBuilderByRef),
							Parm("numTerms",        T_int32),
							Parm("...",             T_vararg),
							Parm("termBuilders",    T_IlBuilder),
							Parm("termValues",      T_IlValue) ) )
		Function(/* .doc           = */	"",
			/* .name           = */ IfThen,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	2,
			/* .parms          = */	Parms(	Parm("thenPath", T_IlBuilderByRef),
							Parm("condition", T_IlValue) ) )
		Function(/* .doc           = */	"",
			/* .name           = */ IfThenElse,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	3,
			/* .parms          = */	Parms(	Parm("thenPath", T_IlBuilderByRef),
							Parm("elsePath", T_IlBuilderByRef),
							Parm("condition", T_IlValue) ) )
		Function(/* .doc           = */	"",
			/* .name           = */ ForLoop,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	8,
			/* .parms          = */	Parms(	Parm("countsUp", T_boolean),
							Parm("indVar", T_string),
							Parm("body", T_IlBuilderByRef),
							Parm("breakBuilder", T_IlBuilderByRef),
							Parm("continueBuilder", T_IlBuilderByRef),
							Parm("initial", T_IlValue),
							Parm("iterateWhile", T_IlValue),
							Parm("increment", T_IlValue) ) )
		Function(/* .doc           = */	"",
			/* .name           = */ ForLoopDown,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	5,
			/* .parms          = */	Parms(	Parm("indVar", T_string),
							Parm("body", T_IlBuilderByRef),
							Parm("initial", T_IlValue),
							Parm("iterateWhile", T_IlValue),
							Parm("increment", T_IlValue) ) )
		Function(/* .doc           = */	"",
			/* .name           = */ ForLoopUp,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	5,
			/* .parms          = */	Parms(	Parm("indVar", T_string),
							Parm("body", T_IlBuilderByRef),
							Parm("initial", T_IlValue),
							Parm("iterateWhile", T_IlValue),
							Parm("increment", T_IlValue) ) )
		Function(/* .doc           = */	"",
			/* .name           = */ ForLoopWithBreak,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	7,
			/* .parms          = */	Parms(	Parm("countsUp", T_boolean),
							Parm("indVar", T_string),
							Parm("body", T_IlBuilderByRef),
							Parm("breakBuilder", T_IlBuilderByRef),
							Parm("initial", T_IlValue),
							Parm("iterateWhile", T_IlValue),
							Parm("increment", T_IlValue) ) )
		Function(/* .doc           = */	"",
			/* .name           = */ ForLoopWithContinue,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	7,
			/* .parms          = */	Parms(	Parm("countsUp", T_boolean),
							Parm("indVar", T_string),
							Parm("body", T_IlBuilderByRef),
							Parm("continueBuilder", T_IlBuilderByRef),
							Parm("initial", T_IlValue),
							Parm("iterateWhile", T_IlValue),
							Parm("increment", T_IlValue) ) )
		Function(/* .doc           = */	"",
			/* .name           = */ Return,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	0,
			/* .parms          = */	Parms( ) )
		Function(/* .doc           = */	"",
			/* .name           = */ ReturnValue,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"Value",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	1,
			/* .parms          = */	Parms(	Parm("value", T_IlValue) ) )
		Function(/* .doc           = */	"",
			/* .name           = */ Switch,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	6,
			/* .parms          = */	Parms(	Parm("selectionVar",     T_string),
							Parm("defaultBuilder",   T_IlBuilderByRef),
							Parm("numCases",         T_unsignedInteger),
							Parm("caseValues",       T_int32Array),
							Parm("caseBuilders",     T_IlBuilderArray),
							Parm("caseFallsThrough", T_int32Array) ) )
		Function(/* .doc           = */	"",
			/* .name           = */ SwitchWithCases,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"WithCases",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	7,
			/* .parms          = */	Parms(	Parm("selectionVar",     T_string),
							Parm("defaultBuilder",   T_IlBuilderByRef),
							Parm("numCases",         T_unsignedInteger),
							Parm("...",              T_vararg),
							Parm("caseValues",       T_int32),
							Parm("caseBuilders",     T_IlBuilderByRef),
							Parm("caseFallsThrough", T_int32) ) )
		Function(/* .doc           = */	"",
			/* .name           = */ WhileDoLoop,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	2,
			/* .parms          = */	Parms(	Parm("exitCondition", T_string),
							Parm("body", T_IlBuilderByRef) ) )
		Function(/* .doc           = */	"",
			/* .name           = */ WhileDoLoopWithBreakAndContinue,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"WithBreakAndContinue",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	4,
			/* .parms          = */	Parms(	Parm("exitCondition", T_string),
							Parm("body", T_IlBuilderByRef),
							Parm("breakBuilder", T_IlBuilderByRef),
							Parm("continueBuilder", T_IlBuilderByRef) ) )
		Function(/* .doc           = */	"",
			/* .name           = */ WhileDoLoopWithBreak,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	3,
			/* .parms          = */	Parms(	Parm("exitCondition", T_string),
							Parm("body", T_IlBuilderByRef),
							Parm("breakBuilder", T_IlBuilderByRef) ) )
		Function(/* .doc           = */	"",
			/* .name           = */ WhileDoLoopWithContinue,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	3,
			/* .parms          = */	Parms(	Parm("exitCondition", T_string),
							Parm("body", T_IlBuilderByRef),
							Parm("continueBuilder", T_IlBuilderByRef) ) )
	)

) // Class(...IlBuilder...)

Class(	/* .doc          = */   "",
	/* .name         = */   BytecodeBuilder,
	/* .shortName    = */   "BB",
	/* .flags        = */   0,
	/* .super        = */   pClassName(IlBuilder),

	Fields(BytecodeBuilder, /* No fields to declare */ )

	Functions(BytecodeBuilder,
		Function(/* .doc            = */"",
			/* .name           = */	CREATOR,
			/* .flags          = */	IS_PROTECTED,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	0,
			/* .parms          = */	Parms( ) )
		Function(/* .doc            = */"",
			/* .name           = */	CREATOR,
			/* .flags          = */	IS_PROTECTED | IS_STATIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	1,
			/* .parms          = */	Parms(	Parm("impl", T_pointer       ) ) )
		Function(/* .doc            = */"",
			/* .name           = */	INITIALIZER,
			/* .flags          = */	IS_PROTECTED,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	1,
			/* .parms          = */	Parms(	Parm("impl", T_pointer ) ) )
		Function(/* .doc            = */"",
			/* .name           = */	DESTRUCTOR,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	0,
			/* .parms          = */	Parms( ) )
		Function(/* .doc            = */"",
			/* .name           = */	bcIndex,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_int32,
			/* .numParameters  = */	0,
			/* .parms          = */	Parms( ) )
		Function(/* .doc            = */"",
			/* .name           = */	name,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_string,
			/* .numParameters  = */	0,
			/* .parms          = */	Parms( ) )
		Function(/* .doc            = */"",
			/* .name           = */	vmState,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_VirtualMachineState,
			/* .numParameters  = */	0,
			/* .parms          = */	Parms( ) )
		Function(/* .doc            = */"",
			/* .name           = */	AddFallThroughBuilder,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	1,
			/* .parms          = */	Parms(	Parm("ftb", T_BytecodeBuilder) ) )
		Function(/* .doc            = */"",
			/* .name           = */	AddSuccessorBuilders,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	3,
			/* .parms          = */	Parms(	Parm("numBuilders", T_uint32),
							Parm("...",         T_vararg),
							Parm("builders",    T_BytecodeBuilder) ) )
		Function(/* .doc            = */"",
			/* .name           = */	AddSuccessorBuilder,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	1,
			/* .parms          = */	Parms(	Parm("b", T_BytecodeBuilderByRef) ) )
		Function(/* .doc           = */	"",
			/* .name           = */ Goto,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	1,
			/* .parms          = */	Parms(	Parm("b", T_BytecodeBuilder) ) )
		Function(/* .doc           = */	"",
			/* .name           = */ GotoNew,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"New",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	1,
			/* .parms          = */	Parms(	Parm("b", T_BytecodeBuilderByRef) ) )

/* Helper to create BytecodeBuilder Conditional services */
#define BYTECODEBUILDER_CONDITIONAL(doc, name)                                          \
	Function(/* .doc           = */	doc,                                            \
		/* .name           = */ name,                                           \
		/* .flags          = */	IS_PUBLIC,                                      \
		/* .overloadSuffix = */	"",                                             \
		/* .returnType     = */	T_none,                                         \
		/* .numParameters  = */	3,                                              \
		/* .parms          = */	Parms(	Parm("target", T_BytecodeBuilder),      \
						Parm("left", T_IlValue),                \
						Parm("right", T_IlValue) ) )            \
	Function(/* .doc           = */	doc,                                            \
		/* .name           = */ name##New,                                      \
		/* .flags          = */	IS_PUBLIC,                                      \
		/* .overloadSuffix = */	"New",                                          \
		/* .returnType     = */	T_none,                                         \
		/* .numParameters  = */	3,                                              \
		/* .parms          = */	Parms(	Parm("target", T_BytecodeBuilderByRef), \
						Parm("left", T_IlValue),                \
						Parm("right", T_IlValue) ) )

		BYTECODEBUILDER_CONDITIONAL("", IfCmpEqual)
		BYTECODEBUILDER_CONDITIONAL("", IfCmpLessOrEqual)
		BYTECODEBUILDER_CONDITIONAL("", IfCmpLessThan)
		BYTECODEBUILDER_CONDITIONAL("", IfCmpGreaterOrEqual)
		BYTECODEBUILDER_CONDITIONAL("", IfCmpGreaterThan)
		BYTECODEBUILDER_CONDITIONAL("", IfCmpNotEqual)
		BYTECODEBUILDER_CONDITIONAL("", IfCmpUnsignedLessOrEqual)
		BYTECODEBUILDER_CONDITIONAL("", IfCmpUnsignedLessThan)
		BYTECODEBUILDER_CONDITIONAL("", IfCmpUnsignedGreaterOrEqual)
		BYTECODEBUILDER_CONDITIONAL("", IfCmpUnsignedGreaterThan)

		Function(/* .doc           = */	"",
			/* .name           = */ IfCmpEqualZero,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	2,
			/* .parms          = */	Parms(	Parm("target", T_BytecodeBuilder),
							Parm("condition", T_IlValue) ) )
		Function(/* .doc           = */	"",
			/* .name           = */ IfCmpEqualZeroNew,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"New",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	2,
		/* .parms          = */	Parms(	Parm("target", T_BytecodeBuilderByRef),
						Parm("condition", T_IlValue) ) )
		Function(/* .doc           = */	"",
			/* .name           = */ IfCmpNotEqualZero,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	2,
			/* .parms          = */	Parms(	Parm("target", T_BytecodeBuilder),
							Parm("condition", T_IlValue) ) )
		Function(/* .doc           = */	"",
			/* .name           = */ IfCmpNotEqualZeroNew,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"New",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	2,
			/* .parms          = */	Parms(	Parm("target", T_BytecodeBuilderByRef),
							Parm("condition", T_IlValue) ) )
	)
) // Class(...BytecodeBuilder...)

Class(	/* .doc          = */   "",
	/* .name         = */   IlReference,
	/* .shortName    = */   "IR",
	/* .flags        = */   0,
	/* .super        = */   NULL,

	Fields(IlReference,
		Field("_impl", T_pointer, IS_PUBLIC )
	)

	Functions(IlReference,
		Function(/* .doc            = */"",
			/* .name           = */	CREATOR,
			/* .flags          = */	IS_PROTECTED,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	0,
			/* .parms          = */	Parms( ) )
		Function(/* .doc            = */"",
			/* .name           = */	CREATOR,
			/* .flags          = */	IS_PROTECTED,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	1,
			/* .parms          = */	Parms( Parm("impl", T_pointer) ) )
		Function(/* .doc            = */"",
			/* .name           = */	INITIALIZER,
			/* .flags          = */	IS_PROTECTED,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	1,
			/* .parms          = */	Parms(	Parm("impl", T_pointer ) ) )
		Function(/* .doc            = */"",
			/* .name           = */	DESTRUCTOR,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	0,
			/* .parms          = */	Parms( ) )
	)
) // Class(...IlReference...)

Class(	/* .doc          = */   "",
	/* .name         = */   IlType,
	/* .shortName    = */   "IT",
	/* .flags        = */   0,
	/* .super        = */   NULL,

	Fields(IlType,
		Field("_impl", T_pointer, IS_PUBLIC )
	)

	Functions(IlType,
		Function(/* .doc            = */"",
			/* .name           = */	CREATOR,
			/* .flags          = */	IS_PROTECTED,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	0,
			/* .parms          = */	Parms( ) )
		Function(/* .doc            = */"",
			/* .name           = */	CREATOR,
			/* .flags          = */	IS_PROTECTED | IS_STATIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	1,
			/* .parms          = */	Parms( Parm("impl", T_pointer ) ) )
		Function(/* .doc            = */"",
			/* .name           = */	INITIALIZER,
			/* .flags          = */	IS_PROTECTED,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	1,
			/* .parms          = */	Parms(	Parm("impl", T_pointer ) ) )
		Function(/* .doc            = */"",
			/* .name           = */	DESTRUCTOR,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	0,
			/* .parms          = */	Parms( ) )
		Function(/* .doc            = */"",
			/* .name           = */	baseType,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_IlType,
			/* .numParameters  = */	0,
			/* .parms          = */	Parms( ) )
		Function(/* .doc            = */"",
			/* .name           = */	getName,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_constString,
			/* .numParameters  = */	0,
			/* .parms          = */	Parms( ) )
		Function(/* .doc            = */"",
			/* .name           = */	primitiveType,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_IlType,
			/* .numParameters  = */	1,
			/* .parms          = */	Parms( Parm("d", T_TypeDictionary ) ) )
		Function(/* .doc            = */"",
			/* .name           = */	getSize,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_unsignedInteger,
			/* .numParameters  = */	0,
			/* .parms          = */	Parms( ) )
		Function(/* .doc            = */"",
			/* .name           = */	isPointer,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_boolean,
			/* .numParameters  = */	0,
			/* .parms          = */	Parms( ) )
	)
) // Class(...IlType...)

Class(	/* .doc          = */   "",
	/* .name         = */   IlValue,
	/* .shortName    = */   "IV",
	/* .flags        = */   0,
	/* .super        = */   NULL,

	Fields(IlValue,
		Field("_impl", T_pointer, IS_PUBLIC )
	)

	Functions(IlValue,
		Function(/* .doc            = */"",
			/* .name           = */	CREATOR,
			/* .flags          = */	IS_PROTECTED,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	0,
			/* .parms          = */	Parms( ) )
		Function(/* .doc            = */"",
			/* .name           = */	CREATOR,
			/* .flags          = */	IS_PROTECTED | IS_STATIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	1,
			/* .parms          = */	Parms( Parm("impl", T_pointer) ) )
		Function(/* .doc            = */"",
			/* .name           = */	INITIALIZER,
			/* .flags          = */	IS_PROTECTED,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	1,
			/* .parms          = */	Parms(	Parm("impl", T_pointer ) ) )
		Function(/* .doc            = */"",
			/* .name           = */	DESTRUCTOR,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	0,
			/* .parms          = */	Parms( ) )
		Function(/* .doc            = */"",
			/* .name           = */	getID,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_int32,
			/* .numParameters  = */	0,
			/* .parms          = */	Parms( ) )
	)
) // Class(...IlValue...)

Class(	/* .doc          = */   "",
	/* .name         = */   MethodBuilder,
	/* .shortName    = */   "MB",
	/* .flags        = */   IS_CREATABLE,
	/* .super        = */   pClassName(IlBuilder),

	Fields(MethodBuilder, /* No fields to declare */ )

	Functions(MethodBuilder,
		Function(/* .doc            = */"",
			/* .name           = */	CREATOR,
			/* .flags          = */	IS_PROTECTED,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	0,
			/* .parms          = */	Parms( ) )
		Function(/* .doc            = */"",
			/* .name           = */	CREATOR,
			/* .flags          = */	IS_PUBLIC | IS_STATIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	1,
			/* .parms          = */	Parms(	Parm("impl",    T_pointer) ) )
		Function(/* .doc            = */"",
			/* .name           = */	INITIALIZER,
			/* .flags          = */	IS_PROTECTED,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	1,
			/* .parms          = */	Parms(	Parm("impl", T_pointer ) ) )
		Function(/* .doc            = */"",
			/* .name           = */	CONSTRUCTOR,
			/* .flags          = */	IS_PUBLIC | CREATE_IMPL,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	1,
			/* .parms          = */	Parms(	Parm("dict",    T_TypeDictionary) ) )
		Function(/* .doc            = */"",
			/* .name           = */	CONSTRUCTOR,
			/* .flags          = */	IS_PUBLIC | CREATE_IMPL,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	2,
			/* .parms          = */	Parms(	Parm("dict",    T_TypeDictionary      ),
			/* .parms          = */	      	Parm("vmState", T_VirtualMachineState ) ) )
		Function(/* .doc            = */"",
			/* .name           = */	DESTRUCTOR,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	0,
			/* .parms          = */	Parms( ) )

		Function(/* .doc            = */"",
			/* .name           = */	RequestFunction,
			/* .flags          = */	IS_PUBLIC | IS_CALLBACK,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_boolean,
			/* .numParameters  = */	1,
			/* .parms          = */	Parms(	Parm("name", T_constString) ) )

		Function(/* .doc            = */"",
			/* .name           = */	AllLocalsHaveBeenDefined,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	0,
			/* .parms          = */	Parms( ) )
		Function(/* .doc            = */"",
			/* .name           = */	AppendBuilder,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	1,
			/* .parms          = */	Parms(	Parm("b", T_IlBuilder) ) )
		Function(/* .doc            = */"",
			/* .name           = */	AppendBytecodeBuilder,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	1,
			/* .parms          = */	Parms(	Parm("b", T_BytecodeBuilder) ) )
		Function(/* .doc            = */"",
			/* .name           = */	DefineFile,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	1,
			/* .parms          = */	Parms(	Parm("fileName", T_constString) ) )
		Function(/* .doc            = */"",
			/* .name           = */	DefineLineString,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"String",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	1,
			/* .parms          = */	Parms(	Parm("line", T_constString) ) )
		Function(/* .doc            = */"",
			/* .name           = */	DefineLineInteger,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"Integer",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	1,
			/* .parms          = */	Parms(	Parm("line", T_int32) ) )
		Function(/* .doc            = */"",
			/* .name           = */	DefineName,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	1,
			/* .parms          = */	Parms(	Parm("name", T_constString) ) )
		Function(/* .doc            = */"",
			/* .name           = */	DefineParameter,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	2,
			/* .parms          = */	Parms(	Parm("name", T_constString),
							Parm("type", T_IlType     ) ) )
		Function(/* .doc            = */"",
			/* .name           = */	DefineArrayParameter,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	2,
			/* .parms          = */	Parms(	Parm("name", T_constString),
							Parm("type", T_IlType     ) ) )
		Function(/* .doc            = */"",
			/* .name           = */	DefineReturnType,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	1,
			/* .parms          = */	Parms(	Parm("type", T_IlType) ) )
		Function(/* .doc            = */"",
			/* .name           = */	DefineLocal,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	2,
			/* .parms          = */	Parms(	Parm("name", T_constString),
							Parm("type", T_IlType     ) ) )
		Function(/* .doc            = */"",
			/* .name           = */	DefineMemory,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	3,
			/* .parms          = */	Parms(	Parm("name",     T_constString),
							Parm("type",     T_IlType     ),
							Parm("location", T_pointer    ) ) )
		Function(/* .doc            = */"",
			/* .name           = */	DefineFunctionVarArg,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"VarArg",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	8,
			/* .parms          = */	Parms(	Parm("name",       T_constString),
							Parm("fileName",   T_constString),
							Parm("lineNumber", T_constString),
							Parm("entryPoint", T_pointer    ),
							Parm("returnType", T_IlType     ),
							Parm("numParms",   T_int32      ),
							Parm("...",        T_vararg     ),
							Parm("parmType",   T_IlType     ) ) )
		Function(/* .doc            = */"",
			/* .name           = */	DefineFunction,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	7,
			/* .parms          = */	Parms(	Parm("name",       T_constString),
							Parm("fileName",   T_constString),
							Parm("lineNumber", T_constString),
							Parm("entryPoint", T_pointer    ),
							Parm("returnType", T_IlType     ),
							Parm("numParms",   T_int32      ),
							Parm("parmTypes",  T_IlTypeArray ) ) )
		Function(/* .doc            = */"",
			/* .name           = */	GetNextBytecodeFromWorklist,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_int32,
			/* .numParameters  = */	0,
			/* .parms          = */	Parms( ) )
		Function(/* .doc            = */"",
			/* .name           = */	setVMState,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	1,
			/* .parms          = */	Parms( Parm("vmState", T_VirtualMachineState) ) )
	)
) // Class(...MethodBuilder...)

Class(	/* .doc          = */   "",
	/* .name         = */   ThunkBuilder,
	/* .shortName    = */   "TB",
	/* .flags        = */   IS_CREATABLE,
	/* .super        = */   pClassName(MethodBuilder),

	Fields(ThunkBuilder,
		Field("_impl", T_pointer, IS_PUBLIC )
	)

	Functions(ThunkBuilder,
		Function(/* .doc            = */"",
			/* .name           = */	CREATOR,
			/* .flags          = */	IS_PROTECTED,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	0,
			/* .parms          = */	Parms( ) )
		Function(/* .doc            = */"",
			/* .name           = */	CREATOR,
			/* .flags          = */	IS_PROTECTED | IS_STATIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	1,
			/* .parms          = */	Parms(	Parm("impl", T_pointer) ) )
		Function(/* .doc            = */"",
			/* .name           = */	INITIALIZER,
			/* .flags          = */	IS_PROTECTED,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	1,
			/* .parms          = */	Parms(	Parm("impl", T_pointer ) ) )
		Function(/* .doc            = */"",
			/* .name           = */	CONSTRUCTOR,
			/* .flags          = */	IS_PUBLIC | CREATE_IMPL,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	5,
			/* .parms          = */	Parms(	Parm("dict", T_TypeDictionary),
							Parm("name", T_constString),
							Parm("returnType", T_IlType),
							Parm("numCalleeParms", T_uint32),
							Parm("calleeParms", T_IlTypeArray) ) )
		Function(/* .doc            = */"",
			/* .name           = */	DESTRUCTOR,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	0,
			/* .parms          = */	Parms( ) )
	)
)

Class(	/* .doc          = */   "",
	/* .name         = */   VirtualMachineState,
	/* .shortName    = */   "VMS",
	/* .flags        = */   IS_CREATABLE,
	/* .super        = */   NULL,

	Fields(VirtualMachineState,
		Field("_impl", T_pointer, IS_PUBLIC )
	)

	Functions(VirtualMachineState,
		Function(/* .doc            = */"",
			/* .name           = */	CREATOR,
			/* .flags          = */	IS_PROTECTED | IS_STATIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	1,
			/* .parms          = */	Parms(	Parm("impl", T_pointer) ) )
		Function(/* .doc            = */"",
			/* .name           = */	INITIALIZER,
			/* .flags          = */	IS_PROTECTED,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	1,
			/* .parms          = */	Parms(	Parm("impl", T_pointer ) ) )
		Function(/* .doc            = */"",
			/* .name           = */	CONSTRUCTOR,
			/* .flags          = */	IS_PUBLIC | CREATE_IMPL,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	0,
			/* .parms          = */	Parms( ) )
		Function(/* .doc            = */"",
			/* .name           = */	DESTRUCTOR,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	0,
			/* .parms          = */	Parms( ) )
		Function(/* .doc            = */"",
			/* .name           = */	Commit,
			/* .flags          = */	IS_PUBLIC | IS_CALLBACK | IS_VIRTUAL | IS_OVERIDDEN,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	1,
			/* .parms          = */	Parms(	Parm("b", T_IlBuilder ) ) )
		Function(/* .doc            = */"",
			/* .name           = */	Reload,
			/* .flags          = */	IS_PUBLIC | IS_CALLBACK | IS_VIRTUAL | IS_OVERIDDEN,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	1,
			/* .parms          = */	Parms(	Parm("b", T_IlBuilder ) ) )
		Function(/* .doc            = */"",
			/* .name           = */	MakeCopy,
			/* .flags          = */	IS_PUBLIC | IS_CALLBACK | IS_VIRTUAL | IS_OVERIDDEN,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_VirtualMachineState,
			/* .numParameters  = */	0,
			/* .parms          = */	Parms( ) ) 
		Function(/* .doc            = */"",
			/* .name           = */	MergeInto,
			/* .flags          = */	IS_PUBLIC | IS_CALLBACK | IS_VIRTUAL | IS_OVERIDDEN,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	2,
			/* .parms          = */	Parms(	Parm("vmState", T_VirtualMachineState ),
							Parm("b", T_IlBuilder                 ) ) )
	)
) // Class(...VirtualMachineState...)

Class(	/* .doc          = */   "",
	/* .name         = */   VirtualMachineRegister,
	/* .shortName    = */   "VMR",
	/* .flags        = */   IS_CREATABLE,
	/* .super        = */   pClassName(VirtualMachineState),

	Fields(VirtualMachineRegister, /* No fields to declare */ )

	Functions(VirtualMachineRegister,
		Function(/* .doc            = */"",
			/* .name           = */	CREATOR,
			/* .flags          = */	IS_PROTECTED,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	0,
			/* .parms          = */	Parms( ) )
		Function(/* .doc            = */"",
			/* .name           = */	CREATOR,
			/* .flags          = */	IS_PROTECTED | IS_STATIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	1,
			/* .parms          = */	Parms(	Parm("impl", T_pointer) ) )
		Function(/* .doc            = */"",
			/* .name           = */	INITIALIZER,
			/* .flags          = */	IS_PROTECTED,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	1,
			/* .parms          = */	Parms(	Parm("impl", T_pointer ) ) )
		Function(/* .doc            = */"",
			/* .name           = */	CONSTRUCTOR,
			/* .flags          = */	IS_PUBLIC | CREATE_IMPL,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	5,
			/* .parms          = */	Parms(	Parm("b",                     T_IlBuilder  ),
							Parm("localName",             T_constString),
							Parm("pointerToRegisterType", T_IlType     ),
							Parm("adjustByStep",          T_uint32     ),
							Parm("addressOfRegister",     T_IlValue    ) ) )
		Function(/* .doc            = */"",
			/* .name           = */	DESTRUCTOR,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	0,
			/* .parms          = */	Parms( ) )
		Function(/* .doc            = */"",
			/* .name           = */	Adjust,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	2,
			/* .parms          = */	Parms(	Parm("b",      T_IlBuilder),
							Parm("amount", T_IlValue  ) ) )
		Function(/* .doc            = */"",
			/* .name           = */	AdjustByConst,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"ByConst",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	2,
			/* .parms          = */	Parms(	Parm("b",      T_IlBuilder),
							Parm("amount", T_int64    ) ) )
		Function(/* .doc            = */"",
			/* .name           = */	Commit,
			/* .flags          = */	IS_PUBLIC | IS_VIRTUAL | IS_OVERIDDEN,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	1,
			/* .parms          = */	Parms(	Parm("b", T_IlBuilder ) ) )
		Function(/* .doc            = */"",
			/* .name           = */	Load,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_IlValue,
			/* .numParameters  = */	1,
			/* .parms          = */	Parms(	Parm("b", T_IlBuilder ) ) )
		Function(/* .doc            = */"",
			/* .name           = */	MakeCopy,
			/* .flags          = */	IS_PUBLIC | IS_VIRTUAL | ALLOCATES_CLASS | IS_OVERIDDEN,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_VirtualMachineState,
			/* .numParameters  = */	0,
			/* .parms          = */	Parms( ) ) 
		Function(/* .doc            = */"",
			/* .name           = */	MergeInto,
			/* .flags          = */	IS_PUBLIC | IS_VIRTUAL | IS_OVERIDDEN,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	2,
			/* .parms          = */	Parms(	Parm("vmState", T_VirtualMachineState ),
							Parm("b", T_IlBuilder                 ) ) )
		Function(/* .doc            = */"",
			/* .name           = */	Reload,
			/* .flags          = */	IS_PUBLIC | IS_VIRTUAL | IS_OVERIDDEN,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	1,
			/* .parms          = */	Parms(	Parm("b", T_IlBuilder ) ) )
		Function(/* .doc            = */"",
			/* .name           = */	Store,
			/* .flags          = */	IS_PUBLIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	2,
			/* .parms          = */	Parms(	Parm("b",     T_IlBuilder),
							Parm("value", T_IlValue  ) ) )
	)
) // Class(...VirtualMachineRegister...)

Class(	/* .doc          = */   "",
	/* .name         = */   VirtualMachineRegisterInStruct,
	/* .shortName    = */   "VMRIS",
	/* .flags        = */   IS_CREATABLE,
	/* .super        = */   pClassName(VirtualMachineRegister),

	Fields(VirtualMachineRegisterInStruct, /* No fields to declare */ )

	Functions(VirtualMachineRegisterInStruct,
		Function(/* .doc            = */"",
			/* .name           = */	CREATOR,
			/* .flags          = */	IS_PROTECTED,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	0,
			/* .parms          = */	Parms( ) )
		Function(/* .doc            = */"",
			/* .name           = */	CREATOR,
			/* .flags          = */	IS_PROTECTED | IS_STATIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	1,
			/* .parms          = */	Parms(	Parm("impl", T_pointer) ) )
		Function(/* .doc            = */"",
			/* .name           = */	INITIALIZER,
			/* .flags          = */	IS_PROTECTED,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	1,
			/* .parms          = */	Parms(	Parm("impl", T_pointer ) ) )
		Function(/* .doc            = */"",
			/* .name           = */	CONSTRUCTOR,
			/* .flags          = */	IS_PUBLIC | CREATE_IMPL,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	5,
			/* .parms          = */	Parms(	Parm("b",                             T_IlBuilder  ),
							Parm("structName",                    T_constString),
							Parm("localNameHoldingStructAddress", T_constString),
							Parm("fieldName",                     T_constString),
							Parm("localName",                     T_constString) ) )
		Function(/* .doc            = */"",
			/* .name           = */	Commit,
			/* .flags          = */	IS_PUBLIC | IS_VIRTUAL,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	1,
			/* .parms          = */	Parms(	Parm("b", T_IlBuilder ) ) )
		Function(/* .doc            = */"",
			/* .name           = */	Reload,
			/* .flags          = */	IS_PUBLIC | IS_VIRTUAL,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	1,
			/* .parms          = */	Parms(	Parm("b", T_IlBuilder ) ) )
		Function(/* .doc            = */"",
			/* .name           = */	MakeCopy,
			/* .flags          = */	IS_PUBLIC | IS_VIRTUAL | ALLOCATES_CLASS,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_VirtualMachineState,
			/* .numParameters  = */	0,
			/* .parms          = */	Parms( ) ) 
		Function(/* .doc            = */"",
			/* .name           = */	MergeInto,
			/* .flags          = */	IS_PUBLIC | IS_VIRTUAL,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	2,
			/* .parms          = */	Parms(	Parm("vmState", T_VirtualMachineState ),
							Parm("b", T_IlBuilder                 ) ) )
	)
) // Class(...VirtualMachineRegisterInStruct...)

Class(	/* .doc          = */   "",
	/* .name         = */   VirtualMachineOperandArray,
	/* .shortName    = */   "VMOA",
	/* .flags        = */   IS_CREATABLE,
	/* .super        = */   pClassName(VirtualMachineState),

	Fields(VirtualMachineOperandArray, /* No fields to declare */ )

	Functions(VirtualMachineOperandArray,
		Function(/* .doc            = */"",
			/* .name           = */	CREATOR,
			/* .flags          = */	IS_PROTECTED,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	0,
			/* .parms          = */	Parms( ) )
		Function(/* .doc            = */"",
			/* .name           = */	CREATOR,
			/* .flags          = */	IS_PROTECTED | IS_STATIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	1,
			/* .parms          = */	Parms(	Parm("impl", T_pointer) ) )
		Function(/* .doc            = */"",
			/* .name           = */	INITIALIZER,
			/* .flags          = */	IS_PROTECTED,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	1,
			/* .parms          = */	Parms(	Parm("impl", T_pointer ) ) )
		Function(/* .doc            = */"",
			/* .name           = */	CONSTRUCTOR,
			/* .flags          = */	IS_PUBLIC | CREATE_IMPL,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	4,
			/* .parms          = */	Parms(	Parm("mb",            T_MethodBuilder  ),
							Parm("numOfElements", T_int32),
							Parm("elementType",   T_IlType),
							Parm("arrayBase",     T_VirtualMachineRegister) ) )
		Function(/* .doc            = */"",
			/* .name           = */	CONSTRUCTOR,
			/* .flags          = */	IS_PUBLIC | CREATE_IMPL,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	1,
			/* .parms          = */	Parms(	Parm("other", T_VirtualMachineOperandArray ) ) )
		Function(/* .doc            = */"",
			/* .name           = */	Commit,
			/* .flags          = */	IS_PUBLIC | IS_VIRTUAL,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	1,
			/* .parms          = */	Parms(	Parm("b", T_IlBuilder ) ) )
		Function(/* .doc            = */"",
			/* .name           = */	Get,
			/* .flags          = */	IS_PUBLIC | IS_VIRTUAL,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_IlValue,
			/* .numParameters  = */	1,
			/* .parms          = */	Parms(	Parm("index", T_int32) ) ) 
		Function(/* .doc            = */"",
			/* .name           = */	MakeCopy,
			/* .flags          = */	IS_PUBLIC | IS_VIRTUAL | ALLOCATES_CLASS,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_VirtualMachineState,
			/* .numParameters  = */	0,
			/* .parms          = */	Parms( ) ) 
		Function(/* .doc            = */"",
			/* .name           = */	MergeInto,
			/* .flags          = */	IS_PUBLIC | IS_VIRTUAL,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	2,
			/* .parms          = */	Parms(	Parm("other", T_VirtualMachineState ),
							Parm("b",     T_IlBuilder                  ) ) )
		Function(/* .doc            = */"",
			/* .name           = */	Move,
			/* .flags          = */	IS_PUBLIC | IS_VIRTUAL,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	3,
			/* .parms          = */	Parms(	Parm("b", T_IlBuilder),
							Parm("dstIndex", T_int32),
							Parm("srcIndex", T_int32) ) ) 
		Function(/* .doc            = */"",
			/* .name           = */	Reload,
			/* .flags          = */	IS_PUBLIC | IS_VIRTUAL,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	1,
			/* .parms          = */	Parms(	Parm("b", T_IlBuilder ) ) )
		Function(/* .doc            = */"",
			/* .name           = */	Set,
			/* .flags          = */	IS_PUBLIC | IS_VIRTUAL,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	2,
			/* .parms          = */	Parms(	Parm("index", T_int32),
							Parm("value", T_IlValue) ) ) 
		Function(/* .doc            = */"",
			/* .name           = */	UpdateArray,
			/* .flags          = */	IS_PUBLIC | IS_VIRTUAL,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	2,
			/* .parms          = */	Parms(	Parm("b",     T_IlBuilder ),
							Parm("array", T_IlValue   ) ) )
	)
) // Class(...VirtualMachineOperandArray...)

Class(	/* .doc          = */   "",
	/* .name         = */   VirtualMachineOperandStack,
	/* .shortName    = */   "VMOA",
	/* .flags        = */   IS_CREATABLE,
	/* .super        = */   pClassName(VirtualMachineState),

	Fields(VirtualMachineOperandStack, /* No fields to declare */ )

	Functions(VirtualMachineOperandStack,
		Function(/* .doc            = */"",
			/* .name           = */	CREATOR,
			/* .flags          = */	IS_PROTECTED,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	0,
			/* .parms          = */	Parms( ) )
		Function(/* .doc            = */"",
			/* .name           = */	CREATOR,
			/* .flags          = */	IS_PROTECTED | IS_STATIC,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	1,
			/* .parms          = */	Parms(	Parm("impl", T_pointer) ) )
		Function(/* .doc            = */"",
			/* .name           = */	INITIALIZER,
			/* .flags          = */	IS_PROTECTED,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	1,
			/* .parms          = */	Parms(	Parm("impl", T_pointer ) ) )
		Function(/* .doc            = */"",
			/* .name           = */	CONSTRUCTOR,
			/* .flags          = */	IS_PUBLIC | CREATE_IMPL,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	4,
			/* .parms          = */	Parms(	Parm("mb",          T_MethodBuilder  ),
							Parm("sizeHint",    T_int32),
							Parm("elementType", T_IlType),
							Parm("stackTop",    T_VirtualMachineRegister) ) )
		Function(/* .doc            = */"",
			/* .name           = */	Commit,
			/* .flags          = */	IS_PUBLIC | IS_VIRTUAL,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	1,
			/* .parms          = */	Parms(	Parm("b", T_IlBuilder ) ) )
		Function(/* .doc            = */"",
			/* .name           = */	Drop,
			/* .flags          = */	IS_PUBLIC | IS_VIRTUAL,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	2,
			/* .parms          = */	Parms(	Parm("b",     T_IlBuilder ),
							Parm("depth", T_int32   ) ) )
		Function(/* .doc            = */"",
			/* .name           = */	Dup,
			/* .flags          = */	IS_PUBLIC | IS_VIRTUAL,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	1,
			/* .parms          = */	Parms(	Parm("b",     T_IlBuilder ) ) )
		Function(/* .doc            = */"",
			/* .name           = */	MakeCopy,
			/* .flags          = */	IS_PUBLIC | IS_VIRTUAL | ALLOCATES_CLASS,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_VirtualMachineState,
			/* .numParameters  = */	0,
			/* .parms          = */	Parms( ) ) 
		Function(/* .doc            = */"",
			/* .name           = */	MergeInto,
			/* .flags          = */	IS_PUBLIC | IS_VIRTUAL,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	2,
			/* .parms          = */	Parms(	Parm("other", T_VirtualMachineOperandStack ),
							Parm("b",     T_IlBuilder                  ) ) )
		Function(/* .doc            = */"",
			/* .name           = */	Pick,
			/* .flags          = */	IS_PUBLIC | IS_VIRTUAL,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_IlValue,
			/* .numParameters  = */	1,
			/* .parms          = */	Parms(	Parm("depth", T_int32 ) ) )
		Function(/* .doc            = */"",
			/* .name           = */	Pop,
			/* .flags          = */	IS_PUBLIC | IS_VIRTUAL,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_IlValue,
			/* .numParameters  = */	1,
			/* .parms          = */	Parms(	Parm("b", T_IlBuilder ) ) )
		Function(/* .doc            = */"",
			/* .name           = */	Push,
			/* .flags          = */	IS_PUBLIC | IS_VIRTUAL,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	2,
			/* .parms          = */	Parms(	Parm("b",     T_IlBuilder ),
							Parm("value", T_IlValue   ) ) )
		Function(/* .doc            = */"",
			/* .name           = */	Reload,
			/* .flags          = */	IS_PUBLIC | IS_VIRTUAL,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	1,
			/* .parms          = */	Parms(	Parm("b", T_IlBuilder ) ) )
		Function(/* .doc            = */"",
			/* .name           = */	Top,
			/* .flags          = */	IS_PUBLIC | IS_VIRTUAL,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_IlValue,
			/* .numParameters  = */	0,
			/* .parms          = */	Parms( ) )
		Function(/* .doc            = */"",
			/* .name           = */	UpdateStack,
			/* .flags          = */	IS_PUBLIC | IS_VIRTUAL,
			/* .overloadSuffix = */	"",
			/* .returnType     = */	T_none,
			/* .numParameters  = */	2,
			/* .parms          = */	Parms(	Parm("b",     T_IlBuilder ),
							Parm("array", T_IlValue   ) ) )
	)
)

Classes(
   pClassName(BytecodeBuilder),
   pClassName(IlBuilder),
   pClassName(IlReference),
   pClassName(IlType),
   pClassName(IlValue),
   pClassName(MethodBuilder),
   pClassName(ThunkBuilder),
   pClassName(TypeDictionary),
   pClassName(VirtualMachineOperandArray),
   pClassName(VirtualMachineOperandStack),
   pClassName(VirtualMachineRegister),
   pClassName(VirtualMachineRegisterInStruct),
   pClassName(VirtualMachineState),
)

// just define these explicitly for now
static JBField *globals[] = { };
static int numGlobals = sizeof(globals) / sizeof(JBField *);

GlobalFunctions(
	Function(/* .doc           = */"",
		/* .name           = */	initializeJit,
		/* .flags          = */	IS_PUBLIC | IS_STATIC,
		/* .overloadSuffix = */	"",
		/* .returnType     = */	T_boolean,
		/* .numParameters  = */	0,
		/* .parms          = */	Parms( ) ) 
	Function(/* .doc           = */"",
		/* .name           = */	initializeJitWithOptions,
		/* .flags          = */	IS_PUBLIC | IS_STATIC,
		/* .overloadSuffix = */	"",
		/* .returnType     = */	T_boolean,
		/* .numParameters  = */	1,
		/* .parms          = */	Parms(	Parm("options", T_string) ) ) 
	Function(/* .doc           = */"",
		/* .name           = */	compileMethodBuilder,
		/* .flags          = */	IS_PUBLIC | IS_STATIC,
		/* .overloadSuffix = */	"",
		/* .returnType     = */	T_int32,
		/* .numParameters  = */	2,
		/* .parms          = */	Parms(	Parm("methodBuilder", T_MethodBuilder),
						Parm("entryPoint",    T_ppointer) ) ) 
	Function(/* .doc           = */"",
		/* .name           = */	shutdownJit,
		/* .flags          = */	IS_PUBLIC | IS_STATIC,
		/* .overloadSuffix = */	"",
		/* .returnType     = */	T_none,
		/* .numParameters  = */	0,
		/* .parms          = */	Parms( ) ) 
) // GlobalFunctions(...)

} // namespace JitBuilderCoreAPI

JitBuilderAPI JitBuilderAPI::_coreAPI(JitBuilderCoreAPI::numClasses,         (JBClass **)   JitBuilderCoreAPI::classes,
                                      JitBuilderCoreAPI::numGlobals,         (JBField *)    JitBuilderCoreAPI::globals,
                                      JitBuilderCoreAPI::numGlobalFunctions, (JBFunction *) JitBuilderCoreAPI::globalFunctions);

