open RereactPrimitives;

module Test = {
  let createElement = (~children as _, _) =>
    Rereact.element({
      ...Rereact.statelessComponent("Test"),
      render: _self => <view> <text> (Rereact.string("Hello")) </text> </view>
    });
};

module ReducerComponent = {
  type state = int;
  type action =
    | Add(int);
  let createElement = (~children as _, _) =>
    Rereact.element({
      ...Rereact.reducerComponent("Reducer Component"),
      initialState: () => 2,
      reducer: (action: action, state: state) =>
        switch action {
        | Add(value) => Rereact.Update(value + state)
        },
      render: self =>
        <view
          layout={
            ...defaultLayout,
            flex: 1,
            justifyContent: JustifySpaceAround
          }>
          <view layout={...defaultLayout, height: 200, flexDirection: Row}>
            <button layout={...defaultLayout, flex: 1} title="Click me" />
            <button layout={...defaultLayout, flex: 1} title="Or me" />
          </view>
          <view
            layout={...defaultLayout, flex: 1, margin: 16}
            style={backgroundColor: Some((150., 0., 0., 0.2))}
          />
          <view
            layout={...defaultLayout, flex: 1, margin: 16}
            style={backgroundColor: Some((0., 0., 0., 0.2))}
          />
          <view
            layout={...defaultLayout, flex: 1, margin: 16}
            style={backgroundColor: Some((0., 0., 0., 0.2))}
          />
        </view>
    });
};