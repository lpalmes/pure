open Pure.Types;

open Cocoa;

type hostNative =
  | Window(nsWindow)
  | View(nsView)
  | ImageView(NSImageView.imageView)
  | Text(TextView.textView)
  | Button(nsButton)
  | ScrollView(NSScrollView.nsScrollView);

let fontWeightToCocoa = (w: Pure.Style.fontWeight): Font.fontWeight =>
  switch (w) {
  | Black => Black
  | Bold => Bold
  | Heavy => Heavy
  | Light => Light
  | Medium => Medium
  | Regular => Regular
  | Semibold => Semibold
  | Thin => Thin
  | UltraLight => UltraLight
  };

let createTextStorage = props => {
  let text =
    switch (props.title) {
    | Some(t) => t
    | None => ""
    };
  let textStorage = TextStorage.make(text);
  switch (props.style) {
  | Some(style) =>
    let font =
      switch (style.fontSize) {
      | Some(size) => Font.getSystemFont(size, fontWeightToCocoa(Regular))
      | None => Font.getSystemFont(12.0, fontWeightToCocoa(Regular))
      };
    TextStorage.setFont(textStorage, font);
    switch (style.color) {
    | Some(Rgba(r, g, b, a)) =>
      TextStorage.setColor(textStorage, NSColor.make(r, g, b, a))
    | Some(Rgb(r, g, b)) =>
      TextStorage.setColor(textStorage, NSColor.make(r, g, b, 1.0))
    | _ => ()
    };
  | None => ()
  };
  textStorage;
};

module Host: Reconciler_plus_layout.Spec.HostConfig = {
  type hostNode = hostNative;
  let createInstance = element =>
    switch (element) {
    | Nested(primitive, props, _) =>
      let v =
        switch (primitive) {
        | View =>
          let view = NSView.make((0., 0., 0., 0.));
          switch (props.style) {
          | Some(style) =>
            switch (style.backgroundColor) {
            | Some(Rgba(r, g, b, a)) =>
              NSView.setBackgroundColor(view, r, g, b, a)
            | _ => ()
            };
            switch (style.borderRadius) {
            | Some(radius) => NSView.setBorderRadius(view, radius)
            | None => ()
            };
          | None => ()
          };
          View(view);
        | Text =>
          let textStorage = createTextStorage(props);
          let textView = TextView.make((0., 0., 0., 0.));
          TextView.setTextStorage(textView, textStorage);
          switch (props.style) {
          | Some(style) =>
            switch (style.backgroundColor) {
            | Some(Rgba(r, g, b, a)) =>
              NSView.setBackgroundColor(Obj.magic(textView), r, g, b, a)
            | _ => ()
            }
          | None => ()
          };
          Text(textView);
        | Image =>
          let view = NSImageView.make((0., 0., 200., 200.));
          switch (props.src) {
          | Some(src) => NSImageView.setImage(view, src)
          | None => ()
          };
          switch (props.style) {
          | Some(style) =>
            switch (style.backgroundColor) {
            | Some(Rgba(r, g, b, a)) =>
              NSView.setBackgroundColor(Obj.magic(view), r, g, b, a)
            | _ => ()
            };
            switch (style.borderRadius) {
            | Some(radius) =>
              NSView.setBorderRadius(Obj.magic(view), radius)
            | None => ()
            };
          | None => ()
          };
          ImageView(view);
        | Button =>
          let button = NSButton.make((0., 0., 100., 100.));
          switch (props.title) {
          | Some(v) => NSButton.setTitle(button, v)
          | None => ()
          };
          switch (props.onClick) {
          | Some(c) => NSButton.setCallback(button, c)
          | None => ()
          };
          Button(button);
        | ScrollView =>
          let scrollView = NSScrollView.make((0., 0., 0., 0.));
          switch (props.style) {
          | Some(style) =>
            switch (style.backgroundColor) {
            | Some(Rgba(r, g, b, a)) =>
              NSView.setBackgroundColor(Obj.magic(scrollView), r, g, b, a)
            | _ => ()
            };
            switch (style.borderRadius) {
            | Some(radius) =>
              NSView.setBorderRadius(Obj.magic(scrollView), radius)
            | None => ()
            };
          | None => ()
          };
          ScrollView(scrollView);
        | Window =>
          let w = NSWindow.windowWithContentRect(0, (0., 0., 800., 550.));
          switch (props.title) {
          | Some(t) => NSWindow.setTitle(w, t)
          | None => ()
          };
          NSWindow.center(w);
          NSWindow.setMinSize(w, (400., 400.));
          NSWindow.makeKeyAndOrderFront(w);
          /* NSWindow.windowDidResize(perfomLayout); */
          Window(w);
        };
      v;
    | _ => assert(false)
    };
  let createTextInstance = _value => View(NSView.make((0., 0., 100., 100.)));

