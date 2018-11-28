type color = (float, float, float, float);

type style = {backgroundColor: option(color)};

type fontWeight =
  | Black
  | Bold
  | Heavy
  | Light
  | Medium
  | Regular
  | Semibold
  | Thin
  | UltraLight;

type props = {
  id: option(string),
  value: option(string),
  title: option(string),
  placeholder: option(string),
  src: option(string),
  disabled: option(bool),
  onClick: option(unit => unit),
  onChangeText: option(string => unit),
  fontFamily: option(string),
  fontSize: option(float),
  fontColor: option(color),
  fontWeight,
  borderRadius: option(float),
  layout: PureLayout.LayoutSupport.LayoutTypes.cssStyle,
  style,
};

type primitive =
  | View
  | Text
  | ScrollView
  | Button
  | Image
  | Window;

type sideEffects = unit => unit;

type stateless = unit;

type actionless = unit;

module Callback = {
  type t('payload) = 'payload => unit;
  let default = _event => ();
  let chain = (handlerOne, handlerTwo, payload) => {
    handlerOne(payload);
    handlerTwo(payload);
  };
};

type reduce('payload, 'action) =
  ('payload => 'action) => Callback.t('payload);

type update('state, 'action) =
  | NoUpdate
  | Update('state)
and self('state, 'action) = {
  state: 'state,
  send: 'action => unit,
};

type element =
  | String(string)
  | Component(component('state, 'action)): element
  | List(list(pureElement))
  | Nil
and pureElement =
  | Flat(element)
  | Nested(primitive, props, list(pureElement))
and componentSpec('state, 'initialState, 'action) = {
  debugName: string,
  render: self('state, 'action) => pureElement,
  initialState: unit => 'initialState,
  didMount: self('state, 'action) => unit,
  reducer: ('action, 'state) => update('state, 'action),
}
and component('state, 'action) = componentSpec('state, 'state, 'action);