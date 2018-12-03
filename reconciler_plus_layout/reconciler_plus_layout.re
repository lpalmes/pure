module Spec = Spec;

module Make = (Config: Spec.HostConfig) => {
  type hostNode = (Config.hostNode, int);

  module Layout = Layout.Make(Config);

  type node = Layout.LayoutSupport.LayoutTypes.node;

  let globalId: ref(int) = ref(0);

  type layoutNode = {
    node,
    id: int,
  };

  type layoutRoot = {
    mutable rootNode: layoutNode,
    mutable orphanNodes: list((int, node)),
  };

  let layoutRoot = {
    rootNode: {
      node:
        Layout.LayoutSupport.createNode(
          ~withChildren=[||],
          ~andStyle=Layout.LayoutSupport.defaultStyle,
          (Config.nullContextView(), 0),
        ),
      id: 0,
    },
    orphanNodes: [],
  };

  let appendChildNode = (node: node, child: node) => {
    open Layout.LayoutSupport;
    open LayoutTypes;
    assert(child.parent === theNullNode);
    /* assert(node.measure === None); */
    node.children = Array.append(node.children, [|child|]);
    node.childrenCount = node.childrenCount + 1;
    child.parent = node;
    Layout.LayoutSupport.markDirtyInternal(node);
  };

  let rec applyLayout = (node: node) => {
    let left = node.layout.left;
    let top = node.layout.top;
    let width = node.layout.width;
    let height = node.layout.height;

    let (v, _) = node.context;
    Config.applyLayout(v, {left, top, width, height});

    Array.iter(applyLayout, node.children);
  };
  let perfomLayout = (width, height) => {
    Layout.layoutNode(layoutRoot.rootNode.node, width, height, Ltr);
    /* PureLayout.LayoutPrint.printCssNode((
         layoutRoot.rootNode.node,
         {printLayout: true, printStyle: true, printChildren: true},
       )); */
    /* List.length(layoutRoot.orphanNodes) |> string_of_int |> print_endline; */
    applyLayout(layoutRoot.rootNode.node);
  };

  let createInstance = element => {
    print_endline("Creating instance");
    globalId := globalId.contents + 1;

    let view = Config.createInstance(element);
    Config.setPerfomLayout(view, perfomLayout);
    let measure = Config.textMeasure(view, element);

    let style =
      (
        switch (element) {
        | Nested(_, {style: Some(s), _}, _) => s
        | _ => Pure.Style.defaultStyle
        }
      )
      |> Layout.styleToLayoutStyle;

    let node =
      switch (measure) {
      | Some(m) =>
        Layout.LayoutSupport.createNode(
          ~withChildren=[||],
          ~andStyle=style,
          ~andMeasure=
            (_, w, _, _, _) => {
              let (width, height) = m(w);
              {width, height};
            },
          (view, globalId.contents),
        )
      | None =>
        Layout.LayoutSupport.createNode(
          ~withChildren=[||],
          ~andStyle=style,
          (view, globalId.contents),
        )
      };

    let id = globalId^;

    if (id == 1) {
      layoutRoot.rootNode = {node, id};
    };
    layoutRoot.orphanNodes = [(id, node), ...layoutRoot.orphanNodes];
    (view, globalId.contents);
  };
  let createTextInstance = value => {
    let v = Config.createTextInstance(value);
    globalId := globalId.contents + 1;
    let id = globalId^;
    let node =
      Layout.LayoutSupport.createNode(
        ~withChildren=[||],
        ~andStyle=Layout.LayoutSupport.defaultStyle,
        (v, id),
      );
    layoutRoot.orphanNodes = [(id, node), ...layoutRoot.orphanNodes];
    (v, globalId.contents);
  };

  let appendChild = ((parent, parentId), (node, nodeId)) => {
    Config.appendChild(parent, node);
    print_endline("Appending a child");
    let (_, parentLayoutNode) =
      List.find(((id, _)) => id == parentId, layoutRoot.orphanNodes);
    let (_, childLayoutNode) =
      List.find(((id, _)) => id == nodeId, layoutRoot.orphanNodes);

    appendChildNode(parentLayoutNode, childLayoutNode);
  };
  let removeChild = ((parent, parentId), (child, nodeId)) => {
    Config.removeChild(parent, child);
    let (_, parentLayoutNode) =
      List.find(((id, _)) => id == parentId, layoutRoot.orphanNodes);
    let (_, _childLayoutNode) =
      List.find(((id, _)) => id == nodeId, layoutRoot.orphanNodes);

    Array.length(parentLayoutNode.children) |> string_of_int |> print_endline;
    parentLayoutNode.children =
      parentLayoutNode.children
      |> Array.to_list
      |> List.filter((node: node) => {
           let (_, id) = node.context;
           id != nodeId;
         })
      |> Array.of_list;
    parentLayoutNode.childrenCount = parentLayoutNode.childrenCount - 1;
    Array.length(parentLayoutNode.children) |> string_of_int |> print_endline;
  };
  let commitUpdate =
      (
        (node, nodeId),
        oldProps: option(Pure.Types.props),
        props: Pure.Types.props,
      ) => {
    let (_, n) =
      List.find(((id, _)) => id == nodeId, layoutRoot.orphanNodes);

    let style =
      (
        switch (props.style) {
        | Some(s) => s
        | None => Pure.Style.defaultStyle
        }
      )
      |> Layout.styleToLayoutStyle;
    n.style = style;
    Config.commitUpdate(node, oldProps, props);
  };
  let afterCommit = () => {
    let (v, _) = layoutRoot.rootNode.node.context;
    let (w, h) = Config.getDimensions(v);
    perfomLayout(w, h);
  };
};

module Create = (Config: Spec.HostConfig) => {
  module ReconcilerLayout = Make(Config);
  module Reconciler = Reconciler.Make(ReconcilerLayout);
};