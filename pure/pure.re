module Types = PureTypes;
module Primitives = PurePrimitives;
module Layout = PureLayout;
open Types;
let basicComponent = (debugName): componentSpec(_, _, _) => {
  debugName,
  render: _self => assert(false),
  initialState: () => (),
  didMount: _self => (),
  reducer: (_action, _state) => NoUpdate,
};

let statelessComponent = debugName => {
  ...basicComponent(debugName),
  initialState: () => (),
};

let statefulComponent = debugName => basicComponent(debugName);

let reducerComponent = debugName => basicComponent(debugName);

let string = value => Flat(String(value));

let list = value => Flat(List(value));

let array = value => Flat(List(Array.to_list(value)));
let null = Flat(Nil);

let nil = Flat(Nil);

let element = component => Flat(Component(component));