open PurePrimitives;

module ReducerComponent = {
  type state = int;
  type action =
    | Add(int);
  let createElement = (~children as _, _) =>
    Pure.element({
      ...Pure.reducerComponent("Reducer Component"),
      initialState: () => 2,
      reducer: (action: action, state: state) =>
        switch (action) {
        | Add(value) => Pure.Update(value + state)
        },
      render: self =>
        <view layout={...defaultLayout, flex: 1, flexDirection: Row}>
          <button
            layout={...defaultLayout, flex: 1}
            title="Add"
            onClick={() => self.send(Add(1))}
          />
          <button
            layout={...defaultLayout, flex: 1}
            title={string_of_int(self.state)}
            onClick={() => self.state |> string_of_int |> print_endline}
          />
          <button
            layout={...defaultLayout, flex: 1}
            title="Substract"
            onClick={() => self.send(Add(-1))}
          />
        </view>,
    });
};

module Flex = {
  type state = int;
  type action =
    | Add(int);
  let createElement = (~children as _, _) =>
    Pure.element({
      ...Pure.reducerComponent("Reducer Component"),
      initialState: () => 2,
      reducer: (action: action, state: state) =>
        switch (action) {
        | Add(value) => Pure.Update(value + state)
        },
      render: _self =>
        <window layout={...defaultLayout, flex: 1}>
          <view layout={...defaultLayout, flex: 1}>
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
        </window>,
    });
};

module Test = {
  type state = {show: bool};

  type action =
    | Display(bool);
  let createElement = (~children as _, _) =>
    Pure.element({
      ...Pure.reducerComponent("Test"),
      initialState: () => {show: true},
      reducer: (action, state) =>
        switch (action) {
        | Display(show) => Pure.Update({show: show})
        },
      render: self =>
          <view layout={...defaultLayout, flex: 1}>
            <button
              title="Click me"
              onClick={() => self.send(Display(!self.state.show))}
              layout={...defaultLayout, flex: 1}
            />
            <view>
              {Pure.string(self.state.show ? "Showing" : "Hidden")}
            </view>
            {
              self.state.show ?
                <button
                  title="Show this button"
                  onClick={() => print_endline("Clicked")}
                  layout={...defaultLayout, flex: 1}
                /> :
                Pure.nil
            }
          </view>
        ,
    });
};