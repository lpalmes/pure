module Make = (Config: ReconcilerSpec.HostConfig) => {
  module FiberTypes = FiberTypes.Make(Config);
  module Debug = RereactDebug.Make(Config);
  open FiberTypes;
  module Node = {
    type context = FiberTypes.opaqueFiber;
    /* Ignored - only needed to create the dummy instance. */
    let nullContext =
      Fiber({
        tag: Host,
        state: None,
        fiberType: Flat(Nil),
        parent: None,
        child: None,
        sibling: None,
        alternate: None,
        effectTag: None,
        effects: [],
        stateNode: None
      });
  };
  module Encoding = FixedEncoding;
  module LayoutTypes = LayoutTypes.Create(Node, FixedEncoding);
  open LayoutTypes;
  module Layout = Layout.Create(Node, FixedEncoding);
  module LS = Layout.LayoutSupport;
  module LayoutPrint = LayoutPrint.Create(Node, FixedEncoding);
  let rec createLayoutNodeFromFiber =
          (fiber: FiberTypes.opaqueFiber)
          : Layout.LayoutSupport.NodeType.node => {
    let Fiber(f) = fiber;
    let children =
      switch f.child {
      | Some(child) => createLayoutNodeChildren(child) |> Array.map(createLayoutNodeFromFiber)
      | None => [||]
      };
    let style =
      switch f.fiberType {
      | Nested(_, props, _) => props.layout
      | _ => LayoutSupport.defaultStyle
      };
    LS.createNode(~withChildren=children, ~andStyle=style, fiber);
  }
  and createLayoutNodeChildren = (fiber: FiberTypes.opaqueFiber) => {
    let children: ref(array(FiberTypes.opaqueFiber)) = ref([|fiber|]);
    let Fiber(n) = fiber;
    let node = ref(n);
    let break = ref(false);
    while (break^ == false) {
      switch node.contents.sibling {
      | Some(Fiber(sibl) as siblingNode) =>
        children := Array.append(children^, [|siblingNode|]);
        node := Obj.magic(sibl);
      | None => break := true
      };
    };
    children^;
  };
  let rec applyLayout = (node: LS.NodeType.node) => {
    let Fiber(fiber) = node.context;
    switch fiber.stateNode {
    | Some(stateNode) =>
      let layout: ReconcilerSpec.cssLayout = {
        left: float_of_int(node.layout.left),
        top: float_of_int(node.layout.top),
        right: float_of_int(node.layout.right),
        bottom: float_of_int(node.layout.bottom),
        width: float_of_int(node.layout.width),
        height: float_of_int(node.layout.height)
      };
      Config.applyLayout(stateNode, layout);
    | _ => ()
    };
    Array.iter(applyLayout, node.children);
  };
  let layout = (fiber: FiberTypes.opaqueFiber, width, height) => {
    let root = createLayoutNodeFromFiber(fiber);
    Layout.layoutNode(root, width, height, Ltr);
    applyLayout(root);
  };
};