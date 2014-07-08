// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

package tests.detailed;

import java.awt.BorderLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.io.File;

import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.Timer;

import org.cef.CefApp;
import org.cef.CefClient;
import org.cef.OS;
import org.cef.browser.CefBrowser;
import org.cef.browser.CefMessageRouter;
import org.cef.browser.CefRequestContext;
import org.cef.handler.CefDisplayHandlerAdapter;
import org.cef.handler.CefLoadHandlerAdapter;
import org.cef.handler.CefRequestContextHandler;
import org.cef.network.CefCookieManager;

import tests.detailed.dialog.DownloadDialog;
import tests.detailed.handler.AppHandler;
import tests.detailed.handler.ContextMenuHandler;
import tests.detailed.handler.DragHandler;
import tests.detailed.handler.GeolocationHandler;
import tests.detailed.handler.JSDialogHandler;
import tests.detailed.handler.KeyboardHandler;
import tests.detailed.handler.MessageRouterHandler;
import tests.detailed.handler.MessageRouterHandlerEx;
import tests.detailed.handler.RequestHandler;
import tests.detailed.ui.ControlPanel;
import tests.detailed.ui.MenuBar;
import tests.detailed.ui.StatusPanel;

public class MainFrame extends JFrame {
  private static final long serialVersionUID = -2295538706810864538L;
  public static void main(String [] args) {
    // OSR mode is enabled by default on Linux.
    // and disabled by default on Windows and Mac OS X.
    boolean osrEnabledArg = OS.isLinux();
    String cookiePath = null;
    for (String arg : args) {
      arg = arg.toLowerCase();
      if (!OS.isLinux() && arg.equals("--off-screen-rendering-enabled")) {
        osrEnabledArg = true;
      } else if(arg.startsWith("--cookie-path=")) {
        cookiePath = arg.substring("--cookie-path=".length());
        File testPath = new File(cookiePath);
        if (!testPath.isDirectory() || !testPath.canWrite()) {
          System.out.println("Can't use " + cookiePath + " as cookie directory. Check if it exists and if it is writable");
          cookiePath = null;
        } else {
          System.out.println("Storing cookies in " + cookiePath);
        }
      }
    }

    System.out.println("Offscreen rendering " + (osrEnabledArg ? "enabled" : "disabled"));

    // MainFrame keeps all the knowledge to display the embedded browser
    // frame.
    final MainFrame frame = new MainFrame(osrEnabledArg, cookiePath, args); 
    frame.addWindowListener(new WindowAdapter() {
      @Override
      public void windowClosing(WindowEvent e) {
        frame.dispose();
        CefApp.getInstance().dispose();

        new Timer(3000, new ActionListener() {
          @Override
          public void actionPerformed(ActionEvent e) {
            System.exit(0);
          }
        }).start();
      }
    });

    frame.setSize(800, 600);
    frame.setVisible(true);
  }

  private final CefClient client_;
  private String errorMsg_ = "";
  private final CefBrowser browser_;
  private ControlPanel control_pane_;
  private StatusPanel status_panel_;
  private final CefCookieManager cookieManager_;

