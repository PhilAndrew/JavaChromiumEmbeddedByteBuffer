// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef CEF_TESTS_CEFCLIENT_JNI_UTIL_H_
#define CEF_TESTS_CEFCLIENT_JNI_UTIL_H_

#include <jni.h>
#include <vector>
#include "include/cef_base.h"
#include "include/cef_browser.h"
#include "include/wrapper/cef_message_router.h"
#include "util.h"

// Set the global JVM reference.
void SetJVM(JavaVM* jvm);

// Retrieve the JNIEnv for the current thread.
JNIEnv* GetJNIEnv();

// Determines whether the current thread is already attached to the VM,
// and tells the caller if it needs to later DetachCurrentThread.
//
// INSTEAD OF USING THIS FUNCTION DIRECTLY, USE THE HELPER MACRO
// BEGIN_ENV(e) INSTEAD.
jint GetJNIEnv(JNIEnv **env, bool *mustDetach);

// Detaches the current thread from the VM.
//
// INSTEAD OF USING THIS FUNCTION DIRECTLY; USE THE HELPER MACRO
// END_ENV(e) INSTEAD.
void DetachFromThread(bool *mustDetach);

// Helper macros to bind and release the JNI environment
// to other threads than the JNI function was called on.
#define BEGIN_ENV(e) \
  JNIEnv *e = NULL; \
  bool __shouldDetach = false; \
  if (GetJNIEnv(&e, &__shouldDetach) == JNI_OK && e != NULL) { \

#define END_ENV(e) \
    DetachFromThread(&__shouldDetach); \
  } \

# if defined(OS_MACOSX)
// Required for onscreen rendering ability on Mac OS X.
void AddLayerToComponent(jobject parent, JNIEnv *env, CefWindowHandle child);
#elif defined(OS_WIN)
HWND GetHwndOfCanvas(jobject canvas, JNIEnv *env);
#endif

// Create a new JNI object and call the default constructor.
jobject NewJNIObject(JNIEnv* env, jclass cls);
jobject NewJNIObject(JNIEnv* env, const char* class_name);
jobject NewJNIObject(JNIEnv* env, const char* class_name, const char* sig, ...);

// Create a new primitive reference
jobject NewJNIBoolRef(JNIEnv* env, bool initValue);
jobject NewJNIIntRef(JNIEnv* env, int initValue);
jobject NewJNIStringRef(JNIEnv* env, const CefString& initValue);

// Retrieve primitive reference values
bool GetJNIBoolRef(JNIEnv* env, jobject jboolRef);
int GetJNIIntRef(JNIEnv* env, jobject jintRef);
CefString GetJNIStringRef(JNIEnv* env, jobject jstringRef);

// Set primitive reference values
void SetJNIBoolRef(JNIEnv* env, jobject jboolRef, bool boolValue);
void SetJNIIntRef(JNIEnv* env, jobject jintRef, int intValue);
void SetJNIStringRef(JNIEnv* env, jobject jstringRef, const CefString& initValue);

// Create a new date type
jobject NewJNIDate(JNIEnv* env, const CefTime& time);

// Create a new cookie object
jobject NewJNICookie(JNIEnv* env, const CefCookie& cookie);

// Retrieve a Cookie value
CefCookie GetJNICookie(JNIEnv* env, jobject jcookie);

// Retrieve a String value.
CefString GetJNIString(JNIEnv* env, jstring jstr);

// Retrieve a String array.
void GetJNIStringArray(JNIEnv* env, jobjectArray jarray,
                       std::vector<CefString>& vals);

CefMessageRouterConfig GetJNIMessageRouterConfig(JNIEnv* env,
                                                 jobject jConfig);
CefMessageRouterConfig GetJNIMessageRouterConfigFromRouter(JNIEnv* env,
                                                           jobject jRouter);

// Create a new JNI error code.
jobject NewJNIErrorCode(JNIEnv* env, cef_errorcode_t errorCode);

// Create a new String value.
jstring NewJNIString(JNIEnv* env, const CefString& str);

// Create a new array of String values.
jobjectArray NewJNIStringArray(JNIEnv* env,
                               const std::vector<CefString>& vals);

jobject NewJNIStringVector(JNIEnv* env,
                           const std::vector<CefString>& vals);

void AddJNIStringToVector(JNIEnv* env, jobject jvector, 
                          const CefString &str);

void GetJNIStringVector(JNIEnv* env, jobject jvector,
                        std::vector<CefString>& vals);

bool GetJNIFieldString(JNIEnv* env, jclass cls, jobject obj,
                       const char* field_name, CefString *value);

bool GetJNIFieldDate(JNIEnv* env, jclass cls, jobject obj,
                     const char* field_name, CefTime* value);

bool GetJNIFieldBoolean(JNIEnv* env, jclass cls, jobject obj,
                        const char* field_name, int* value);

// Retrieve the int value stored in the |field_name| field of |cls|.
bool GetJNIFieldInt(JNIEnv* env, jclass cls, jobject obj,
                    const char* field_name, int* value);

// Set the int value stored in the |field_name| field of |cls|.
bool SetJNIFieldInt(JNIEnv* env, jclass cls, jobject obj,
                    const char* field_name, int value);

