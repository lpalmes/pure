external _NSLog : string => unit = "ml_NSLog";

let log = fmt => Printf.ksprintf(_NSLog, fmt);

type nsRect = (float, float, float, float);

type nsSize = (float, float);

type nsWindow;

type nsView;

type nsButton;

type subView = {. addSubview: subView => unit};

module NSView = {
  external make : nsRect => nsView = "ml_NSViewWithContentRect";
  external setRect : (nsView, nsRect) => unit = "ml_NSViewSetContentRect";
  external getRect : nsView => nsRect = "ml_NSViewGetContentRect";
  external addSubview : (nsView, nsView) => unit = "ml_NSViewAddSubview";
  external getHeight : nsView => float = "ml_NSViewGetHeight";
  external getWidth : nsView => float = "ml_NSViewGetWidth";
  external setBackgroundColor :
    (nsView, [@unboxed] float, [@unboxed] float, [@unboxed] float, [@unboxed] float) => unit =
    "View_setBackgroundColor";
};

module NSButton = {
  external make : nsRect => nsButton = "ml_NSButtonWithContentRect";
  external setTitle : (nsButton, string) => unit = "ml_NSButtonSetTitle";
  external setCallback : (nsButton, unit => unit) => unit = "Button_setCallback";
};

module NSWindow = {
  type windowDelegate = {mutable windowDidResize: unit => unit};
  let delegate = () => {windowDidResize: () => ()};
  let win_id = ref(0);
  let globalWindowDelegate = ref(delegate());
  external getContentView : nsWindow => nsView = "ml_NSWindowGetContentView";
  external windowWithContentRect : (int, nsRect) => nsWindow = "ml_NSWindow_windowWithContentRect";
  external isVisible : nsWindow => bool = "ml_NSWindow_isVisible";
  external center : nsWindow => unit = "ml_NSWindow_center";
  external makeKeyAndOrderFront : nsWindow => unit = "ml_NSWindow_makeKeyAndOrderFront";
  external setTitle : (nsWindow, string) => unit = "ml_NSWindow_setTitle";
  external title : nsWindow => string = "ml_NSWindow_title";
  external addSubview : (nsWindow, nsView) => unit = "ml_NSWindowAddSubview";
  external setMinSize : (nsWindow, nsSize) => unit = "ml_NSWindowSetMinSize";
  let windowDidResize = f => {
    let delegate = delegate();
    globalWindowDelegate := delegate;
    delegate.windowDidResize = f;
  };
  type windowDelegateSelector =
    | WindowDidResize;
  let windowDelegate = (id, sel) =>
    switch sel {
    | WindowDidResize => globalWindowDelegate^.windowDidResize()
    };
  let () = Callback.register("[NSWindow delegate]", windowDelegate);
};

type nsApplication;

external _NSApplication_NSApp : int => nsApplication = "ml_NSApplication_NSApp";

external _NSApplication_run : nsApplication => unit = "ml_NSApplication_run";

module NSApplication = {
  class type t = {
    pub run: unit;
    pub applicationWillFinishLaunching: (unit => unit) => unit
  };
  type applicationDelegate = {mutable applicationWillFinishLaunching: unit => unit};
  let delegate = () => {applicationWillFinishLaunching: () => ()};
  let app_id = ref(0);
  let globalApplicationDelegate = ref(delegate());
  let app =
    lazy {
      let id = {
        incr(app_id);
        app_id^;
      };
      let app = _NSApplication_NSApp(id);
      let del = delegate();
      globalApplicationDelegate := del;
      {
        as _;
        pub run = _NSApplication_run(app);
        pub applicationWillFinishLaunching = f => del.applicationWillFinishLaunching = f
      };
    };
  type delegateSelector =
    | ApplicationWillFinishLaunching;
  let applicationDelegate = (id, sel) =>
    switch sel {
    | ApplicationWillFinishLaunching => globalApplicationDelegate^.applicationWillFinishLaunching()
    };
  let () = Callback.register("[NSApp delegate]", applicationDelegate);
};