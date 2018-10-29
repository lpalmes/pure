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
              style={backgroundColor: Some((0., 150., 0., 0.2))}
            />
            <view
              layout={...defaultLayout, flex: 1, margin: 16}
              style={backgroundColor: Some((0., 0., 150., 0.2))}
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
          <view>
            <button
              title="Click me"
              onClick={() => self.send(Display(!self.state.show))}
              layout={...defaultLayout, flex: 1}
            />
            <view>
              {Pure.string(self.state.show ? "Showing" : "Hidden")}
            </view>
            <button
              title="Show this cool button"
              onClick={() => print_endline("Clicked")}
              layout={...defaultLayout, flex: 1}
            />
          </view>
          <view>
            {
              self.state.show ?
                <>
                  <view> {Pure.string("Hello my friend")} </view>
                  <view> {Pure.string("Hello again my friend")} </view>
                  <view> {Pure.string("You again my friend")} </view>
                </>
                |> Pure.list :
                <button onClick={() => print_endline("What's up my friend")}>
                  {Pure.string("Another button :)")}
                </button>
            }
          </view>
        </view>,
    });
};

module ListTest = {
  type state = {elements: list(int)};
  type action =
    | Add;
  let createElement = (~children as _, _) =>
    Pure.element({
      ...Pure.reducerComponent("Test"),
      initialState: () => {elements: [1, 2, 3]},
      reducer: (action, state) =>
        switch (action) {
        | Add =>
          Pure.Update({
            elements:
              List.append(
                state.elements,
                [List.length(state.elements) + 1],
              ),
          })
        },
      render: self =>
        <window layout={...defaultLayout, flex: 1}>
          <view layout={...defaultLayout, flex: 1}>
            <button
              title="Add elements"
              onClick={() => self.send(Add)}
              layout={...defaultLayout, flex: 1}
            />
            {
              self.state.elements
              |> List.map(i =>
                   <button
                     title={"Show button " ++ string_of_int(i)}
                     onClick={() => i |> string_of_int |> print_endline}
                     layout={
                       ...defaultLayout,
                       top: 20,
                       height: 100,
                       width: 200,
                     }
                   />
                 )
              |> Pure.list
            }
          </view>
        </window>,
    });
};