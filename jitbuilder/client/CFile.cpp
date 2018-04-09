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


#include "CFile.hpp"

using namespace std;

static string DEFAULT_COPYRIGHT_START("/*******************************************************************************\n");
static string DEFAULT_COPYRIGHT_LINESTART(" * ");
static string DEFAULT_COPYRIGHT_END(" *******************************************************************************/\n");

string
CFile::startCopyrightComment()
   {
   return DEFAULT_COPYRIGHT_START;
   }

string
CFile::copyrightLineStart()
   {
   return DEFAULT_COPYRIGHT_LINESTART;
   }

string
CFile::endCopyrightComment()
   {
   return DEFAULT_COPYRIGHT_END;
   }

bool
CFile::isArrayClassType(JBType type)
   {
   switch (type)
      {
      case T_IlBuilderArray:
      case T_IlValueArray:
         return true;

      default:
         break;
      }
   return false;
   }

string
CFile::typeName(JBType type)
   {
   switch (type)
      {
      case T_none                          : { return "void"; }
      case T_boolean                       : { return "int8_t"; }
      case T_integer                       : { return "size_t"; }
      case T_int8                          : { return "int8_t"; }
      case T_int16                         : { return "int16_t"; }
      case T_int32                         : { return "int32_t"; }
      case T_int64                         : { return "int64_t"; }
      case T_uint32                        : { return "uint32_t"; }
      case T_float                         : { return "float"; }
      case T_double                        : { return "double"; }
      case T_pointer                       : { return "void *"; }
      case T_ppointer                      : { return "void **"; }
      case T_unsignedInteger               : { return "size_t"; }
      case T_constString                   : { return "const char *"; }
      case T_string                        : { return "char *"; }
      case T_booleanArray                  : { return "int8_t *"; }
      case T_int32Array                    : { return "int32_t *"; }
      case T_vararg                        : { return ""; }
      case T_BytecodeBuilder               : { return "BytecodeBuilder *"; }
      case T_BytecodeBuilderArray          : { return "BytecodeBuilder **"; }
      case T_BytecodeBuilderByRef          : { return "BytecodeBuilder **"; }
      case T_BytecodeBuilderImpl           : { return "TR::BytecodeBuilder *"; }
      case T_ppBytecodeBuilder             : { return "BytecodeBuilder **"; }
      case T_IlBuilder                     : { return "IlBuilder *"; }
      case T_IlBuilderArray                : { return "IlBuilder **"; }
      case T_IlBuilderImpl                 : { return "TR::IlBuilder *"; }
      case T_IlBuilderByRef                : { return "IlBuilder **"; }
      case T_ppIlBuilder                   : { return "IlBuilder **"; }
      case T_MethodBuilder                 : { return "MethodBuilder *"; }
      case T_MethodBuilderImpl             : { return "TR::MethodBuilder *"; }
      case T_IlReference                   : { return "IlReference *"; }
      case T_IlType                        : { return "IlType *"; }
      case T_IlTypeArray                   : { return "IlType **"; }
      case T_IlTypeImpl                    : { return "TR::IlType *"; }
      case T_IlValue                       : { return "IlValue *"; }
      case T_IlValueImpl                   : { return "TR::IlValue *"; }
      case T_IlValueArray                  : { return "IlValue **"; }
      case T_ThunkBuilder                  : { return "ThunkBuilder *"; }
      case T_TypeDictionary                : { return "TypeDictionary *"; }
      case T_TypeDictionaryImpl            : { return "TR::TypeDictionary *"; }
      case T_VirtualMachineOperandArray    : { return "VirtualMachineOperandArray *"; }
      case T_VirtualMachineOperandStack    : { return "VirtualMachineOperandStack *"; }
      case T_VirtualMachineRegister        : { return "VirtualMachineRegister *"; }
      case T_VirtualMachineRegisterInStruct: { return "VirtualMachineRegisterInStruct *"; }
      case T_VirtualMachineState           : { return "VirtualMachineState *"; }
      case T_VirtualMachineStateImpl       : { return "TR::VirtualMachineState *"; }
      default                              : { cerr << "Error generating text for type " << type << endl; assert(0); }
      }
   return "";
   }

