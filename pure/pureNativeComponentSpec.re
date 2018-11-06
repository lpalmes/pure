module type HostComponentSpec = {
  type t;
  let createView: unit => option(t);
  let updateView: t => unit;
};

module KeyCompare = {
  type t = string;
  let compare = (a, b) => Pervasives.compare(a, b);
};

module HostComponentRegister = Map.Make(KeyCompare);

module IntHostComponent: HostComponentSpec = {
  type t = int;
  let createView = () => None;
  let updateView = view => ();
};
let testComponent: module HostComponentSpec = (module IntHostComponent);

let m = HostComponentRegister.empty;
let m1 = HostComponentRegister.add("Int", testComponent, m);

let component = HostComponentRegister.find("Int", m1);

module Host = (val component: HostComponentSpec);

let v = Host.createView();