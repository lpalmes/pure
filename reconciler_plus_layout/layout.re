module Make = (Config: Spec.HostConfig) => {
  module Node = {
    type context = (Config.hostNode, int);
    /* Ignored - only needed to create the dummy instance. */
    let nullContext = (Config.nullContextView(), 0);
  };

  include Flex.Layout.Create(Node, Flex.FloatEncoding);

  module StyleT = {
    let direction =
        (v: Pure.Style.direction): LayoutSupport.LayoutTypes.direction =>
      switch (v) {
      | Inherit => Inherit
      | Ltr => Ltr
      | Rtl => Rtl
      };
    let flexDirection =
        (v: Pure.Style.flexDirection): LayoutSupport.LayoutTypes.flexDirection =>
      switch (v) {
      | Column => Column
      | ColumnReverse => ColumnReverse
      | Row => Row
      | RowReverse => RowReverse
      };
    let justify = (v: Pure.Style.justify): LayoutSupport.LayoutTypes.justify =>
      switch (v) {
      | FlexStart => JustifyFlexStart
      | Center => JustifyCenter
      | FlexEnd => JustifyFlexEnd
      | SpaceBetween => JustifySpaceBetween
      | SpaceAround => JustifySpaceAround
      };
    let align = (v: Pure.Style.align): LayoutSupport.LayoutTypes.align =>
      switch (v) {
      | Auto => AlignAuto
      | FlexStart => AlignFlexStart
      | Center => AlignCenter
      | FlexEnd => AlignFlexEnd
      | Stretch => AlignStretch
      };
    let position =
        (v: Pure.Style.position): LayoutSupport.LayoutTypes.positionType =>
      switch (v) {
      | Relative => Relative
      | Absolute => Absolute
      };

    let flexWrap =
        (v: Pure.Style.wrapType): LayoutSupport.LayoutTypes.wrapType =>
      switch (v) {
      | NoWrap => CssNoWrap
      | Wrap => CssWrap
      };
    let overflow =
        (v: Pure.Style.overflow): LayoutSupport.LayoutTypes.overflow =>
      switch (v) {
      | Visible => Visible
      | Hidden => Hidden
      | Scroll => Scroll
      };
  };

  let styleToLayoutStyle =
      (
        {
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
          _,
        }: Pure.Style.style,
      )
      : LayoutSupport.LayoutTypes.cssStyle => {
    ...LayoutSupport.defaultStyle,
    direction: StyleT.direction(direction),
    flexDirection: StyleT.flexDirection(flexDirection),
    justifyContent: StyleT.justify(justifyContent),
    alignContent: StyleT.align(alignContent),
    alignItems: StyleT.align(alignItems),
    alignSelf: StyleT.align(alignSelf),
    positionType: StyleT.position(position),
    flexWrap: StyleT.flexWrap(flexWrap),
    overflow: StyleT.overflow(overflow),
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
};