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


#include "CPPFile.hpp"

using namespace std;

static string DEFAULT_COPYRIGHT_START("/*******************************************************************************\n");
static string DEFAULT_COPYRIGHT_LINESTART(" * ");
static string DEFAULT_COPYRIGHT_END(" *******************************************************************************/\n");

string
CPPFile::startCopyrightComment()
   {
   return DEFAULT_COPYRIGHT_START;
   }

string
CPPFile::copyrightLineStart()
   {
   return DEFAULT_COPYRIGHT_LINESTART;
   }

string
CPPFile::endCopyrightComment()
   {
   return DEFAULT_COPYRIGHT_END;
   }

bool
CPPFile::isArrayClassType(JBType type)
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
CPPFile::typeName(JBType type)
   {
   switch (type)
      {
      case T_none                           : { return "void"; }
      case T_boolean                        : { return "bool"; }
      case T_integer                        : { return "size_t"; }
      case T_int8                           : { return "int8_t"; }
      case T_int16                          : { return "int16_t"; }
      case T_int32                          : { return "int32_t"; }
      case T_int64                          : { return "int64_t"; }
      case T_uint32                         : { return "uint32_t"; }
      case T_float                          : { return "float"; }
      case T_double                         : { return "double"; }
      case T_pointer                        : { return "void *"; }
      case T_ppointer                       : { return "void **"; }
      case T_unsignedInteger                : { return "size_t"; }
      case T_constString                    : { return "const char *"; }
      case T_string                         : { return "char *"; }
      case T_booleanArray                   : { return "bool *"; }
      case T_int32Array                     : { return "int32_t *"; }
      case T_vararg                         : { return ""; }
      case T_BytecodeBuilder                : { return "BytecodeBuilder *"; }
      case T_BytecodeBuilderArray           : { return "BytecodeBuilder **"; }
      case T_BytecodeBuilderByRef           : { return "BytecodeBuilder **"; }
      case T_BytecodeBuilderImpl            : { return "TR::BytecodeBuilder *"; }
      case T_ppBytecodeBuilder              : { return "BytecodeBuilder **"; }
      case T_IlBuilder                      : { return "IlBuilder *"; }
      case T_IlBuilderArray                 : { return "IlBuilder **"; }
      case T_IlBuilderImpl                  : { return "TR::IlBuilder *"; }
      case T_IlBuilderByRef                 : { return "IlBuilder **"; }
      case T_ppIlBuilder                    : { return "IlBuilder **"; }
      case T_MethodBuilder                  : { return "MethodBuilder *"; }
      case T_MethodBuilderImpl              : { return "TR::MethodBuilder *"; }
      case T_IlReference                    : { return "IlReference *"; }
      case T_IlType                         : { return "IlType *"; }
      case T_IlTypeArray                    : { return "IlType **"; }
      case T_IlTypeImpl                     : { return "TR::IlType *"; }
      case T_IlValue                        : { return "IlValue *"; }
      case T_IlValueImpl                    : { return "TR::IlValue *"; }
      case T_IlValueArray                   : { return "IlValue **"; }
      case T_ThunkBuilder                   : { return "ThunkBuilder *"; }
      case T_TypeDictionary                 : { return "TypeDictionary *"; }
      case T_TypeDictionaryImpl             : { return "TR::TypeDictionary *"; }
      case T_VirtualMachineOperandArray     : { return "VirtualMachineOperandArray *"; }
      case T_VirtualMachineOperandStack     : { return "VirtualMachineOperandStack *"; }
      case T_VirtualMachineRegister         : { return "VirtualMachineRegister *"; }
      case T_VirtualMachineRegisterInStruct : { return "VirtualMachineRegisterInStruct *"; }
      case T_VirtualMachineState            : { return "VirtualMachineState *"; }
      case T_VirtualMachineStateImpl        : { return "TR::VirtualMachineState *"; }
      default                               : { cerr << "Error generating text for type " << type << endl; assert(0); }
      }
   return "";
   }

