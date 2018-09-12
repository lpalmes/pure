open Pure;

module Host: ReconcilerSpec.HostConfig = {
  type hostNode = Dom.element;
  let createInstance = element =>
    switch (element) {
    | Nested(primitive, props, _) =>
      let name =
        switch (primitive) {
        | View => "div"
        | Text => "span"
        | Button => "button"
        };
      let node = Webapi.Dom.Document.createElement(name, Webapi.Dom.document);
      PureDomProps.reconcile(node, None, props);
      switch (primitive, props.title) {
      | (Button, Some(value)) =>
        Webapi.Dom.Document.setTextContent(Obj.magic(node), value)
      | _ => ()
      };
      node;
    | _ => assert(false)
    };

  let createTextInstance = value => {
    let node = Webapi.Dom.Document.createTextNode(value, Webapi.Dom.document);
    Webapi.Dom.Text.setNodeValue(node, Js.Null.return(value));
    Obj.magic(node);
  };
  let commitUpdate = (node, oldProps, props) =>
    PureDomProps.reconcile(node, oldProps, props);
  let appendChild = (parent, node) =>
    Webapi.Dom.Element.appendChild(node, parent);
  let removeChild = (parent, node) =>
    Webapi.Dom.Element.removeChild(node, parent) |> ignore;
  let applyLayout = (_node, _layout) => ();
};

module DOMReconciler = Reconciler.Make(Host);

let render = (pureElement: pureElement, container: Host.hostNode) => {
  DOMReconciler.updateQueue :=
    [HostRoot({node: Some(container), children: pureElement})];
  DOMReconciler.perfomWork();
};