open Pure;

open Cocoa;

type hostNative =
  | Window(nsWindow)
  | View(nsView)
  | Button(nsButton);

let layout: ref((LayoutTypes.unitOfM, LayoutTypes.unitOfM) => unit) =
  ref((w, h) => print_endline("the layout function has not been replaced"));

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
      | Window =>
        let w = NSWindow.windowWithContentRect(0, (0., 0., 400., 400.));
        NSWindow.center(w);
        NSWindow.setMinSize(w, (400., 400.));
        NSWindow.makeKeyAndOrderFront(w);
        NSWindow.windowDidResize(() => {
          let contentView = NSWindow.getContentView(w);
          let width = NSView.getWidth(contentView) |> int_of_float;
          let height = NSView.getHeight(contentView) |> int_of_float;
          print_endline(
            "Did resize w: "
            ++ string_of_int(width)
            ++ " h:"
            ++ string_of_int(height),
          );
          layout.contents(width, height);
        });
        Window(w);
      };
    | _ => View(NSView.make((0., 0., 0., 0.)))
    };
  let createTextInstance = _value => {
    print_endline("Creating text instance");
    View(NSView.make((0., 0., 100., 100.)));
  };
  let commitUpdate = (node, oldProps, props) => {
    print_endline("Commiting and update");
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
  };
  let appendChild = (parent, node) => {
    print_endline("Apending child");
    switch (parent, node) {
    | (View(parent), View(node)) => NSView.addSubview(parent, node)
    | (View(parent), Button(node)) =>
      NSView.addSubview(parent, Obj.magic(node))
    | (Window(parent), View(node)) => NSWindow.addSubview(parent, node)
    | _ => ()
    };
  };
  let removeChild = (parent, node) => {
    print_endline("Removing child");
    switch (parent, node) {
    | (View(parent), View(node)) => NSView.removeFromSuperview(node)
    | (View(parent), Button(node)) =>
      NSView.removeFromSuperview(Obj.magic(node))
    | (Window(parent), View(node)) => NSView.removeFromSuperview(node)
    | _ => ()
    };
  };
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
    | Window(_) => ()
    };
};

module MacOSReconciler = Reconciler.Make(Host);

layout := MacOSReconciler.perfomLayout;

let render = (pureElement: pureElement) => {
  let app = Lazy.force(NSApplication.app);
  app#applicationWillFinishLaunching(() => {
    MacOSReconciler.updateQueue :=
      MacOSReconciler.updateQueue^
      @ [HostRoot({node: None, children: pureElement})];
    MacOSReconciler.perfomWork();
  });
  app#run;
};