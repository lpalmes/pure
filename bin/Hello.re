open Pure.Primitives;

module Test = {
  let white = (255., 255., 255., 1.0);
  let textBlack = (56., 68., 77., 1.0);

  let backgroundColor = (225., 232., 237., 1.0);
  let createElement = (~children as _, _) =>
    Pure.element({
      ...Pure.reducerComponent("Test"),
      render: _self =>
        <window layout={...defaultLayout, flex: 1} title="Pure Native App">
          <view
            layout={...defaultLayout, flex: 1, flexDirection: Row}
            style={backgroundColor: Some(backgroundColor)}>
            <text title="Hello my friend" fontSize=200. />
          </view>
        </window>,
    });
};

PureMacOS.render(<Test />);