string
CFile::bareTypeName(JBType type)
   {
   switch (type)
      {
      case T_BytecodeBuilder                : { return "BytecodeBuilder"; }
      case T_IlBuilder                      : { return "IlBuilder"; }
      case T_IlBuilderByRef                 : { return "IlBuilder"; }
      case T_MethodBuilder                  : { return "MethodBuilder"; }
      case T_IlReference                    : { return "IlReference"; }
      case T_IlType                         : { return "IlType"; }
      case T_IlValue                        : { return "IlValue"; }
      case T_TypeDictionary                 : { return "TypeDictionary"; }
      case T_ThunkBuilder                   : { return "ThunkBuilder"; }
      case T_VirtualMachineOperandArray     : { return "VirtualMachineOperandArray"; }
      case T_VirtualMachineOperandStack     : { return "VirtualMachineOperandStack"; }
      case T_VirtualMachineRegister         : { return "VirtualMachineRegister"; }
      case T_VirtualMachineRegisterInStruct : { return "VirtualMachineRegisterInStruct"; }
      case T_VirtualMachineState            : { return "VirtualMachineState"; }
      default                               : { cerr << "Error generating bare type name for type " << type << endl; assert(0); }
      }
   return "";
   }

string
CFile::implName(JBType type)
   {
   switch (type)
      {
      case T_boolean                        : return "int8_t";
      case T_int32                          : return "int32_t";
      case T_string                         : return "char *";
      case T_constString                    : return "const char *";
      case T_unsignedInteger                : return "size_t";
      case T_BytecodeBuilder                : return "TR::BytecodeBuilder *";
      case T_IlBuilder                      : return "TR::IlBuilder *";
      case T_IlBuilderByRef                 : return "TR::IlBuilder *";
      case T_MethodBuilder                  : return "TR::MethodBuilder *";
      case T_IlReference                    : return "TR::IlReference *";
      case T_IlType                         : return "TR::IlType *";
      case T_IlValue                        : return "TR::IlValue *";
      case T_ThunkBuilder                   : return "TR::ThunkBuilder *";
      case T_TypeDictionary                 : return "TR::TypeDictionary *";
      case T_VirtualMachineOperandArray     : return "TR::VirtualMachineOperandArray *";
      case T_VirtualMachineOperandStack     : return "TR::VirtualMachineOperandStack *";
      case T_VirtualMachineRegister         : return "TR::VirtualMachineRegister *";
      case T_VirtualMachineRegisterInStruct : return "TR::VirtualMachineRegisterInStruct *";
      case T_VirtualMachineState            : return "TR::VirtualMachineState *";
      default                               : break;
      }

   return "";
   }

string
CFile::implTypeCast(JBType type)
   {
   switch (type)
      {
      case T_pointer                        : return "(void *)";
      case T_ppointer                       : return "(void **)";
      case T_BytecodeBuilder                : return "(TR::BytecodeBuilder *)";
      case T_IlBuilder                      : return "(TR::IlBuilder *)";
      case T_MethodBuilder                  : return "(TR::MethodBuilder *)";
      case T_IlReference                    : return "(TR::IlReference *)";
      case T_IlType                         : return "(TR::IlType *)";
      case T_IlValue                        : return "(TR::IlValue *)";
      case T_ThunkBuilder                   : return "(TR::ThunkBuilder *)";
      case T_TypeDictionary                 : return "(TR::TypeDictionary *)";
      case T_VirtualMachineOperandArray     : return "(TR::VirtualMachineOperandArray *)";
      case T_VirtualMachineOperandStack     : return "(TR::VirtualMachineOperandStack *)";
      case T_VirtualMachineRegister         : return "(TR::VirtualMachineRegister *)";
      case T_VirtualMachineRegisterInStruct : return "(TR::VirtualMachineRegisterInStruct *)";
      case T_VirtualMachineState            : return "(TR::VirtualMachineState *)";
      case T_BytecodeBuilderByRef           : return "(TR::BytecodeBuilder *)";
      case T_IlBuilderByRef                 : return "(TR::IlBuilder *)";

      case T_boolean:
      case T_integer:
      case T_int8:
      case T_int16:
      case T_int32:
      case T_int64:
      case T_uint32:
      case T_float:
      case T_double:
      case T_unsignedInteger:
      case T_constString:
      case T_string:
      case T_booleanArray:
      case T_int32Array:
      case T_vararg:
      case T_BytecodeBuilderImpl:
      case T_IlBuilderArray:
      case T_IlBuilderImpl:
      case T_IlTypeArray:
      case T_IlTypeImpl:
      case T_IlValueArray:
      case T_IlValueImpl:
      case T_TypeDictionaryImpl:
         return "";

      default:
         cerr << "Error generating impl type cast text for type " << type << endl;
         assert(0);
      }
   return "";
   }

