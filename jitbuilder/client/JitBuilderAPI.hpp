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

#if !defined(JITBUILDERAPI_INCL)
#define JITBUILDERAPI_INCL

#include <assert.h>
#include <string>

#define TOSTR(s) STR(s)
#define STR(s) #s

typedef enum JBType
   {
   T_none = 0,
   T_boolean,
   T_int8,
   T_int16,
   T_int32,
   T_int64,
   T_uint32,
   T_float,
   T_double,
   T_integer,
   T_unsignedInteger,
   T_constString,
   T_string,
   T_vararg,
   T_pointer,
   T_ppointer,
   T_booleanArray,
   T_int32Array,
   T_BytecodeBuilder,
   T_BytecodeBuilderArray,
   T_BytecodeBuilderByRef,
   T_BytecodeBuilderImpl,
   T_ppBytecodeBuilder,
   T_IlBuilder,
   T_IlBuilderArray,
   T_IlBuilderByRef,
   T_IlBuilderImpl,
   T_ppIlBuilder,
   T_MethodBuilder,
   T_MethodBuilderImpl,
   T_IlReference,
   T_IlType,
   T_IlTypeArray,
   T_IlTypeImpl,
   T_IlValue,
   T_IlValueImpl,
   T_IlValueArray,
   T_ThunkBuilder,
   T_TypeDictionary,
   T_TypeDictionaryImpl,
   T_VirtualMachineOperandArray,
   T_VirtualMachineOperandStack,
   T_VirtualMachineRegister,
   T_VirtualMachineRegisterInStruct,
   T_VirtualMachineState,
   T_VirtualMachineStateImpl,
   } JBType;


/* possible flag settings */
#define ACCESS_FLAG		1
#define	IS_PROTECTED		(0)
#define	IS_PUBLIC		(1)

#define IS_STATIC		(1<<1)
#define IS_VIRTUAL		(1<<2)
#define	IS_CREATABLE		(1<<3)
#define ASSIGN_AT_INIT  	(1<<4)
#define IS_CALLBACK		(1<<5)
#define CREATE_IMPL             (1<<6)
#define HAS_EXTRAS_HEADER       (1<<7)
#define ALLOCATES_CLASS         (1<<8)
#define IS_OVERIDDEN            (1<<9)


typedef uint32_t flags_t;

typedef struct JBField
   {
   std::string  name;
   JBType       type;
   flags_t      flags;
   } JBField;

typedef struct JBParameter
   {
   std::string  name;
   JBType       type;

   bool passedByRef() { return (type == T_BytecodeBuilderByRef) || (type == T_IlBuilderByRef) || (type == T_IlTypeArray) || (type == T_IlValueArray); }
   } JBParameter;

#define NO_PARAMETERS
#define CREATOR Create
#define INITIALIZER initializeFromImpl
#define CONSTRUCTOR Initialize
#define DESTRUCTOR Destroy

#define CREATOR_STR     TOSTR(CREATOR)
#define INITIALIZER_STR TOSTR(INITIALIZER)
#define CONSTRUCTOR_STR TOSTR(CONSTRUCTOR)
#define DESTRUCTOR_STR  TOSTR(DESTRUCTOR)

#define CREATOR_LEN     strlen(CREATOR_STR)
#define INITIALIZER_LEN strlen(INITIALIZERR_STR)
#define CONSTRUCTOR_LEN strlen(CONSTRUCTOR_STR)
#define DESTRUCTOR_LEN  strlen(DESTRUCTOR_STR)

typedef struct JBFunction
   {
   std::string  doc;
   std::string  name;
   std::string  overloadSuffix;
   JBType       returnType;
   flags_t      flags;
   uint32_t     numParameters;
   JBParameter  parms[8];

   bool isCallback()         { return (flags & IS_CALLBACK) != 0; }
   bool isConstructor()      { return name == CONSTRUCTOR_STR; }
   bool isCreator()          { return name == CREATOR_STR; }
   bool isInitializer()      { return name == INITIALIZER_STR; }
   bool isDestructor()       { return name == DESTRUCTOR_STR; }
   bool isStatic()           { return (flags & IS_STATIC) != 0; }
   bool isVirtual()          { return (flags & IS_VIRTUAL) != 0; }
   bool allocatesThisClass() { return (flags & ALLOCATES_CLASS) != 0; }
   bool isOveridden()        { return (flags & IS_OVERIDDEN) != 0; }

   } JBFunction;

