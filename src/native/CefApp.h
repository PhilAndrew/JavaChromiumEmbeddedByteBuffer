/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class org_cef_CefApp */

#ifndef _Included_org_cef_CefApp
#define _Included_org_cef_CefApp
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     org_cef_CefApp
 * Method:    N_Initialize
 * Signature: (Ljava/lang/String;Lorg/cef/handler/CefAppHandler;)Z
 */
JNIEXPORT jboolean JNICALL Java_org_cef_CefApp_N_1Initialize
  (JNIEnv *, jobject, jstring, jobject);

/*
 * Class:     org_cef_CefApp
 * Method:    N_Shutdown
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_cef_CefApp_N_1Shutdown
  (JNIEnv *, jobject);

/*
 * Class:     org_cef_CefApp
 * Method:    N_DoMessageLoopWork
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_cef_CefApp_N_1DoMessageLoopWork
  (JNIEnv *, jobject);

/*
 * Class:     org_cef_CefApp
 * Method:    N_RegisterSchemeHandlerFactory
 * Signature: (Ljava/lang/String;Ljava/lang/String;Lorg/cef/callback/CefSchemeHandlerFactory;)Z
 */
JNIEXPORT jboolean JNICALL Java_org_cef_CefApp_N_1RegisterSchemeHandlerFactory
  (JNIEnv *, jobject, jstring, jstring, jobject);

/*
 * Class:     org_cef_CefApp
 * Method:    N_ClearSchemeHandlerFactories
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_org_cef_CefApp_N_1ClearSchemeHandlerFactories
  (JNIEnv *, jobject);

#ifdef __cplusplus
}
#endif
#endif
