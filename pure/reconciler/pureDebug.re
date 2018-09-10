open Pure;

module Make = (Config: ReconcilerSpec.HostConfig) => {
  module Types = FiberTypes.Make(Config);
  open Types;
  let indent = n =>
    for (_i in 0 to n - 1) {
      print_string("  ");
    };
  let getFiberTag = fiber =>
    switch (fiber.tag) {
    | HostRoot => "HostRoot"
    | Host => "Host"
    | Component => "Component"
    };
  let getFiberEffect = fiber =>
    switch (fiber.effectTag) {
    | Some(Placement) => "Placement"
    | Some(Update) => "Update"
    | Some(Deletion) => "Deletion"
    | None => "No effect"
    };
  let getFiberElement = element =>
    switch (element) {
    | Nested(t, _, _) =>
      "Nested "
      ++ (
        switch (t) {
        | View => "View"
        | Button => "Button"
        | Text => "Text"
        }
      )
    | Flat(t) =>
      "Flat "
      ++ (
        switch (t) {
        | Component({debugName}) => debugName
        | String(text) => "Text(" ++ text ++ ")"
        | Nil => "nil"
        }
      )
    };
  let printEffects = effects =>
    List.iter(
      f =>
        getFiberEffect(f)
        ++ " "
        ++ getFiberElement(f.fiberType)
        |> print_endline,
      effects,
    );

  let rec printTree = (Fiber(fiber)): string => {
    let tree = ref("");

    tree.contents = tree.contents ++ printFiber(Fiber(fiber));

    /* tree.contents =
       tree.contents
       ++ "\n"
       ++ (
         switch (fiber.sibling) {
         | Some(sibiling) => printTree(sibiling)
         | None => ""
         }
       ); */

    tree.contents =
      tree.contents
      ++ (
        switch (fiber.child) {
        | Some(child) => "\n  |\n" ++ printTree(child)
        | None => ""
        }
      );

    tree.contents;
  }
  and printFiber = (Fiber(fiber)) => {
    let fib = ref("");
    fib := fib^ ++ getFiberTag(fiber);
    fib := fib^ ++ "\nElement: " ++ getFiberElement(fiber.fiberType);
    fib :=
      fib^
      ++ "\nChild: "
      ++ (
        switch (fiber.child) {
        | Some(_) => "Has Child Node"
        | None => "No Child Node"
        }
      );
    fib :=
      fib^
      ++ "\nStateNode: "
      ++ (
        switch (fiber.stateNode) {
        | Some(_) => "Has State Node"
        | None => "No State Node"
        }
      );
    fib :=
      fib^
      ++ "\nParent: "
      ++ (
        switch (fiber.parent) {
        | Some(_) => "Has Parent Node"
        | None => "No Parent Node"
        }
      );
    fib :=
      fib^
      ++ "\nEffect: "
      ++ (
        switch (fiber.effectTag) {
        | Some(Placement) => "Placement"
        | Some(Update) => "Update"
        | Some(Deletion) => "Deletion"
        | None => "No effect"
        }
      );
    fib :=
      fib^
      ++ "\nSibiling: "
      ++ (
        switch (fiber.sibling) {
        | Some(_) => "Has Sibiling Node"
        | None => "No Sibiling"
        }
      );

    fib^;
  };
};