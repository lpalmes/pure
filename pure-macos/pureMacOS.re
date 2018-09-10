open Pure;

open Cocoa;

type hostNative =
  | View(nsView)
  | Button(nsButton);

module Host: ReconcilerSpec.HostConfig = {
  type hostNode = hostNative;
  let createInstance = element =>
    switch (element) {
    | Nested(primitive, props, _) =>
      print_endline("Creating item");
      switch (primitive) {
      | View =>
        let view = NSView.make((0., 0., 0., 0.));
        switch (props.style.backgroundColor) {
        | Some((r, g, b, a)) =>
          print_endline("Setting background color");
          NSView.setBackgroundColor(view, r, g, b, a);
        | None => ()
        };
        View(view);
      | Text => View(NSView.make((0., 0., 0., 0.)))
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
      };
    | _ => View(NSView.make((0., 0., 0., 0.)))
    };
  let createTextInstance = _value => View(NSView.make((0., 0., 100., 100.)));
  let commitUpdate = (node, oldProps, props) =>
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

    | _ => ()
    };
  let appendChild = (parent, node) =>
    switch (parent, node) {
    | (View(parent), View(node)) => NSView.addSubview(parent, node)
    | (View(parent), Button(node)) =>
      NSView.addSubview(parent, Obj.magic(node))
    | _ => ()
    };
  let removeChild = (_, _) => print_endline("Removing child");
  let applyLayout = (node, layout: ReconcilerSpec.cssLayout) =>
    switch (node) {
    | View(view) =>
      print_endline("Applying layout");
      NSView.setRect(
        view,
        (layout.left, layout.top, layout.width, layout.height),
      );
    | Button(view) =>
      NSView.setRect(
        Obj.magic(view),
        (layout.left, layout.top, layout.width, layout.height),
      )
    };
};

module MacOSReconciler = Reconciler.Make(Host);

let render = (pureElement: pureElement) => {
  let app = Lazy.force(NSApplication.app);
  app#applicationWillFinishLaunching(() => {
    let w = NSWindow.windowWithContentRect(0, (0., 0., 400., 400.));
    NSWindow.center(w);
    NSWindow.setMinSize(w, (400., 400.));
    NSWindow.makeKeyAndOrderFront(w);
    NSWindow.windowDidResize(() => {
      let contentView = NSWindow.getContentView(w);
      let width = NSView.getWidth(contentView) |> int_of_float;
      let height = NSView.getHeight(contentView) |> int_of_float;
      MacOSReconciler.perfomLayout(width, height);
      print_endline(
        "Did resize w: "
        ++ string_of_int(width)
        ++ " h:"
        ++ string_of_int(height),
      );
    });

    let rootView = NSWindow.getContentView(w);
    MacOSReconciler.updateQueue :=
      MacOSReconciler.updateQueue^
      @ [
        HostRoot({node: Obj.magic(View(rootView)), children: pureElement}),
      ];
    MacOSReconciler.perfomWork();
  });
  app#run;
};