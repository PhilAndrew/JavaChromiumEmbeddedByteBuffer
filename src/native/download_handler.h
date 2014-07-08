// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef CEF_TESTS_CEFCLIENT_DOWNLOAD_HANDLER_H_
#define CEF_TESTS_CEFCLIENT_DOWNLOAD_HANDLER_H_
#pragma once

#include <jni.h>
#include "include/cef_download_handler.h"

// DownloadHandler implementation.
class DownloadHandler : public CefDownloadHandler {
 public:
  DownloadHandler(JNIEnv* env, jobject handler);
  virtual ~DownloadHandler();

  // CefDownloadHandler methods
  virtual void OnBeforeDownload(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefDownloadItem> download_item,
    const CefString& suggested_name,
    CefRefPtr<CefBeforeDownloadCallback> callback) OVERRIDE;

  virtual void OnDownloadUpdated(
      CefRefPtr<CefBrowser> browser,
      CefRefPtr<CefDownloadItem> download_item,
      CefRefPtr<CefDownloadItemCallback> callback) OVERRIDE;

 protected:
  jobject jhandler_;

  // Include the default reference counting implementation.
  IMPLEMENT_REFCOUNTING(DownloadHandler);
  // Include the default locking implementation.
  IMPLEMENT_LOCKING(DownloadHandler);
};

#endif  // CEF_TESTS_CEFCLIENT_DOWNLOAD_HANDLER_H_
