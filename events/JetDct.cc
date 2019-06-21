// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME JetDct

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "RConfig.h"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Since CINT ignores the std namespace, we need to do so in this file.
namespace std {} using namespace std;

// Header files passed as explicit arguments
#include "JetEvent.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void *new_JetData(void *p = 0);
   static void *newArray_JetData(Long_t size, void *p);
   static void delete_JetData(void *p);
   static void deleteArray_JetData(void *p);
   static void destruct_JetData(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::JetData*)
   {
      ::JetData *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::JetData >(0);
      static ::ROOT::TGenericClassInfo 
         instance("JetData", ::JetData::Class_Version(), "JetEvent.h", 60,
                  typeid(::JetData), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::JetData::Dictionary, isa_proxy, 4,
                  sizeof(::JetData) );
      instance.SetNew(&new_JetData);
      instance.SetNewArray(&newArray_JetData);
      instance.SetDelete(&delete_JetData);
      instance.SetDeleteArray(&deleteArray_JetData);
      instance.SetDestructor(&destruct_JetData);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::JetData*)
   {
      return GenerateInitInstanceLocal((::JetData*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::JetData*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_JetEvent(void *p = 0);
   static void *newArray_JetEvent(Long_t size, void *p);
   static void delete_JetEvent(void *p);
   static void deleteArray_JetEvent(void *p);
   static void destruct_JetEvent(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::JetEvent*)
   {
      ::JetEvent *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::JetEvent >(0);
      static ::ROOT::TGenericClassInfo 
         instance("JetEvent", ::JetEvent::Class_Version(), "JetEvent.h", 106,
                  typeid(::JetEvent), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::JetEvent::Dictionary, isa_proxy, 4,
                  sizeof(::JetEvent) );
      instance.SetNew(&new_JetEvent);
      instance.SetNewArray(&newArray_JetEvent);
      instance.SetDelete(&delete_JetEvent);
      instance.SetDeleteArray(&deleteArray_JetEvent);
      instance.SetDestructor(&destruct_JetEvent);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::JetEvent*)
   {
      return GenerateInitInstanceLocal((::JetEvent*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::JetEvent*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr JetData::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *JetData::Class_Name()
{
   return "JetData";
}

//______________________________________________________________________________
const char *JetData::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::JetData*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int JetData::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::JetData*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *JetData::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::JetData*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *JetData::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::JetData*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr JetEvent::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *JetEvent::Class_Name()
{
   return "JetEvent";
}

//______________________________________________________________________________
const char *JetEvent::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::JetEvent*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int JetEvent::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::JetEvent*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *JetEvent::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::JetEvent*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *JetEvent::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::JetEvent*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void JetData::Streamer(TBuffer &R__b)
{
   // Stream an object of class JetData.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(JetData::Class(),this);
   } else {
      R__b.WriteClassBuffer(JetData::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_JetData(void *p) {
      return  p ? new(p) ::JetData : new ::JetData;
   }
   static void *newArray_JetData(Long_t nElements, void *p) {
      return p ? new(p) ::JetData[nElements] : new ::JetData[nElements];
   }
   // Wrapper around operator delete
   static void delete_JetData(void *p) {
      delete ((::JetData*)p);
   }
   static void deleteArray_JetData(void *p) {
      delete [] ((::JetData*)p);
   }
   static void destruct_JetData(void *p) {
      typedef ::JetData current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::JetData

//______________________________________________________________________________
void JetEvent::Streamer(TBuffer &R__b)
{
   // Stream an object of class JetEvent.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(JetEvent::Class(),this);
   } else {
      R__b.WriteClassBuffer(JetEvent::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_JetEvent(void *p) {
      return  p ? new(p) ::JetEvent : new ::JetEvent;
   }
   static void *newArray_JetEvent(Long_t nElements, void *p) {
      return p ? new(p) ::JetEvent[nElements] : new ::JetEvent[nElements];
   }
   // Wrapper around operator delete
   static void delete_JetEvent(void *p) {
      delete ((::JetEvent*)p);
   }
   static void deleteArray_JetEvent(void *p) {
      delete [] ((::JetEvent*)p);
   }
   static void destruct_JetEvent(void *p) {
      typedef ::JetEvent current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::JetEvent

namespace ROOT {
   static TClass *vectorlEintgR_Dictionary();
   static void vectorlEintgR_TClassManip(TClass*);
   static void *new_vectorlEintgR(void *p = 0);
   static void *newArray_vectorlEintgR(Long_t size, void *p);
   static void delete_vectorlEintgR(void *p);
   static void deleteArray_vectorlEintgR(void *p);
   static void destruct_vectorlEintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<int>*)
   {
      vector<int> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<int>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<int>", -2, "vector", 214,
                  typeid(vector<int>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEintgR_Dictionary, isa_proxy, 4,
                  sizeof(vector<int>) );
      instance.SetNew(&new_vectorlEintgR);
      instance.SetNewArray(&newArray_vectorlEintgR);
      instance.SetDelete(&delete_vectorlEintgR);
      instance.SetDeleteArray(&deleteArray_vectorlEintgR);
      instance.SetDestructor(&destruct_vectorlEintgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<int> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const vector<int>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<int>*)0x0)->GetClass();
      vectorlEintgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEintgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<int> : new vector<int>;
   }
   static void *newArray_vectorlEintgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<int>[nElements] : new vector<int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEintgR(void *p) {
      delete ((vector<int>*)p);
   }
   static void deleteArray_vectorlEintgR(void *p) {
      delete [] ((vector<int>*)p);
   }
   static void destruct_vectorlEintgR(void *p) {
      typedef vector<int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<int>

namespace {
  void TriggerDictionaryInitialization_JetDct_Impl() {
    static const char* headers[] = {
"JetEvent.h",
0
    };
    static const char* includePaths[] = {
"./include",
"/work/hsiikone/install/rootbuild/include",
"/work/hsiikone/development/gen_handle/events/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "JetDct dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
namespace std{template <typename _Tp> class __attribute__((annotate("$clingAutoload$bits/allocator.h")))  __attribute__((annotate("$clingAutoload$string")))  allocator;
}
class __attribute__((annotate("$clingAutoload$JetEvent.h")))  JetData;
class __attribute__((annotate("$clingAutoload$JetEvent.h")))  JetEvent;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "JetDct dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "JetEvent.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"JetData", payloadCode, "@",
"JetEvent", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("JetDct",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_JetDct_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_JetDct_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_JetDct() {
  TriggerDictionaryInitialization_JetDct_Impl();
}