  let appendChild = (parent, node) =>
    switch (parent, node) {
    | (View(parent), View(node)) => NSView.addSubview(parent, node)
    | (View(parent), Text(node)) =>
      NSView.addSubview(parent, Obj.magic(node))
    | (View(parent), ImageView(node)) =>
      NSView.addSubview(parent, Obj.magic(node))
    | (View(parent), ScrollView(node)) =>
      NSView.addSubview(parent, Obj.magic(node))
    | (ScrollView(parent), View(node)) =>
      NSScrollView.setDocumentView(parent, node)
    | (View(parent), Button(node)) =>
      NSView.addSubview(parent, Obj.magic(node))
    | (Window(parent), View(node)) => NSWindow.addSubview(parent, node)
    | _ => ()
    };
  let removeChild = (parent, child) =>
    switch (parent, child) {
    | (_, View(node)) => NSView.removeFromSuperview(node)
    | (_, ScrollView(node)) => NSView.removeFromSuperview(Obj.magic(node))
    | (View(_), Button(node)) =>
      NSView.removeFromSuperview(Obj.magic(node))
    | _ => ()
    };
  let commitUpdate = (node, _oldProps, props) =>
    switch (node) {
    | Button(button) =>
      switch (props.title) {
      | Some(title) => NSButton.setTitle(button, title)
      | None => ()
      };

      switch (props.onClick) {
      | Some(c) => NSButton.setCallback(button, c)
      | None => ()
      };
    | Text(textView) =>
      let textStorage = createTextStorage(props);
      TextView.setTextStorage(textView, textStorage);

    | _ => ()
    };
  let afterCommit = () => print_endline("Everything has been comitted");

  let nullContextView = () => View(NSView.make((0., 0., 0., 0.)));

  let applyLayout = (v, layout: Reconciler_plus_layout.Spec.layoutNode) => {
    print_endline("Applying layout");
    let view =
      switch (v) {
      | View(v) => v
      | Text(t) =>
        TextView.setFrame(t, layout.width);
        Obj.magic(t);
      | Button(v) => Obj.magic(v)
      | ImageView(v) => Obj.magic(v)
      | Window(w) => w |> NSWindow.getContentView
      | ScrollView(s) => Obj.magic(s)
      };

    NSView.setRect(
      view,
      (layout.left, layout.top, layout.width, layout.height),
    );
  };

  let textMeasure = (v, element) =>
    switch (v, element) {
    | (Text(_), Nested(_, props, _)) =>
      Some(
        (
          (w: float) => {
            let textStorage = createTextStorage(props);
            let (w, h) = TextStorage.measure(textStorage, w, 0);
            (w, h);
          }
        ),
      )
    | _ => None
    };

  let getDimensions = node => {
    let view =
      switch (node) {
      | Window(w) => NSWindow.getContentView(w)
      | View(v) => v
      | ImageView(v) => Obj.magic(v)
      | Text(t) => Obj.magic(t)
      | Button(v) => Obj.magic(v)
      | ScrollView(v) => Obj.magic(v)
      };
    let width = NSView.getWidth(view);
    let height = NSView.getHeight(view);
    (width, height);
  };

  let setPerfomLayout = (node, performLayout) =>
    switch (node) {
    | Window(w) =>
      NSWindow.windowDidResize(() => {
        let v = NSWindow.getContentView(w);
        let w = NSView.getWidth(v);
        let h = NSView.getHeight(v);
        performLayout(w, h);
      })
    | _ => ()
    };
};

module MacOSReconciler = Reconciler_plus_layout.Create(Host);

let render = (pureElement: pureElement) => {
  let app = Lazy.force(NSApplication.app);
  app#applicationWillFinishLaunching(() =>
    MacOSReconciler.Reconciler.render(pureElement)
  );
  app#run;
};