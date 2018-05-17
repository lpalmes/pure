open Rereact;

module Host: ReconcilerSpec.HostConfig = {
  type hostNode = Dom.element;
  let createInstance = (Nested(primitive, props, _)) => {
    let name =
      switch primitive {
      | View => "div"
      | Text => "span"
      | Button => "button"
      };
    let node = Webapi.Dom.Document.createElement(name, Webapi.Dom.document);
    RereactDomProps.reconcile(node, None, props);
    switch (primitive, props.title) {
    | (Button, Some(value)) =>
      Webapi.Dom.Document.setTextContent(Obj.magic(node), value)
    | _ => ()
    };
    node;
  };
  let createTextInstance = value => {
    let node = Webapi.Dom.Document.createTextNode(value, Webapi.Dom.document);
    Webapi.Dom.Document.setNodeValue(Obj.magic(node), Js.Null.return(value));
    Obj.magic(node);
  };
  let commitUpdate = (node, oldProps, props) =>
    RereactDomProps.reconcile(node, oldProps, props);
  let appendChild = (parent, node) =>
    Webapi.Dom.Element.appendChild(node, parent);
  let removeChild = (parent, node) => {
    Js.log2(parent, node);
    Webapi.Dom.Element.removeChild(node, parent) |> ignore;
  };
  let applyLayout = (node, layout) => {
    Js.log(node);
    Js.log(layout);
  };
};

module DOMReconciler = Reconciler.Make(Host);

let render = (reactElement: reactElement, containerName) =>
  switch (
    Webapi.Dom.Document.getElementById(containerName, Webapi.Dom.document)
  ) {
  | Some(dom) =>
    DOMReconciler.updateQueue :=
      DOMReconciler.updateQueue^
      @ [HostRoot({node: Obj.magic(dom), children: reactElement})];
    DOMReconciler.perfomWork();
  | None => ()
  };