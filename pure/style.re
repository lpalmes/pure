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
      ~overflow=Visible,
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
      (),
    ) => {
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
};