open PureTypes;

let defaultProps: PureTypes.props = {
  id: None,
  value: None,
  title: None,
  disabled: None,
  placeholder: None,
  src: None,
  onClick: None,
  onChangeText: None,
  style: None,
};

let createNativeElement =
    (
      nativeElement: primitive,
      ~id: option(string)=?,
      ~value: option(string)=?,
      ~title: option(string)=?,
      ~src: option(string)=?,
      ~onClick: option(unit => unit)=?,
      ~style: option(Style.style)=?,
      ~children: list(pureElement),
      _: unit,
    ) =>
  Nested(
    nativeElement,
    {...defaultProps, id, value, onClick, style, src, title},
    children,
  );

let view = createNativeElement(View);

let text = createNativeElement(Text);

let button = createNativeElement(Button);
let scrollView = createNativeElement(ScrollView);

let window = createNativeElement(Window);
let imageView = createNativeElement(Image);