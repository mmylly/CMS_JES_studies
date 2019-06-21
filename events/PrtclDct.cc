// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME PrtclDct

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
#include "PrtclEvent.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void *new_PrtclData(void *p = 0);
   static void *newArray_PrtclData(Long_t size, void *p);
   static void delete_PrtclData(void *p);
   static void deleteArray_PrtclData(void *p);
   static void destruct_PrtclData(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::PrtclData*)
   {
      ::PrtclData *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::PrtclData >(0);
      static ::ROOT::TGenericClassInfo 
         instance("PrtclData", ::PrtclData::Class_Version(), "PrtclEvent.h", 48,
                  typeid(::PrtclData), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::PrtclData::Dictionary, isa_proxy, 4,
                  sizeof(::PrtclData) );
      instance.SetNew(&new_PrtclData);
      instance.SetNewArray(&newArray_PrtclData);
      instance.SetDelete(&delete_PrtclData);
      instance.SetDeleteArray(&deleteArray_PrtclData);
      instance.SetDestructor(&destruct_PrtclData);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::PrtclData*)
   {
      return GenerateInitInstanceLocal((::PrtclData*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::PrtclData*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_PrtclEvent(void *p = 0);
   static void *newArray_PrtclEvent(Long_t size, void *p);
   static void delete_PrtclEvent(void *p);
   static void deleteArray_PrtclEvent(void *p);
   static void destruct_PrtclEvent(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::PrtclEvent*)
   {
      ::PrtclEvent *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::PrtclEvent >(0);
      static ::ROOT::TGenericClassInfo 
         instance("PrtclEvent", ::PrtclEvent::Class_Version(), "PrtclEvent.h", 80,
                  typeid(::PrtclEvent), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::PrtclEvent::Dictionary, isa_proxy, 4,
                  sizeof(::PrtclEvent) );
      instance.SetNew(&new_PrtclEvent);
      instance.SetNewArray(&newArray_PrtclEvent);
      instance.SetDelete(&delete_PrtclEvent);
      instance.SetDeleteArray(&deleteArray_PrtclEvent);
      instance.SetDestructor(&destruct_PrtclEvent);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::PrtclEvent*)
   {
      return GenerateInitInstanceLocal((::PrtclEvent*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::PrtclEvent*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr PrtclData::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *PrtclData::Class_Name()
{
   return "PrtclData";
}

//______________________________________________________________________________
const char *PrtclData::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PrtclData*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int PrtclData::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PrtclData*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *PrtclData::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PrtclData*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *PrtclData::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PrtclData*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr PrtclEvent::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *PrtclEvent::Class_Name()
{
   return "PrtclEvent";
}

//______________________________________________________________________________
const char *PrtclEvent::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PrtclEvent*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int PrtclEvent::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PrtclEvent*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *PrtclEvent::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PrtclEvent*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *PrtclEvent::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PrtclEvent*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void PrtclData::Streamer(TBuffer &R__b)
{
   // Stream an object of class PrtclData.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(PrtclData::Class(),this);
   } else {
      R__b.WriteClassBuffer(PrtclData::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_PrtclData(void *p) {
      return  p ? new(p) ::PrtclData : new ::PrtclData;
   }
   static void *newArray_PrtclData(Long_t nElements, void *p) {
      return p ? new(p) ::PrtclData[nElements] : new ::PrtclData[nElements];
   }
   // Wrapper around operator delete
   static void delete_PrtclData(void *p) {
      delete ((::PrtclData*)p);
   }
   static void deleteArray_PrtclData(void *p) {
      delete [] ((::PrtclData*)p);
   }
   static void destruct_PrtclData(void *p) {
      typedef ::PrtclData current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::PrtclData

//______________________________________________________________________________
void PrtclEvent::Streamer(TBuffer &R__b)
{
   // Stream an object of class PrtclEvent.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(PrtclEvent::Class(),this);
   } else {
      R__b.WriteClassBuffer(PrtclEvent::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_PrtclEvent(void *p) {
      return  p ? new(p) ::PrtclEvent : new ::PrtclEvent;
   }
   static void *newArray_PrtclEvent(Long_t nElements, void *p) {
      return p ? new(p) ::PrtclEvent[nElements] : new ::PrtclEvent[nElements];
   }
   // Wrapper around operator delete
   static void delete_PrtclEvent(void *p) {
      delete ((::PrtclEvent*)p);
   }
   static void deleteArray_PrtclEvent(void *p) {
      delete [] ((::PrtclEvent*)p);
   }
   static void destruct_PrtclEvent(void *p) {
      typedef ::PrtclEvent current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::PrtclEvent

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
  void TriggerDictionaryInitialization_PrtclDct_Impl() {
    static const char* headers[] = {
"PrtclEvent.h",
0
    };
    static const char* includePaths[] = {
"./include",
"/work/hsiikone/install/rootbuild/include",
"/work/hsiikone/development/gen_handle/events/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "PrtclDct dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
namespace std{template <typename _Tp> class __attribute__((annotate("$clingAutoload$bits/allocator.h")))  __attribute__((annotate("$clingAutoload$string")))  allocator;
}
class __attribute__((annotate("$clingAutoload$PrtclEvent.h")))  PrtclData;
class __attribute__((annotate("$clingAutoload$PrtclEvent.h")))  PrtclEvent;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "PrtclDct dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "PrtclEvent.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"PrtclData", payloadCode, "@",
"PrtclEvent", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("PrtclDct",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_PrtclDct_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_PrtclDct_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_PrtclDct() {
  TriggerDictionaryInitialization_PrtclDct_Impl();
}
