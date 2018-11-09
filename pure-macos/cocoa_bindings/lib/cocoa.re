external _NSLog: string => unit = "ml_NSLog";
let log = fmt => Printf.ksprintf(_NSLog, fmt);

type nsRect = (float, float, float, float);

type cgSize = (float, float);

type nsWindow;

type nsView;

type nsButton;

module Font = {
  type nsFont;

  type fontWeight =
    | Black
    | Bold
    | Heavy
    | Light
    | Medium
    | Regular
    | Semibold
    | Thin
    | UltraLight;

  external make: (string, float) => nsFont = "ml_FontMake";
  external getSystemFont: (float, fontWeight) => nsFont = "ml_FontGetSystem";
};

module NSColor = {
  type color;
  external make: (float, float, float, float) => color = "ml_NSColorMake";
};

module TextStorage = {
  type textStorage;

  external make: string => textStorage = "ml_NSTextStorageMake";
  external setFont: (textStorage, Font.nsFont) => unit =
    "ml_NSTextStorageSetFont";
  external setColor: (textStorage, NSColor.color) => unit =
    "ml_NSTextStorageSetColor";
  external measure: (textStorage, float, int) => cgSize =
    "ml_NSTextStorageMeasure";
};

module NSAttributedString = {
  type nsAttributedString;

  external make: string => nsAttributedString = "ml_NSAttributedStringMake";
  external measure: (nsAttributedString, float, int) => cgSize =
    "ml_NSAttributedStringMeasure";
  external setFont: (nsAttributedString, Font.nsFont) => unit =
    "ml_NSAttributedStringSetFont";
};

module NSView = {
  external make: nsRect => nsView = "ml_NSViewWithContentRect";
  external setRect: (nsView, nsRect) => unit = "ml_NSViewSetContentRect";
  external getRect: nsView => nsRect = "ml_NSViewGetContentRect";
  external addSubview: (nsView, nsView) => unit = "ml_NSViewAddSubview";
  external removeFromSuperview: nsView => unit =
    "ml_NSViewRemoveFromSuperview";
  external getHeight: nsView => float = "ml_NSViewGetHeight";
  external getWidth: nsView => float = "ml_NSViewGetWidth";
  external setBackgroundColor:
    (
      nsView,
      [@unboxed] float,
      [@unboxed] float,
      [@unboxed] float,
      [@unboxed] float
    ) =>
    unit =
    "View_setBackgroundColor";
  external setBorderRadius: (nsView, float) => unit =
    "ml_NSViewSetBorderRadius";
};

module NSImageView = {
  type imageView;
  external make: nsRect => imageView = "ml_NSImageViewWithContentRect";
  external setImage: (imageView, string) => unit = "ml_NSImageViewSetImage";
};

module NSButton = {
  external make: nsRect => nsButton = "ml_NSButtonWithContentRect";
  external setTitle: (nsButton, string) => unit = "ml_NSButtonSetTitle";
  external setCallback: (nsButton, unit => unit) => unit =
    "Button_setCallback";
};

module NSWindow = {
  type windowDelegate = {mutable windowDidResize: unit => unit};
  let delegate = () => {windowDidResize: () => ()};
  let win_id = ref(0);
  let globalWindowDelegate = ref(delegate());
  external getContentView: nsWindow => nsView = "ml_NSWindowGetContentView";
  external windowWithContentRect: (int, nsRect) => nsWindow =
    "ml_NSWindow_windowWithContentRect";
  external isVisible: nsWindow => bool = "ml_NSWindow_isVisible";
  external center: nsWindow => unit = "ml_NSWindow_center";
  external makeKeyAndOrderFront: nsWindow => unit =
    "ml_NSWindow_makeKeyAndOrderFront";
  external setTitle: (nsWindow, string) => unit = "ml_NSWindow_setTitle";
  external title: nsWindow => string = "ml_NSWindow_title";
  external addSubview: (nsWindow, nsView) => unit = "ml_NSWindowAddSubview";
  external setMinSize: (nsWindow, cgSize) => unit = "ml_NSWindowSetMinSize";
  let windowDidResize = f => {
    let delegate = delegate();
    globalWindowDelegate := delegate;
    delegate.windowDidResize = f;
  };
  type windowDelegateSelector =
    | WindowDidResize;
  let windowDelegate = (id, sel) =>
    switch (sel) {
    | WindowDidResize => globalWindowDelegate^.windowDidResize()
    };
  let () = Callback.register("[NSWindow delegate]", windowDelegate);
};

module NSScrollView = {
  type nsScrollView;
  external make: nsRect => nsScrollView = "ml_NSScrollViewWithContentRect";
  external setDocumentView: (nsScrollView, nsView) => unit =
    "ml_NSScrollViewSetDocumentView";
};

module TextView = {
  type textView;
  external make: nsRect => textView = "ml_TextViewWithContentRect";
  external setAttributedString:
    (textView, NSAttributedString.nsAttributedString) => unit =
    "ml_TextViewSetAttributedString";
  external setTextStorage: (textView, TextStorage.textStorage) => unit =
    "ml_TextViewSetTextStorage";
  external setText: (textView, string) => unit = "ml_TextViewSetText";
  /* textView, width to render text  */
  external setFrame: (textView, float) => unit = "ml_TextViewSetFrame";
};

type nsApplication;

external _NSApplication_NSApp: int => nsApplication = "ml_NSApplication_NSApp";

external _NSApplication_run: nsApplication => unit = "ml_NSApplication_run";

module NSApplication = {
  class type t = {
    pub run: unit;
    pub applicationWillFinishLaunching: (unit => unit) => unit;
  };
  type applicationDelegate = {
    mutable applicationWillFinishLaunching: unit => unit,
  };
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
        pub applicationWillFinishLaunching = f =>
          del.applicationWillFinishLaunching = f
      };
    };
  type delegateSelector =
    | ApplicationWillFinishLaunching;
  let applicationDelegate = (id, sel) =>
    switch (sel) {
    | ApplicationWillFinishLaunching =>
      globalApplicationDelegate^.applicationWillFinishLaunching()
    };
  let () = Callback.register("[NSApp delegate]", applicationDelegate);
};