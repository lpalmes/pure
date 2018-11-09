open Pure;

let defaultLayout = LayoutSupport.defaultStyle;

let defaultStyle: style = {backgroundColor: None};

let defaultProps: Pure.props = {
  id: None,
  value: None,
  title: None,
  disabled: None,
  placeholder: None,
  src: None,
  onClick: None,
  onChangeText: None,
  fontFamily: None,
  fontSize: None,
  fontWeight: Pure.Regular,
  fontColor: None,
  borderRadius: None,
  layout: defaultLayout,
  style: defaultStyle,
};

let createNativeElement =
    (
      nativeElement: primitive,
      ~id: option(string)=?,
      ~value: option(string)=?,
      ~title: option(string)=?,
      ~src: option(string)=?,
      ~onClick: option(unit => unit)=?,
      ~layout=defaultLayout,
      ~style=defaultStyle,
      ~children: list(pureElement),
      ~borderRadius: option(float)=?,
      ~fontFamily: option(string)=?,
      ~fontSize: option(float)=?,
      ~fontColor: option(color)=?,
      ~fontWeight: Pure.fontWeight=Pure.Regular,
      _: unit,
    ) =>
  Nested(
    nativeElement,
    {
      ...defaultProps,
      id,
      value,
      onClick,
      layout,
      style,
      src,
      title,
      fontFamily,
      borderRadius,
      fontSize,
      fontColor,
      fontWeight,
    },
    children,
  );

let view = createNativeElement(View);

let text = createNativeElement(Text);

let button = createNativeElement(Button);
let scrollView = createNativeElement(ScrollView);

let window = createNativeElement(Window);
let imageView = createNativeElement(Image);