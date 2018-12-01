type color = (float, float, float, float);

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

type fontStyle =
  | Normal
  | Italic;

type textAlign =
  | Auto
  | Left
  | Right
  | Center
  | Justify;
type textDecorationLine =
  | None
  | Underline
  | LineThrough
  | UnderlineLineThrough;

type imageResizeMode =
  | Contain
  | Cover
  | Stretch
  | Center
  | Repeat;

type textDecorationStyle =
  | Solid
  | Double
  | Dotted
  | Dashed;

type edge =
  | Left
  | Top
  | Right
  | Bottom
  | Start
  | End
  | Horizontal
  | Vertical
  | All;

type direction =
  | Inherit /* 'inherit' */
  | Ltr /* 'ltr'     */
  | Rtl /* 'rtl'     */;

type flexDirection =
  | Column /* 'column' */
  | ColumnReverse /* 'column-reverse' */
  | Row /* 'row' */
  | RowReverse /* 'row-reverse' */;

type justify =
  | FlexStart /* 'flex-start' */
  | Center /* 'center' */
  | FlexEnd /* 'flex-end' */
  | SpaceBetween /* 'space-between' */
  | SpaceAround /* space-around' */;

type align =
  | Auto
  | FlexStart /* 'flex-start' */
  | Center /* 'center' */
  | FlexEnd /* 'flex-end' */
  | Stretch /* 'stretch' */;

type position =
  | Relative /* 'relative' */
  | Absolute /* 'absolute' */;

type measureMode =
  /* there are some places in the reference implementation that set
   * the measure mode to negative one.*/
  /* https://github.com/facebook/css-layout/pull/185#r64995699 */
  | Undefined /* 'undefined' */
  | Exactly /* 'exactly' */
  | AtMost /* 'at-most' */
  | CSS_MEASURE_MODE_NEGATIVE_ONE_WHATEVER_THAT_MEANS;

type overflow =
  | Visible
  | Hidden
  | Scroll;

type wrapType =
  | NoWrap
  | Wrap;

type dimensions = {
  width: float,
  height: float,
};

type backfaceVisibility =
  | None
  | Visible
  | Hidden;

type borderStyle =
  | Solid
  | Dotted
  | Dashed;

type coordinates = {
  left: float,
  top: float,
};

type style = {
  direction,
  position,
  overflow,
  /* Flex */
  flex: float,
  flexGrow: float,
  flexShrink: float,
  flexBasis: float,
  flexDirection,
  flexWrap: wrapType,
  justifyContent: justify,
  alignContent: align,
  alignItems: align,
  alignSelf: align,
  /* margin */
  marginLeft: float,
  marginTop: float,
  marginRight: float,
  marginBottom: float,
  marginStart: float,
  marginEnd: float,
  marginHorizontal: float,
  marginVertical: float,
  margin: float,
  /* dimensions */
  width: float,
  height: float,
  minWidth: float,
  minHeight: float,
  maxWidth: float,
  maxHeight: float,
  /* absolute positioning */
  left: float,
  top: float,
  right: float,
  bottom: float,
  start: float,
  endd: float,
  horizontal: float,
  vertical: float,
  /***
   * You should skip all the rules that contain negative values for the
   * following attributes. For example:
   *   {padding: 10, paddingLeft: -5}
   * should output:
   *   {left: 10 ...}
   * the following two are incorrect:
   *   {left: -5 ...}
   *   {left: 0 ...}
   */
  /* Padding */
  paddingLeft: float,
  paddingTop: float,
  paddingRight: float,
  paddingBottom: float,
  paddingStart: float,
  paddingEnd: float,
  paddingHorizontal: float,
  paddingVertical: float,
  padding: float,
  /* Border */
  borderLeft: float,
  borderTop: float,
  borderRight: float,
  borderBottom: float,
  borderStart: float,
  borderEnd: float,
  borderHorizontal: float,
  borderVertical: float,
  border: float,
  /* View style */
  backfaceVisibility,
  backgroundColor: option(Color.color),
  borderBottomColor: option(string),
  borderBottomLeftRadius: option(float),
  borderBottomRightRadius: option(float),
  borderBottomWidth: option(float),
  borderColor: option(Color.color),
  borderLeftColor: option(Color.color),
  borderRadius: option(float),
  borderRightColor: option(Color.color),
  borderRightWidth: option(float),
  borderStyle,
  borderTopColor: option(Color.color),
  borderTopLeftRadius: option(float),
  borderTopRightRadius: option(float),
  borderTopWidth: option(float),
  opacity: option(float),
  shadowColor: option(Color.color),
  shadowOffset: option(dimensions),
  shadowOpacity: option(float),
  shadowRadius: option(float),
  elevation: option(float),
  /* Text Style */
  color: option(Color.color),
  fontFamily: option(string),
  fontSize: option(float),
  fontStyle: option(fontStyle),
  fontWeight: option(fontWeight),
  letterSpacing: option(float),
  lineHeight: option(float),
  textAlign: option(textAlign),
  textDecorationLine: option(textDecorationLine),
  textDecorationStyle: option(textDecorationStyle),
  textDecorationColor: option(Color.color),
  textShadowColor: option(Color.color),
  textShadowOffset: option(dimensions),
  textShadowRadius: option(float),
  /* Image Style */
  resizeMode: option(imageResizeMode),
  overlayColor: option(Color.color),
  tintColor: option(Color.color),
};

