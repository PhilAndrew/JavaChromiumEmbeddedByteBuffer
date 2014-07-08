// Copyright (c) 2014 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef CEF_TESTS_CEFCLIENT_CONTEXT_MENU_HANDLER_H_
#define CEF_TESTS_CEFCLIENT_CONTEXT_MENU_HANDLER_H_
#pragma once

#include <jni.h>
#include "include/cef_context_menu_handler.h"

// ContextMenuHandler implementation.
class ContextMenuHandler : public CefContextMenuHandler {
 public:
  ContextMenuHandler(JNIEnv* env, jobject handler);
  virtual ~ContextMenuHandler();

  // CefContextMenuHandler methods
  virtual void OnBeforeContextMenu(CefRefPtr<CefBrowser> browser,
                                   CefRefPtr<CefFrame> frame,
                                   CefRefPtr<CefContextMenuParams> params,
                                   CefRefPtr<CefMenuModel> model) OVERRIDE;

  virtual bool OnContextMenuCommand(CefRefPtr<CefBrowser> browser,
                                    CefRefPtr<CefFrame> frame,
                                    CefRefPtr<CefContextMenuParams> params,
                                    int command_id,
                                    EventFlags event_flags) OVERRIDE;

  virtual void OnContextMenuDismissed(CefRefPtr<CefBrowser> browser,
                                      CefRefPtr<CefFrame> frame) OVERRIDE;
 protected:
  jobject jhandler_;

  // Include the default reference counting implementation.
  IMPLEMENT_REFCOUNTING(ContextMenuHandler);
  // Include the default locking implementation.
  IMPLEMENT_LOCKING(ContextMenuHandler);
};

#endif  // CEF_TESTS_CEFCLIENT_CONTEXT_MENU_HANDLER_H_