  public MainFrame(boolean osrEnabled, String cookiePath, String [] args) {

    // 1) CefApp is the entry point for JCEF. You can pass
    //    application arguments to it, if you want to handle any
    //    chromium or CEF related switches/attributes in
    //    the native world.
    CefApp myApp = CefApp.getInstance(args);

    //    We're registering our own AppHandler because we want to
    //    add an own schemes (search:// and client://) and its corresponding
    //    protocol handlers. So if you enter "search:something on the web", your
    //    search request "something on the web" is forwarded to www.google.com
    CefApp.addAppHandler(new AppHandler(args));

    //    By calling the method createClient() the native part
    //    of JCEF/CEF will be initialized and an  instance of
    //    CefClient will be created. You can create one to many
    //    instances of CefClient.
    client_ = myApp.createClient();

    // 2) You have the ability to pass different handlers to your
    //    instance of CefClient. Each handler is responsible to
    //    deal with different informations (e.g. keyboard input).
    //
    //    For each handler (with more than one method) adapter
    //    classes exists. So you don't need to override methods
    //    you're not interested in.
    DownloadDialog downloadDialog = new DownloadDialog(this);
    client_.addContextMenuHandler(new ContextMenuHandler(this));
    client_.addDownloadHandler(downloadDialog);
    client_.addDragHandler(new DragHandler());
    client_.addGeolocationHandler(new GeolocationHandler(this));
    client_.addJSDialogHandler(new JSDialogHandler());
    client_.addKeyboardHandler(new KeyboardHandler());
    client_.addRequestHandler(new RequestHandler(this));

    //    Beside the normal handler instances, we're registering a MessageRouter
    //    as well. That gives us the opportunity to reply to JavaScript method
    //    calls (JavaScript binding). We're using the default configuration, so
    //    that the JavaScript binding methods "cefQuery" and "cefQueryCancel"
    //    are used.
    CefMessageRouter msgRouter = CefMessageRouter.create();
    msgRouter.addHandler(new MessageRouterHandler(), true);
    msgRouter.addHandler(new MessageRouterHandlerEx(client_), false);
    client_.addMessageRouter(msgRouter);

    // 2.1) We're overriding CefDisplayHandler as nested anonymous class
    //      to update our address-field, the title of the panel as well
    //      as for updating the status-bar on the bottom of the browser
    client_.addDisplayHandler(new CefDisplayHandlerAdapter() {
      @Override
      public void onAddressChange(CefBrowser browser, String url) {
        control_pane_.setAddress(browser, url);
      }
      @Override
      public void onTitleChange(CefBrowser browser, String title) {
        setTitle(title);
      }
      @Override
      public void onStatusMessage(CefBrowser browser, String value) {
        status_panel_.setStatusText(value);
      }
    });

    // 2.2) To disable/enable navigation buttons and to display a prgress bar
    //      which indicates the load state of our website, we're overloading
    //      the CefLoadHandler as nested anonymous class. Beside this, the 
    //      load handler is responsible to deal with (load) errors as well.
    //      For example if you navigate to a URL which does not exist, the 
    //      browser will show up an error message.
    client_.addLoadHandler(new CefLoadHandlerAdapter() {
      @Override
      public void onLoadingStateChange(CefBrowser browser,
                                       boolean isLoading,
                                       boolean canGoBack,
                                       boolean canGoForward) {
        control_pane_.update(browser, isLoading, canGoBack, canGoForward);
        status_panel_.setIsInProgress(isLoading);

        if (!isLoading && !errorMsg_.isEmpty()) {
          browser.loadString(errorMsg_, control_pane_.getAddress());
          errorMsg_ = "";
        }
      }

      @Override
      public void onLoadError(CefBrowser browser,
                              int frameIdentifer,
                              ErrorCode errorCode,
                              String errorText,
                              String failedUrl) {
        if (errorCode != ErrorCode.ERR_NONE && errorCode != ErrorCode.ERR_ABORTED) {
          errorMsg_  = "<html><head>";
          errorMsg_ += "<title>Error while loading</title>";
          errorMsg_ += "</head><body>";
          errorMsg_ += "<h1>" + errorCode + "</h1>";
          errorMsg_ += "<h3>Failed to load " + failedUrl + "</h3>";
          errorMsg_ += "<p>" + (errorText == null ? "" : errorText) + "</p>";
          errorMsg_ += "</body></html>";
          browser.stopLoad();
        }
      }
    });

    // 3) Before we can display any content, we require an instance of
    //    CefBrowser itself by calling createBrowser() on the CefClient.
    //    You can create one to many browser instances per CefClient.
    //
    //    If the user has specified the application parameter "--cookie-path="
    //    we provide our own cookie manager which persists cookies in a directory.
    CefRequestContext requestContext = null;
    if (cookiePath != null) {
      cookieManager_ = CefCookieManager.createManager(cookiePath, false);
      requestContext = CefRequestContext.createContext(new CefRequestContextHandler() {
        @Override
        public CefCookieManager getCookieManager() {
          return cookieManager_;
        }
      });
    } else {
      cookieManager_ = CefCookieManager.getGlobalManager();
    }
    browser_ = client_.createBrowser("http://www.google.com",
                                     osrEnabled,
                                     false,
                                     requestContext);

    //    Last but not least we're setting up the UI for this example implementation.
    getContentPane().add(createContentPanel(), BorderLayout.CENTER);
    MenuBar menuBar = new MenuBar(this,
                                  browser_,
                                  control_pane_,
                                  downloadDialog,
                                  cookieManager_);

    menuBar.addBookmark("Binding Test", "client://tests/binding_test.html");
    menuBar.addBookmark("Binding Test 2", "client://tests/binding_test2.html");
    menuBar.addBookmark("Download Test", "http://cefbuilds.com");
    menuBar.addBookmark("Geolocation Test","http://slides.html5rocks.com/#geolocation");
    menuBar.addBookmark("Login Test (username:pumpkin, password:pie)","http://www.colostate.edu/~ric/protect/your.html");
    menuBar.addBookmark("Certificate-error Test","https://www.k2go.de");
    menuBar.addBookmark("Resource-Handler Test", "http://www.foo.bar/");
    menuBar.addBookmark("Scheme-Handler Test 1: (scheme \"client\")", "client://tests/handler.html");
    menuBar.addBookmark("Scheme-Handler Test 2: (scheme \"search\")", "search://do a barrel roll/");
    menuBar.addBookmarkSeparator();
    menuBar.addBookmark("javachromiumembedded", "https://code.google.com/p/javachromiumembedded/");
    menuBar.addBookmark("chromiumembedded", "https://code.google.com/p/chromiumembedded/");
    setJMenuBar(menuBar);
  }

  private JPanel createContentPanel() {
    JPanel contentPanel = new JPanel(new BorderLayout());
    control_pane_ = new ControlPanel(browser_);
    status_panel_ = new StatusPanel();
    contentPanel.add(control_pane_, BorderLayout.NORTH);

    // 4) By calling getUIComponen() on the CefBrowser instance, we receive
    //    an displayable UI component which we can add to our application.
    contentPanel.add(browser_.getUIComponent(), BorderLayout.CENTER);

    contentPanel.add(status_panel_, BorderLayout.SOUTH);
    return contentPanel;
  }
}
