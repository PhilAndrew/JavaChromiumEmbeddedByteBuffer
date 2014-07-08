// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef CEF_TESTS_CEFCLIENT_CLIENT_HANDLER_H_
#define CEF_TESTS_CEFCLIENT_CLIENT_HANDLER_H_
#pragma once

#include <set>
#include <jni.h>
#include "include/cef_client.h"
#include "message_router_handler.h"

// ClientHandler implementation.
class ClientHandler : public CefClient {
 public:
  ClientHandler(JNIEnv* env, jobject handler);
  virtual ~ClientHandler();

  // CefClient methods
  virtual CefRefPtr<CefContextMenuHandler> GetContextMenuHandler() OVERRIDE;
  virtual CefRefPtr<CefDialogHandler> GetDialogHandler() OVERRIDE;
  virtual CefRefPtr<CefDisplayHandler> GetDisplayHandler() OVERRIDE;
  virtual CefRefPtr<CefDownloadHandler> GetDownloadHandler() OVERRIDE;
  virtual CefRefPtr<CefDragHandler> GetDragHandler() OVERRIDE;
  virtual CefRefPtr<CefFocusHandler> GetFocusHandler() OVERRIDE;
  virtual CefRefPtr<CefGeolocationHandler> GetGeolocationHandler() OVERRIDE;
  virtual CefRefPtr<CefJSDialogHandler> GetJSDialogHandler() OVERRIDE;
  virtual CefRefPtr<CefKeyboardHandler> GetKeyboardHandler() OVERRIDE;
  virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() OVERRIDE;
  virtual CefRefPtr<CefLoadHandler> GetLoadHandler() OVERRIDE;
  virtual CefRefPtr<CefRenderHandler> GetRenderHandler() OVERRIDE;
  virtual CefRefPtr<CefRequestHandler> GetRequestHandler() OVERRIDE;

  virtual bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
                                        CefProcessId source_process,
                                        CefRefPtr<CefProcessMessage> message)
                                        OVERRIDE;

  // Methods to deal with message router bindings
  void AddMessageRouter(JNIEnv* env, jobject jmessageRouter);
  void RemoveMessageRouter(JNIEnv* env, jobject jmessageRouter);

  // Methods to set and remove a browser ref.
  void OnAfterCreated();
  void OnBeforeClose(CefRefPtr<CefBrowser> browser);
  void OnBeforeBrowse(CefRefPtr<CefBrowser> browser,
                      CefRefPtr<CefFrame> frame);
  void OnRenderProcessTerminated(CefRefPtr<CefBrowser> browser);

  jobject getBrowser(CefRefPtr<CefBrowser> browser);

 protected:
  jobject jhandler_;

  // The child browser window.
  std::map<int,jobject> browserMap_;

  std::set<CefRefPtr<CefMessageRouterBrowserSide> > message_router_;

  // Include the default reference counting implementation.
  IMPLEMENT_REFCOUNTING(ClientHandler);
  // Include the default locking implementation.
  IMPLEMENT_LOCKING(ClientHandler);
};

#endif  // CEF_TESTS_CEFCLIENT_CLIENT_HANDLER_H_