let cssUndefined = nan;

let defaultStyle = {
  direction: Inherit,
  flexDirection: Column,
  justifyContent: FlexStart,
  alignContent: FlexStart,
  alignItems: Stretch,
  alignSelf: Auto,
  position: Relative,
  flexWrap: NoWrap,
  overflow: Visible,
  flex: cssUndefined,
  flexGrow: cssUndefined,
  flexShrink: cssUndefined,
  flexBasis: cssUndefined,
  marginLeft: cssUndefined,
  marginTop: cssUndefined,
  marginRight: cssUndefined,
  marginBottom: cssUndefined,
  paddingLeft: cssUndefined,
  paddingTop: cssUndefined,
  paddingRight: cssUndefined,
  paddingBottom: cssUndefined,
  borderLeft: cssUndefined,
  borderTop: cssUndefined,
  borderRight: cssUndefined,
  borderBottom: cssUndefined,
  width: cssUndefined,
  height: cssUndefined,
  minWidth: cssUndefined,
  minHeight: cssUndefined,
  maxWidth: cssUndefined,
  maxHeight: cssUndefined,
  left: cssUndefined,
  top: cssUndefined,
  right: cssUndefined,
  bottom: cssUndefined,
  start: cssUndefined,
  endd: cssUndefined,
  marginStart: cssUndefined,
  marginEnd: cssUndefined,
  paddingStart: cssUndefined,
  paddingEnd: cssUndefined,
  borderStart: cssUndefined,
  borderEnd: cssUndefined,
  horizontal: cssUndefined,
  vertical: cssUndefined,
  padding: cssUndefined,
  paddingHorizontal: cssUndefined,
  paddingVertical: cssUndefined,
  margin: cssUndefined,
  marginVertical: cssUndefined,
  marginHorizontal: cssUndefined,
  borderHorizontal: cssUndefined,
  borderVertical: cssUndefined,
  border: cssUndefined,
  /* View Defaults */
  backfaceVisibility: None,
  backgroundColor: None,
  borderBottomColor: None,
  borderBottomLeftRadius: None,
  borderBottomRightRadius: None,
  borderBottomWidth: None,
  borderColor: None,
  borderLeftColor: None,
  borderRadius: None,
  borderRightColor: None,
  borderRightWidth: None,
  borderStyle: Solid,
  borderTopColor: None,
  borderTopLeftRadius: None,
  borderTopRightRadius: None,
  borderTopWidth: None,
  opacity: None,
  shadowColor: None,
  shadowOffset: None,
  shadowOpacity: None,
  shadowRadius: None,
  elevation: None,
  /* Text Defaults */
  color: None,
  fontFamily: None,
  fontSize: None,
  fontStyle: None,
  fontWeight: None,
  letterSpacing: None,
  lineHeight: None,
  textAlign: None,
  textDecorationLine: None,
  textDecorationStyle: None,
  textDecorationColor: None,
  textShadowColor: None,
  textShadowOffset: None,
  textShadowRadius: None,
  /* Image Style */
  resizeMode: None,
  overlayColor: None,
  tintColor: None,
};