// Retrieve the static int value stored in the |field_name| field of |cls|.
bool GetJNIFieldStaticInt(JNIEnv* env, jclass cls,
                          const char* field_name, int* value);

// Call a JNI method that returns an int and accepts no arguments.
bool CallJNIMethodI_V(JNIEnv* env, jclass cls, jobject obj,
                      const char* method_name, int* value);

// Call a JNI method that returns a char and accepts no arguments.
bool CallJNIMethodC_V(JNIEnv* env, jclass cls, jobject obj,
                      const char* method_name, char* value);

// Retrieve the CefRect equivalent of a java.awt.Rectangle.
CefRect GetJNIRect(JNIEnv* env, jobject obj);

// Create a new java.awt.Rectangle.
jobject NewJNIRect(JNIEnv* env, const CefRect& rect);

// create a new array of java.awt.Rectangle.
jobjectArray NewJNIRectArray(JNIEnv* env,
                             const std::vector<CefRect>& vals);

// Retrieve the value of a java.awt.Point.
bool GetJNIPoint(JNIEnv* env, jobject obj, int* x, int* y);

// Create a new java.awt.Point.
jobject NewJNIPoint(JNIEnv* env, int x, int y);

// Get java browser counterpart
jobject GetJNIBrowser(CefRefPtr<CefBrowser>);
jobjectArray GetAllJNIBrowser(JNIEnv* env, jobject jclientHandler);

jobject GetJNIEnumValue(JNIEnv* env, const char* class_name, const char* enum_valname);

bool IsJNIEnumValue(JNIEnv* env, jobject jenum, const char* class_name, const char* enum_valname);

// Helper macro for handling jni enum values in a switch statement
#define JNI_CASE(env, cls, type, result) \
  case type: \
    result = GetJNIEnumValue(env,cls,#type); \
    break;

// Helper macros for defining and retrieving static ints.
#define JNI_STATIC(name) _static_##name

#define JNI_STATIC_DEFINE_INT(env, cls, name) \
  JNI_STATIC_DEFINE_INT_RV(env, cls, name, );

#define JNI_STATIC_DEFINE_INT_RV(env, cls, name, rv) \
  static int JNI_STATIC(name) = -1; \
  if (JNI_STATIC(name) == -1 && \
      !GetJNIFieldStaticInt(env, cls, #name, &JNI_STATIC(name))) \
    return rv;

// Helper macros to call a method on the java side
#define JNI_CALL_METHOD(env, obj, method, sig, type, storeIn, ...) { \
  if (env) { \
    jclass cls = env->GetObjectClass(obj); \
    jmethodID methodId = env->GetMethodID(cls, method, sig); \
    if (methodId != NULL) { \
      storeIn = env->Call ## type ## Method(obj, methodId, ##__VA_ARGS__); \
    } \
    if (env->ExceptionOccurred()) { \
      env->ExceptionDescribe(); \
      env->ExceptionClear(); \
    } \
  } \
}

#define JNI_CALL_VOID_METHOD(env, obj, method, sig, ...) { \
  if (env) {\
    jclass cls = env->GetObjectClass(obj); \
    jmethodID methodId = env->GetMethodID(cls, method, sig); \
    if (methodId != NULL) { \
      env->CallVoidMethod(obj, methodId, ##__VA_ARGS__); \
    } \
    if (env->ExceptionOccurred()) { \
      env->ExceptionDescribe(); \
      env->ExceptionClear(); \
    } \
  } \
}

#define JNI_GET_BROWSER_OR_RETURN(env, obj, ...) \
  GetCefFromJNIObject<CefBrowser>(env, obj, "CefBrowser"); \
  if (!browser.get()) \
    return __VA_ARGS__;

// Set the CEF base object for an existing JNI object. A reference will be
// added to the base object. If a previous base object existed a reference
// will be removed from that object.
template <class T>
bool SetCefForJNIObject(JNIEnv* env, jobject obj, T* base, const char* varName) {
  jstring identifer = env->NewStringUTF(varName);
  jlong previousValue = 0;
  if (!obj)
    return false;
  JNI_CALL_METHOD(env, obj, "getNativeRef", "(Ljava/lang/String;)J", Long, previousValue, identifer);

  T* oldbase = reinterpret_cast<T*>(previousValue);
  if(oldbase) {
    // Remove a reference from the previous base object.
    oldbase->Release();
  }

  JNI_CALL_VOID_METHOD(env, obj, "setNativeRef", "(Ljava/lang/String;J)V", identifer, (jlong)base);
  if(base) {
    // Add a reference to the new base object.
    base->AddRef();
  }
  return true;
}

// Retrieve the CEF base object from an existing JNI object.
template <class T>
T* GetCefFromJNIObject(JNIEnv* env, jobject obj, const char* varName) {
  jstring identifer = env->NewStringUTF(varName);
  jlong previousValue = 0;
  if (obj != NULL)
    JNI_CALL_METHOD(env, obj, "getNativeRef", "(Ljava/lang/String;)J", Long, previousValue, identifer);

  if (previousValue != 0)
    return reinterpret_cast<T*>(previousValue);
  return NULL;
}

#endif // CEF_TESTS_CEFCLIENT_JNI_UTIL_H_