string
CPPFile::bareTypeName(JBType type)
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
      case T_ThunkBuilder                   : { return "ThunkBuilder"; }
      case T_TypeDictionary                 : { return "TypeDictionary"; }
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
CPPFile::implName(JBType type)
   {
   switch (type)
      {
      case T_boolean                        : return "bool";
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
CPPFile::implTypeCast(JBType type)
   {
   switch (type)
      {
      case T_pointer:                        return "(void *)";
      case T_ppointer:                       return "(void **)";
      case T_BytecodeBuilder:                return "(TR::BytecodeBuilder *)";
      case T_IlBuilder:                      return "(TR::IlBuilder *)";
      case T_MethodBuilder:                  return "(TR::MethodBuilder *)";
      case T_IlReference:                    return "(TR::IlReference *)";
      case T_IlType:                         return "(TR::IlType *)";
      case T_IlValue:                        return "(TR::IlValue *)";
      case T_ThunkBuilder:                   return "(TR::ThunkBuilder *)";
      case T_TypeDictionary:                 return "(TR::TypeDictionary *)";
      case T_VirtualMachineOperandArray:     return "(TR::VirtualMachineOperandArray *)";
      case T_VirtualMachineOperandStack:     return "(TR::VirtualMachineOperandStack *)";
      case T_VirtualMachineRegister:         return "(TR::VirtualMachineRegister *)";
      case T_VirtualMachineRegisterInStruct: return "(TR::VirtualMachineRegisterInStruct *)";
      case T_VirtualMachineState:            return "(TR::VirtualMachineState *)";

      case T_BytecodeBuilderByRef:           return "(TR::BytecodeBuilder *)";
      case T_IlBuilderByRef:                 return "(TR::IlBuilder *)";

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
CPPFile::hasImpl(JBType type)
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

      default:
         cerr << "Error hasImpl() for type " << type << endl;
         assert(0);
      }
   return "";
   }

string
CPPFile::functionName(JBFunction *func, JBClass *clazz)
   {
   string specifier = classSpecifier(clazz);
   if (func->isConstructor() || func->isCreator())
      return specifier + clazz->name;
   if (func->isDestructor())
      return specifier + "~" + clazz->name;

   return specifier + implFunctionName(func);
   }

string
CPPFile::implFunctionName(JBFunction *func)
   {
   size_t L = func->overloadSuffix.length();
   if (L == 0)
      return func->name;

   return func->name.substr(0, func->name.length() - L);
   }

string
CPPFile::fieldName(JBField *field)
   {
   return field->name;
   }

string
CPPFile::implClassName(JBClass *clazz)
   {
   return "TR::" + clazz->name;
   }

string
CPPFile::fieldQualifiers(JBField *field)
   {
   if (field->flags & IS_STATIC)
      return "static ";

   return "";
   }

string
CPPFile::functionDeclarationText(JBFunction *func, JBClass *clazz)
   {
   std::string decl;

   if (!func->isCreator())
      {
      if (!func->isConstructor() && !func->isDestructor())
         decl += typeName(func->returnType) + " ";
      }

   decl += functionName(func, clazz) + "(";

   if (func->numParameters > 0)
      {
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
CPPFile::includeFile(string fileName)
   {
   _s << "#include \"" << fileName << "\"\n";
   }

void
CPPFile::includeSystemFile(string fileName)
   {
   _s << "#include <" << fileName << ">\n";
   }

void
CPPFile::beginClass(JBClass *clazz)
   {
   string classLine = "class " + clazz->name;
   if (clazz->hasSuper())
      classLine += " : public " + clazz->super->name;
   startScope(classLine);
   backtab();

   //if ((clazz->flags & IS_CREATABLE) == 0)
   if (0)
      {
      // give class a protected default constructor for supers to use
      indent() << "protected:\n";
      tabstop();
      indent() << clazz->name << "() { }\n";
      backtab();
      }
 
   indent() << "public:\n";
   tabstop();
   }

void
CPPFile::blankLine()
   {
   _s << "\n";
   }

void
CPPFile::comment(string text)
   {
   indent() << "/* " << text << " */\n";
   }

void
CPPFile::endScope(string scope, string comment, bool doDedent, bool isStatement)
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
CPPFile::endClass(JBClass *clazz)
   {
   endScope("", "", true, true);
   }

void
CPPFile::fieldDefinition(JBField *field)
   {
   indent() << fieldQualifiers(field) << typeName(field->type) << fieldName(field) << ";\n";
   }

void
CPPFile::endNamespace()
   {
   indent() << "} // namespace JitBuilder\n";
   indent() << "} // namespace OMR\n";
   }

void
CPPFile::startNamespace()
   {
   _s << "\n";
   indent() << "namespace OMR        {\n";
   indent() << "namespace JitBuilder {\n";
   _s << "\n";
   }

void
CPPFile::startScope(string scope, bool doIndent)
   {
   _s << "\n";
   indent() << scope << " {\n";
   if (doIndent)
      tabstop();
   }

void
CPPFile::typedefClass(JBClass *clazz)
   {
   indent() << "class " << clazz->name << ";\n";
   }

void
CPPFile::usingNamespace(string theNamespace)
   {
   indent() << "using namespace " << theNamespace << ";\n";
   }
