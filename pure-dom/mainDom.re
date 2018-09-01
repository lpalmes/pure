open PurePrimitives;

switch (Webapi.Dom.Document.getElementById("container", Webapi.Dom.document)) {
| Some(container) => PureDom.render(<view><view>(Pure.string("Hello"))</view></view>, Obj.magic(container))
| None => print_endline("Couldn't find node")
};