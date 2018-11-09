open Pure;

open Cocoa;

type id = int;

type hostNative =
  | Window(nsWindow)
  | View(nsView)
  | ImageView(NSImageView.imageView)
  | Text(TextView.textView)
  | Button(nsButton)
  | ScrollView(NSScrollView.nsScrollView);
/* (Cocoa View, reference id) */
type hostNativeView = (hostNative, id);

let viewTable = Hashtbl.create(2000);

let layout: ref((LayoutTypes.unitOfM, LayoutTypes.unitOfM) => unit) =
  ref((w, h) => print_endline("the layout function has not been replaced"));

module Node = {
  type context = hostNativeView;
  /* Ignored - only needed to create the dummy instance. */
  let nullContext = (View(Cocoa.NSView.make((0., 0., 0., 0.))), 0);
};

module PureLayout = Layout.Create(Node, HardCodedEncoding);

let globalId: ref(id) = ref(0);

type layoutNode = {
  node: PureLayout.LayoutSupport.NodeType.node,
  id,
};

type layoutRoot = {
  mutable rootNode: layoutNode,
  mutable orphanNodes: list((id, PureLayout.LayoutSupport.NodeType.node)),
};

let layoutRoot = {
  rootNode: {
    node:
      PureLayout.LayoutSupport.createNode(
        ~withChildren=[||],
        ~andStyle=LayoutSupport.defaultStyle,
        (View(Cocoa.NSView.make((0., 0., 0., 0.))), 0),
      ),
    id: 0,
  },
  orphanNodes: [],
};

let appendChildNode =
    (
      node: PureLayout.LayoutSupport.NodeType.node,
      child: PureLayout.LayoutSupport.NodeType.node,
    ) => {
  open PureLayout.LayoutSupport;
  open LayoutTypes;
  assert(child.parent === theNullNode);
  /* assert(node.measure === None); */
  node.children = Array.append(node.children, [|child|]);
  node.childrenCount = node.childrenCount + 1;
  child.parent = node;
  PureLayout.LayoutSupport.markDirtyInternal(node);
};

let fontWeightToCocoa = (w: Pure.fontWeight): Font.fontWeight =>
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

let rec applyLayout = (node: PureLayout.LayoutSupport.NodeType.node) => {
  let left = node.layout.left |> float_of_int;
  let top = node.layout.top |> float_of_int;
  let width = node.layout.width |> float_of_int;
  let height = node.layout.height |> float_of_int;

  let (v, _) = node.context;

  let view =
    switch (v) {
    | View(v) => v
    | Text(t) =>
      TextView.setFrame(t, width);
      Obj.magic(t);
    | Button(v) => Obj.magic(v)
    | ImageView(v) => Obj.magic(v)
    | Window(w) => w |> NSWindow.getContentView
    | ScrollView(s) => Obj.magic(s)
    };

  NSView.setRect(view, (left, top, width, height));

  Array.iter(applyLayout, node.children);
};
let perfomLayout = (w, h) => {
  let (Window(window), _) = layoutRoot.rootNode.node.context;
  let contentView = NSWindow.getContentView(window);
  let width = NSView.getWidth(contentView) |> int_of_float;
  let height = NSView.getHeight(contentView) |> int_of_float;
  PureLayout.layoutNode(layoutRoot.rootNode.node, width, height, Ltr);
  /* PureLayout.LayoutPrint.printCssNode((
       layoutRoot.rootNode.node,
       {printLayout: true, printStyle: false, printChildren: true},
     )); */
  /* List.length(layoutRoot.orphanNodes) |> string_of_int |> print_endline; */
  applyLayout(layoutRoot.rootNode.node);
};
let createTextStorage = props => {
  let text =
    switch (props.title) {
    | Some(t) => t
    | None => ""
    };
  let textStorage = TextStorage.make(text);
  let font =
    switch (props.fontSize) {
    | Some(size) =>
      Font.getSystemFont(size, fontWeightToCocoa(props.fontWeight))
    | None => Font.getSystemFont(12.0, fontWeightToCocoa(props.fontWeight))
    };
  TextStorage.setFont(textStorage, font);
  switch (props.fontColor) {
  | Some((r, g, b, a)) =>
    TextStorage.setColor(textStorage, NSColor.make(r, g, b, a))
  | None => ()
  };
  textStorage;
};

