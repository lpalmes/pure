open Rereact;

module Make = (Config: ReconcilerSpec.HostConfig) => {
  module Types = FiberTypes.Make(Config);
  open Types;
  let indent = n =>
    for (_i in 0 to n - 1) {
      print_string("  ");
    };
  let getFiberTag = fiber =>
    switch fiber.tag {
    | HostRoot =>
      switch fiber.alternate {
      | Some(_) => print_endline("Has alternatev")
      | None => print_endline("No alternate")
      };
      "Root";
    | Host => "Host"
    | Component => "Component"
    };
  let getFiberEffect = fiber =>
    switch fiber.effectTag {
    | Some(Placement) => "Placement"
    | Some(Update) => "Update"
    | Some(Deletion) => "Deletion"
    | None => "No effect"
    };
  let getFiberElement = element =>
    switch element {
    | Nested(t, _, _) =>
      switch t {
      | View => "View"
      | Button => "Button"
      | Text => "Text"
      }
    | Flat(t) =>
      switch t {
      | Component({debugName}) => debugName
      | String(text) => "Text(" ++ text ++ ")"
      | Nil => "nil"
      }
    };
  let printEffects = effects =>
    List.iter(
      f => getFiberEffect(f) ++ " " ++ getFiberElement(f.fiberType) |> print_endline,
      effects
    );
  let rec printFiber = (Fiber(fiber), spaces: int) : unit => {
    let fiberTag = getFiberTag(fiber);
    let elementType = getFiberElement(fiber.fiberType);
    indent(spaces);
    let host =
      switch fiber.stateNode {
      | Some(_) => "Has node"
      | None => "None"
      };
    print_endline(fiberTag ++ " (" ++ elementType ++ ") StateNode: " ++ host);
    switch fiber.child {
    | Some(f) => printFiber(f, spaces + 1)
    | None => ()
    };
    switch fiber.sibling {
    | Some(f) => printFiber(f, spaces)
    | None => ()
    };
    ();
  };
};