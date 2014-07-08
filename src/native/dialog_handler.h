// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef CEF_TESTS_CEFCLIENT_DIALOG_HANDLER_H_
#define CEF_TESTS_CEFCLIENT_DIALOG_HANDLER_H_
#pragma once

#include <jni.h>
#include "include/cef_dialog_handler.h"

// DialogHandler implementation.
class DialogHandler : public CefDialogHandler {
 public:
  DialogHandler(JNIEnv* env, jobject handler);
  virtual ~DialogHandler();

  // CefDialogHandler methods
    virtual bool OnFileDialog(CefRefPtr<CefBrowser> browser,
                            FileDialogMode mode,
                            const CefString& title,
                            const CefString& default_file_name,
                            const std::vector<CefString>& accept_types,
                            CefRefPtr<CefFileDialogCallback> callback) OVERRIDE;

 protected:
  jobject jhandler_;

  // Include the default reference counting implementation.
  IMPLEMENT_REFCOUNTING(DialogHandler);
};

#endif  // CEF_TESTS_CEFCLIENT_DIALOG_HANDLER_H_