module Host: Reconciler.Spec.HostConfig = {
  type hostNode = hostNativeView;
  let createInstance = element => {
    globalId := globalId.contents + 1;
    let (view, node) =
      switch (element) {
      | Nested(primitive, props, _) =>
        let v =
          switch (primitive) {
          | View =>
            let view = NSView.make((0., 0., 0., 0.));
            switch (props.style.backgroundColor) {
            | Some((r, g, b, a)) =>
              NSView.setBackgroundColor(view, r, g, b, a)
            | None => ()
            };
            switch (props.borderRadius) {
            | Some(radius) => NSView.setBorderRadius(view, radius)
            | None => ()
            };
            View(view);
          | Text =>
            let textStorage = createTextStorage(props);
            let textView = TextView.make((0., 0., 0., 0.));
            TextView.setTextStorage(textView, textStorage);
            switch (props.style.backgroundColor) {
            | Some((r, g, b, a)) =>
              NSView.setBackgroundColor(Obj.magic(textView), r, g, b, a)
            | None => ()
            };
            Text(textView);
          | Image =>
            let view = NSImageView.make((0., 0., 200., 200.));
            switch (props.src) {
            | Some(src) => NSImageView.setImage(view, src)
            | None => ()
            };
            switch (props.style.backgroundColor) {
            | Some((r, g, b, a)) =>
              NSView.setBackgroundColor(Obj.magic(view), r, g, b, a)
            | None => ()
            };
            switch (props.borderRadius) {
            | Some(radius) =>
              NSView.setBorderRadius(Obj.magic(view), radius)
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
            switch (props.style.backgroundColor) {
            | Some((r, g, b, a)) =>
              NSView.setBackgroundColor(Obj.magic(scrollView), r, g, b, a)
            | None => ()
            };
            switch (props.borderRadius) {
            | Some(radius) =>
              NSView.setBorderRadius(Obj.magic(scrollView), radius)
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
            NSWindow.windowDidResize(() => {
              let contentView = NSWindow.getContentView(w);
              let width = NSView.getWidth(contentView) |> int_of_float;
              let height = NSView.getHeight(contentView) |> int_of_float;
              layout.contents(width, height);
            });
            Window(w);
          };

        let node =
          switch (v) {
          | Text(_) =>
            let Nested(_, props, _) = element;
            PureLayout.LayoutSupport.createNode(
              ~withChildren=[||],
              ~andStyle=props.layout,
              ~andMeasure=
                (_, w, _, _, _) => {
                  let textStorage = createTextStorage(props);
                  let (width, height) =
                    TextStorage.measure(textStorage, float_of_int(w), 0);
                  {
                    width: int_of_float(width),
                    height: int_of_float(height),
                  };
                },
              (v, globalId.contents),
            );
          | _ =>
            PureLayout.LayoutSupport.createNode(
              ~withChildren=[||],
              ~andStyle=props.layout,
              (v, globalId.contents),
            )
          };

        (v, node);

      | _ => assert(false)
      };

    let id = globalId^;

    Hashtbl.add(viewTable, id, view);

    if (id == 1) {
      layoutRoot.rootNode = {node, id};
    };
    layoutRoot.orphanNodes = [(id, node), ...layoutRoot.orphanNodes];
    (view, globalId.contents);
  };
  let createTextInstance = _value => {
    print_endline("Creating text instance");
    let v = View(NSView.make((0., 0., 100., 100.)));
    globalId := globalId.contents + 1;
    let id = globalId^;
    let node =
      PureLayout.LayoutSupport.createNode(
        ~withChildren=[||],
        ~andStyle=LayoutSupport.defaultStyle,
        (v, id),
      );
    layoutRoot.orphanNodes = [(id, node), ...layoutRoot.orphanNodes];
    (v, globalId.contents);
  };

  let appendChild = ((parent, parentId), (node, nodeId)) => {
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

    let (_, parentLayoutNode) =
      List.find(((id, _)) => id == parentId, layoutRoot.orphanNodes);
    let (_, childLayoutNode) =
      List.find(((id, _)) => id == nodeId, layoutRoot.orphanNodes);

    appendChildNode(parentLayoutNode, childLayoutNode);
  };
  let removeChild = ((parent, parentId), (node, nodeId)) => {
    switch (parent, node) {
    | (_, View(node)) => NSView.removeFromSuperview(node)
    | (_, ScrollView(node)) => NSView.removeFromSuperview(Obj.magic(node))
    | (View(_), Button(node)) =>
      NSView.removeFromSuperview(Obj.magic(node))
    | _ => ()
    };

    let (_, parentLayoutNode) =
      List.find(((id, _)) => id == parentId, layoutRoot.orphanNodes);
    let (_, _childLayoutNode) =
      List.find(((id, _)) => id == nodeId, layoutRoot.orphanNodes);

    Array.length(parentLayoutNode.children) |> string_of_int |> print_endline;
    parentLayoutNode.children =
      parentLayoutNode.children
      |> Array.to_list
      |> List.filter((node: PureLayout.LayoutSupport.NodeType.node) => {
           let (_, id) = node.context;
           id != nodeId;
         })
      |> Array.of_list;
    parentLayoutNode.childrenCount = parentLayoutNode.childrenCount - 1;
    Array.length(parentLayoutNode.children) |> string_of_int |> print_endline;
  };
  let commitUpdate = ((node, nodeId), _oldProps, props) => {
    let (_, n) =
      List.find(((id, _)) => id == nodeId, layoutRoot.orphanNodes);
    n.style = props.layout;
    switch (node) {
    | Button(button) =>
      switch (props.title) {
      | Some(title) => NSButton.setTitle(button, title)
      | None => ()
      }

    /* switch (props.onClick) {
       | Some(c) => NSButton.setCallback(button, c)
       | None => ()
       }; */

    | _ => ()
    };
  };
  let afterCommit = () =>
    perfomLayout(FixedEncoding.cssUndefined, FixedEncoding.cssUndefined);
};

module MacOSReconciler = Reconciler.Make(Host);

layout := perfomLayout;

let render = (pureElement: pureElement) => {
  let app = Lazy.force(NSApplication.app);
  app#applicationWillFinishLaunching(() => {
    MacOSReconciler.render(pureElement);
    perfomLayout(FixedEncoding.cssUndefined, FixedEncoding.cssUndefined);
  });
  app#run;
};