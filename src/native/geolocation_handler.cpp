// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "geolocation_handler.h"
#include "client_handler.h"

#include "jni_util.h"
#include "util.h"

GeolocationHandler::GeolocationHandler(JNIEnv* env, jobject handler) {
  jhandler_ = env->NewGlobalRef(handler);
}

GeolocationHandler::~GeolocationHandler() {
  JNIEnv* env = GetJNIEnv();
  env->DeleteGlobalRef(jhandler_);
}

void GeolocationHandler::OnRequestGeolocationPermission(
      CefRefPtr<CefBrowser> browser,
      const CefString& requesting_url,
      int request_id,
      CefRefPtr<CefGeolocationCallback> callback) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return;
  jobject jcallback = NewJNIObject(env, "org/cef/callback/CefGeolocationCallback_N");
  if (!jcallback)
    return;
  SetCefForJNIObject(env, jcallback, callback.get(), "CefGeolocationCallback");
  JNI_CALL_VOID_METHOD(env, jhandler_,
                       "onRequestGeolocationPermission",
                       "(Lorg/cef/browser/CefBrowser;Ljava/lang/String;ILorg/cef/callback/CefGeolocationCallback;)V",
                       GetJNIBrowser(browser),
                       NewJNIString(env, requesting_url),
                       request_id,
                       jcallback);
}

void GeolocationHandler::OnCancelGeolocationPermission(
      CefRefPtr<CefBrowser> browser,
      const CefString& requesting_url,
      int request_id) {
  JNIEnv* env = GetJNIEnv();
  if (!env)
    return;
  JNI_CALL_VOID_METHOD(env, jhandler_,
                       "onCancelGeolocationPermission",
                       "(Lorg/cef/browser/CefBrowser;Ljava/lang/String;I)V",
                       GetJNIBrowser(browser),
                       NewJNIString(env, requesting_url),
                       request_id);
}
