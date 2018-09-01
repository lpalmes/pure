open Pure;
open GMain;

type hostNative =
  | View(GPack.fixed)
  | Button(GButton.button);

module Host: ReconcilerSpec.HostConfig = {
  type hostNode = hostNative;

  let createInstance = element => {
    print_endline("creating instances");
    switch (element) {
    | Nested(primitive, props, _) =>
      print_endline("Nested");
      switch (primitive) {
      | View =>
        print_endline("View");
        let view = GPack.fixed(~width=0, ~height=0, ());
        View(view);
      | Text =>
        print_endline("Text");
        View(GPack.fixed(~width=0, ~height=0, ()));
      | Button =>
        print_endline("Button");
        let button = GButton.button(~label="button", ());
        switch (props.onClick) {
        | Some(c) => button#connect#clicked(~callback=c) |> ignore
        | None => ()
        };
        Button(button);
      };
    | Flat(_) =>
      print_endline("Flat");
      View(GPack.fixed(~width=0, ~height=0, ()));
    };
  };

  let createTextInstance = _value =>
    View(GPack.fixed(~width=0, ~height=0, ()));

  let commitUpdate = (_node, _oldProps, _props) => print_endline("Updating");

  let appendChild = (parent, node) =>
    switch (parent, node) {
    | (View(parent), View(node)) =>
      parent#put(Obj.magic(node), ~x=0, ~y=0)
    | (View(parent), Button(node)) =>
      parent#put(Obj.magic(node), ~x=0, ~y=0)
    | _ => ()
    };

  let removeChild = (_, _) => {
    print_endline("Removing child");
    print_endline("Removing child");
  };

  let applyLayout = (node, layout: ReconcilerSpec.cssLayout) =>
    switch (node) {
    | View(view) => print_float(layout.left)
    | Button(view) => print_float(layout.left)
    };
};

module GTKReconciler = Reconciler.Make(Host);

let render = (pureElement: pureElement) => {
  let window =
    GWindow.window(
      ~width=400,
      ~height=400,
      ~title="Simple lablgtk program",
      (),
    );

  let _ = window#connect#destroy(~callback=Main.quit);
  let mainFixed =
    GPack.fixed(~width=400, ~height=400, ~packing=window#add, ());
  GTKReconciler.updateQueue :=
    GTKReconciler.updateQueue^
    @ [HostRoot({node: Obj.magic(View(mainFixed)), children: pureElement})];
  GTKReconciler.perfomWork();
  window#show();
  Main.main();
};