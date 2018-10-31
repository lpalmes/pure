open Pure;

module Tree = {
  let referenceId = ref(0);

  type node =
    | String(string)
    | Host(Pure.primitive);

  type element = {
    id: int,
    node,
    mutable children: list(element),
  };

  let rec treesDiffer = (a, b) =>
    if (a.node != b.node
        || List.length(a.children) != List.length(b.children)) {
      true;
    } else {
      List.exists2(treesDiffer, a.children, b.children);
    };

  let compareTrees = (a, b) =>
    if (!treesDiffer(a, b)) {
      print_endline("Same trees");
    } else {
      print_endline("Different trees");
    };

  let indent = n =>
    for (_i in 0 to n - 1) {
      print_string("  ");
    };

  let printNode = node =>
    switch (node) {
    | String(value) => "String(" ++ value ++ ")"
    | Host(primitive) =>
      "Host("
      ++ (
        switch (primitive) {
        | View => "View"
        | Text => "Text"
        | Button => "Button"
        | Window => "Window"
        }
      )
      ++ ")"
    };
  let rec printTree = (element, level) => {
    indent(level);
    print_string(printNode(element.node));
    /* indent(level);
       print_string("Id: " ++ string_of_int(element.id)); */
    print_newline();
    List.iter(e => printTree(e, level + 1), element.children);
  };

  let createElement = primitive => {
    referenceId := referenceId^ + 1;
    {id: referenceId^, node: Host(primitive), children: []};
  };

  let createTextElement = text => {
    referenceId := referenceId^ + 1;
    {id: referenceId^, node: String(text), children: []};
  };

  let addChild = (parent, child) =>
    parent.children = parent.children @ [child];

  let removeChild = (parent, child) =>
    parent.children = List.filter(c => c.id != child.id, parent.children);
};

module Host: Reconciler.Spec.HostConfig = {
  type hostNode = Tree.element;
  let createInstance = element =>
    switch (element) {
    | Nested(primitive, _, _) => Tree.createElement(primitive)
    | _ => assert(false)
    };
  let createTextInstance = value => Tree.createTextElement(value);
  let commitUpdate = (node, oldProps, props) => ();
  let appendChild = Tree.addChild;
  let removeChild = Tree.removeChild;

  let afterCommit = () => print_endline("Commited");
};

module NoopReconciler = Reconciler.Make(Host);

let rootA = Tree.createElement(View);
let rootB = Tree.createElement(View);

Tree.compareTrees(rootA, rootB);

/* Tree.printTree(rootA, 0); */

open PurePrimitives;
let listOfNumbers =
  <view>
    <button />
    {Pure.string("Sample text")}
    {
      [1, 2, 3, 4, 5, 6]
      |> List.map(i =>
           <view>
             {Pure.string(string_of_int(i))}
             <button>
               {Pure.string("Click to console " ++ string_of_int(i))}
             </button>
           </view>
         )
      |> Pure.list
    }
  </view>;

let hello = <view> {Pure.string("Helloo my friend")} </view>;
let listOfStrings =
  ["Nothing wrong here", "yet", "well i hope so"]
  |> List.map(i => <view> {Pure.string(i)} </view>)
  |> Pure.list;
NoopReconciler.renderWithRoot(listOfNumbers, Obj.magic(rootA));

print_endline("Tree A");
Tree.printTree(rootA, 0);
print_newline();
print_endline("Tree B");
Tree.printTree(rootB, 0);
print_newline();

NoopReconciler.renderWithRoot(hello, Obj.magic(rootA));

print_endline("Tree A");
Tree.printTree(rootA, 0);
print_newline();
print_endline("Tree B");
Tree.printTree(rootB, 0);
print_newline();

NoopReconciler.renderWithRoot(listOfStrings, Obj.magic(rootA));

print_endline("Tree A");
Tree.printTree(rootA, 0);
print_newline();
print_endline("Tree B");
Tree.printTree(rootB, 0);
print_newline();