#define NO_SUPER  NULL

typedef struct JBClass
   {
   std::string  doc;
   std::string  name;
   std::string  shortName;
   JBType       type;
   flags_t      flags;
   JBClass *    super;
   uint32_t     numFields;
   JBField    * fields;
   uint32_t     numFunctions;
   JBFunction * functions;

   bool hasSuper()        { return super != NULL; }
   bool hasExtrasHeader() { return ((flags & HAS_EXTRAS_HEADER) != 0); }

   class FunctionIterator
      {
      public:
      FunctionIterator(int numFunctions, JBFunction *functions)
         : _numFunctions(numFunctions),
           _functions(functions),
           _cursor(0)
         { }
      JBFunction * operator*() // not returning & because don't want to be modifiable
         {
         assert (_cursor < _numFunctions);
         return _functions + _cursor;
         }
      bool operator!=(JBFunction *other)
         {
         assert(other == NULL);
         return (_cursor != _numFunctions);
         }
      FunctionIterator & operator ++()  // only prefix ++ defined
         {
         if (_cursor < _numFunctions)
            ++_cursor;
         return *this;
         }

      private:
      int _numFunctions;
      JBFunction *_functions;
      int _cursor;
      };

   JBClass::FunctionIterator functionsBegin()    { return JBClass::FunctionIterator(numFunctions, functions); }
   JBFunction *              functionsEnd()      { return NULL; }

   class SuperIterator
      {
      public:
      SuperIterator(JBClass *firstSuper)
         : _cursor(firstSuper)
         { }
      JBClass * operator*() // not returning & because don't want to be modifiable
         {
         assert (_cursor != NULL);
         return _cursor;
         }
      bool operator!=(JBClass *other)
         {
         return (_cursor != other);
         }
      SuperIterator & operator ++()  // only prefix ++ defined
         {
         if (_cursor != NULL)
            _cursor = _cursor->super;
         return *this;
         }

      private:
      JBClass * _cursor;
      };

   JBClass::SuperIterator supersBegin()    { return JBClass::SuperIterator(super); }
   JBClass *              supersEnd()      { return NULL; }

   } JBClass;

#define ClassName(name) __ ## name ## Class__
#define ClassFieldsName(name) __ ## name ## Fields__
#define ClassFunctionsName(name) __ ## name ## Functions__
#define pClassName(name) &__ ## name ## Class__

/**
 * Helper macros to describe a class
 */
#define Class(doc, name, shortName, flags, super, definitions )                                \
	definitions                                                                            \
	static JBClass ClassName(name) = {                                                     \
		/* .doc          = */ doc,                                                     \
		/* .name         = */ TOSTR(name),                                             \
		/* .shortName    = */ shortName,                                               \
		/* .type         = */ T_ ## name,                                              \
		/* .flags        = */ flags,                                                   \
		/* .super        = */ super,                                                   \
		/* .numFields    = */ (sizeof(ClassFieldsName(name)) / sizeof(JBField)),       \
		/* .fields       = */ ClassFieldsName(name),                                   \
		/* .numFunctions = */ (sizeof(ClassFunctionsName(name)) / sizeof(JBFunction)), \
		/* .functions    = */ ClassFunctionsName(name)                                 \
	};

/**
 * Helper macros to describe the fields of a class
 */

#define Fields(className, ... ) \
	static JBField ClassFieldsName(className) [] = { \
		__VA_ARGS__ \
	};

#define Field(name, type, flags) \
	{ name, type, flags },

/**
 * Helper macros to describe the functions of a class
 */

#define Functions(className, ... ) \
	static JBFunction ClassFunctionsName(className) [] = { \
		__VA_ARGS__ \
	};