let make =
    (
      ~direction=Inherit,
      ~flexDirection=Column,
      ~alignContent=FlexStart,
      ~justifyContent: justify=FlexStart,
      ~alignItems=Stretch,
      ~alignSelf=Auto,
      ~position=Relative,
      ~flexWrap=NoWrap,
      ~overflow: overflow=Visible,
      ~flex=cssUndefined,
      ~flexGrow=cssUndefined,
      ~flexShrink=cssUndefined,
      ~flexBasis=cssUndefined,
      ~marginLeft=cssUndefined,
      ~marginTop=cssUndefined,
      ~marginRight=cssUndefined,
      ~marginBottom=cssUndefined,
      ~paddingLeft=cssUndefined,
      ~paddingTop=cssUndefined,
      ~paddingRight=cssUndefined,
      ~paddingBottom=cssUndefined,
      ~borderLeft=cssUndefined,
      ~borderTop=cssUndefined,
      ~borderRight=cssUndefined,
      ~borderBottom=cssUndefined,
      ~width=cssUndefined,
      ~height=cssUndefined,
      ~minWidth=cssUndefined,
      ~minHeight=cssUndefined,
      ~maxWidth=cssUndefined,
      ~maxHeight=cssUndefined,
      ~left=cssUndefined,
      ~top=cssUndefined,
      ~right=cssUndefined,
      ~bottom=cssUndefined,
      ~start=cssUndefined,
      ~endd=cssUndefined,
      ~marginStart=cssUndefined,
      ~marginEnd=cssUndefined,
      ~paddingStart=cssUndefined,
      ~paddingEnd=cssUndefined,
      ~borderStart=cssUndefined,
      ~borderEnd=cssUndefined,
      ~horizontal=cssUndefined,
      ~vertical=cssUndefined,
      ~padding=cssUndefined,
      ~paddingHorizontal=cssUndefined,
      ~paddingVertical=cssUndefined,
      ~margin=cssUndefined,
      ~marginVertical=cssUndefined,
      ~marginHorizontal=cssUndefined,
      ~borderHorizontal=cssUndefined,
      ~borderVertical=cssUndefined,
      ~border=cssUndefined,
      ~backfaceVisibility=None,
      ~backgroundColor=?,
      ~borderBottomColor=?,
      ~borderBottomLeftRadius=?,
      ~borderBottomRightRadius=?,
      ~borderBottomWidth=?,
      ~borderColor=?,
      ~borderLeftColor=?,
      ~borderRadius=?,
      ~borderRightColor=?,
      ~borderRightWidth=?,
      ~borderStyle=Solid,
      ~borderTopColor=?,
      ~borderTopLeftRadius=?,
      ~borderTopRightRadius=?,
      ~borderTopWidth=?,
      ~opacity=?,
      ~shadowColor=?,
      ~shadowOffset=?,
      ~shadowOpacity=?,
      ~shadowRadius=?,
      ~elevation=?,
      ~color=?,
      ~fontFamily=?,
      ~fontSize=?,
      ~fontStyle=?,
      ~fontWeight=?,
      ~letterSpacing=?,
      ~lineHeight=?,
      ~textAlign=?,
      ~textDecorationLine=?,
      ~textDecorationStyle=?,
      ~textDecorationColor=?,
      ~textShadowColor=?,
      ~textShadowOffset=?,
      ~textShadowRadius=?,
      ~resizeMode=?,
      ~overlayColor=?,
      ~tintColor=?,
      (),
    )
    : style => {
  direction,
  flexDirection,
  justifyContent,
  alignContent,
  alignItems,
  alignSelf,
  position,
  flexWrap,
  overflow,
  flex,
  flexGrow,
  flexShrink,
  flexBasis,
  marginLeft,
  marginTop,
  marginRight,
  marginBottom,
  paddingLeft,
  paddingTop,
  paddingRight,
  paddingBottom,
  borderLeft,
  borderTop,
  borderRight,
  borderBottom,
  width,
  height,
  minWidth,
  minHeight,
  maxWidth,
  maxHeight,
  left,
  top,
  right,
  bottom,
  start,
  endd,
  marginStart,
  marginEnd,
  paddingStart,
  paddingEnd,
  borderStart,
  borderEnd,
  horizontal,
  vertical,
  padding,
  paddingHorizontal,
  paddingVertical,
  margin,
  marginVertical,
  marginHorizontal,
  borderHorizontal,
  borderVertical,
  border,
  backfaceVisibility,
  backgroundColor,
  borderBottomColor,
  borderBottomLeftRadius,
  borderBottomRightRadius,
  borderBottomWidth,
  borderColor,
  borderLeftColor,
  borderRadius,
  borderRightColor,
  borderRightWidth,
  borderStyle,
  borderTopColor,
  borderTopLeftRadius,
  borderTopRightRadius,
  borderTopWidth,
  opacity,
  shadowColor,
  shadowOffset,
  shadowOpacity,
  shadowRadius,
  elevation,
  color,
  fontFamily,
  fontSize,
  fontStyle,
  fontWeight,
  letterSpacing,
  lineHeight,
  textAlign,
  textDecorationLine,
  textDecorationStyle,
  textDecorationColor,
  textShadowColor,
  textShadowOffset,
  textShadowRadius,
  resizeMode,
  overlayColor,
  tintColor,
};