bool
CFile::hasImpl(JBType type)
   {
   switch (type)
      {
      case T_boolean:
      case T_integer:
      case T_int8:
      case T_int16:
      case T_int32:
      case T_int64:
      case T_uint32:
      case T_float:
      case T_double:
      case T_pointer:
      case T_unsignedInteger:
      case T_constString:
      case T_string:
      case T_booleanArray:
      case T_int32Array:
      case T_vararg:
      case T_BytecodeBuilderByRef:
      case T_BytecodeBuilderImpl:
      case T_IlBuilderArray:
      case T_IlBuilderByRef:
      case T_IlBuilderImpl:
      case T_IlTypeArray:
      case T_IlTypeImpl:
      case T_IlValueArray:
      case T_IlValueImpl:
      case T_TypeDictionaryImpl:
         return false;

      case T_BytecodeBuilder:
      case T_IlBuilder:
      case T_MethodBuilder:
      case T_IlReference:
      case T_IlType:
      case T_IlValue:
      case T_ThunkBuilder:
      case T_TypeDictionary:
      case T_VirtualMachineOperandArray:
      case T_VirtualMachineOperandStack:
      case T_VirtualMachineRegister:
      case T_VirtualMachineRegisterInStruct:
      case T_VirtualMachineState:
         return true;

      // Specifically avoiding these ones as they should never be asked about
      //case T_none                 : { return "void"; }
      default                     : { cerr << "Error hasImpl() for type " << type << endl; assert(0); }
      }
   return "";
   }

string
CFile::callbackFieldName(JBFunction *func)
   {
   return "_callback_" + func->name;
   }

string
CFile::functionName(JBFunction *func, JBClass *clazz)
   {
   if (clazz == NULL)
      return func->name;
   if (func->isCreator())
      return "Create_" + clazz->name;

   return clazz->shortName + "_" + func->name;
   }

string
CFile::implFunctionName(JBFunction *func)
   {
   size_t L = func->overloadSuffix.length();
   if (L == 0)
      return func->name;

   return func->name.substr(0, func->name.length() - L);
   }

string
CFile::fieldName(JBField *field)
   {
   return field->name;
   }

string
CFile::implClassName(JBClass *clazz)
   {
   return "TR::" + clazz->name;
   }

string
CFile::fieldQualifiers(JBField *field)
   {
   if (field->flags & IS_STATIC)
      return "static ";

   return "";
   }

string
CFile::functionDeclarationText(JBFunction *func, JBClass *clazz)
   {
   std::string decl;

   if (func->isCreator())
      decl += typeName(clazz->type) + " ";
   else
      decl += typeName(func->returnType) + " ";

   decl += functionName(func, clazz) + "(";

   if (!func->isStatic()) // need an explicit "receiver" object
      decl += "struct " + clazz->name + " * receiverObject";

   if (func->numParameters > 0)
      {
      if (!func->isStatic())
         decl += ", ";
      decl += typeName(func->parms[0].type) + " " + func->parms[0].name;

      for (int p=1;p < func->numParameters;p++)
         {
         JBParameter & parm = func->parms[p];
         decl += ", " + typeName(parm.type) + " " + parm.name;
         if (parm.type == T_vararg)
            break;
         }
      }
   decl += ")";

   return decl;
   }

void
CFile::includeFile(string fileName)
   {
   _s << "#include \"" << fileName << "\"\n";
   }

void
CFile::includeSystemFile(string fileName)
   {
   _s << "#include <" << fileName << ">\n";
   }

void
CFile::beginStruct(string name)
   {
   startScope("typedef struct " + name);
   }

void
CFile::blankLine()
   {
   _s << "\n";
   }

void
CFile::comment(string text)
   {
   indent() << "/* " << text << " */\n";
   }

void
CFile::endScope(string scope, string comment, bool doDedent, bool isStatement)
   {
   if (doDedent)
      backtab();

   indent() << "}";
   if (scope.length() > 0)
      _s << " " << scope << ";";
   else if (isStatement)
      _s << ";";

   if (comment.length() > 0)
      _s << "  /* " << comment << "*/";

   _s << "\n\n";
   }

void
CFile::endStruct(string name)
   {
   endScope(name);
   }

void
CFile::fieldDefinition(JBField *field)
   {
   indent() << fieldQualifiers(field) << typeName(field->type) << fieldName(field) << ";\n";
   }

void
CFile::callbackDefinition(JBFunction *func)
   {
   indent() << "void *" << callbackFieldName(func) << ";\n";
   }

void
CFile::startScope(string scope, bool doIndent)
   {
   _s << "\n";
   indent() << scope << " {\n";
   if (doIndent)
      tabstop();
   }

void
CFile::typedefClass(JBClass *clazz)
   {
   indent() << "typedef struct " << clazz->name << " " << clazz->name << ";\n";
   }