#define Function(doc, name, flags, overloadSuffix, returnType, numParameters, parameters) \
	{ doc, TOSTR(name), overloadSuffix, returnType, flags, numParameters, parameters },

#define Parms(...) { __VA_ARGS__ },
#define Parm(name, type) { name, type }

/**
 * Helper macro to create the full list of classes
 * note that arguments must explicitly use &ClassName() or pClassName() macro for Classes() macro to work properly
 */
#define Classes(...) \
	static JBClass *classes[] = { __VA_ARGS__ }; \
	static int numClasses = sizeof(classes) / sizeof(JBClass *);

#define GlobalFunctions(... ) \
	static JBFunction globalFunctions [] = { \
		__VA_ARGS__ \
	}; \
	static int numGlobalFunctions = sizeof(globalFunctions) / sizeof(JBFunction);



class JitBuilderAPI
   {
public:
   // non-core API extensions should be created with this constructor
   JitBuilderAPI(JitBuilderAPI *baseAPI, int numClasses, JBClass **classes, int numGlobals, JBField *globals, int numGlobalFunctions, JBFunction *globalFunctions)
      : _baseAPI(baseAPI),
        _numClasses(numClasses),
        _classes(classes),
        _numGlobals(numGlobals),
        _globals(globals),
        _numGlobalFunctions(numGlobalFunctions),
        _globalFunctions(globalFunctions)
      {
      }

   // should only be used to create the core API object
   JitBuilderAPI(int numClasses, JBClass **classes, int numGlobals, JBField *globals, int numGlobalFunctions, JBFunction *globalFunctions)
      : _baseAPI(NULL),
        _numClasses(numClasses),
        _classes(classes),
        _numGlobals(numGlobals),
        _globals(globals),
        _numGlobalFunctions(numGlobalFunctions),
        _globalFunctions(globalFunctions)
      {
      }

   static JitBuilderAPI & coreAPI() { return _coreAPI; }

   class ClassIterator
      {
      public:
      ClassIterator(int numClasses, JBClass **classes)
         : _numClasses(numClasses),
           _classes(classes),
           _cursor(0)
         { }
      JBClass * operator*() // not returning & because don't want to be modifiable
         {
         assert(_cursor < _numClasses);
         return _classes[_cursor];
         }
      bool operator!=(JBClass *other)
         {
         assert(other == NULL);
         return (_cursor != _numClasses);
         }
      ClassIterator & operator ++()  // only prefix ++ defined
         {
         if (_cursor < _numClasses)
            ++_cursor;
         return *this;
         }

      private:
      int _numClasses;
      JBClass **_classes;
      int _cursor;
      };

   JitBuilderAPI::ClassIterator classesBegin() { return JitBuilderAPI::ClassIterator(_numClasses, _classes); }
   JBClass *                    classesEnd()   { return NULL; }

   class FunctionIterator
      {
      public:
      FunctionIterator(int numFunctions, JBFunction *functions)
         : _numFunctions(numFunctions),
           _functions(functions),
           _cursor(0)
         { }
      JBFunction * operator*() // not returning & because don't want to be modifiable
         {
         assert(_cursor < _numFunctions);
         return _functions+_cursor;
         }
      bool operator!=(JBFunction *other)
         {
         assert(other == NULL);
         return (_cursor != _numFunctions);
         }
      FunctionIterator & operator ++()  // only prefix ++ defined
         {
         if (_cursor < _numFunctions)
            ++_cursor;
         return *this;
         }
      private:
      int _numFunctions;
      JBFunction *_functions;
      int _cursor;
      };

   JitBuilderAPI::FunctionIterator functionsBegin() { return JitBuilderAPI::FunctionIterator(_numGlobalFunctions, _globalFunctions); }
   JBFunction *                    functionsEnd()   { return NULL; }


protected:
   static JitBuilderAPI _coreAPI;

   JitBuilderAPI *_baseAPI;

   int _numClasses;
   JBClass **_classes;

   int _numGlobals;
   JBField *_globals;

   int _numGlobalFunctions;
   JBFunction *_globalFunctions;

   };

#endif // !defined(JITBUILDERAPI_INCL)
