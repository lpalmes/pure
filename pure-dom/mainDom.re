switch (Webapi.Dom.Document.getElementById("container", Webapi.Dom.document)) {
| Some(container) => PureDom.render(<Sample.Test />, Obj.magic(container))
| None => print_endline("Couldn't find node")
};