open PurePrimitives;

module Test = {
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
        <window layout={...defaultLayout, flex: 1, width: 200, height: 300}>
          <view
            layout={...defaultLayout, flex: 1}
            style={backgroundColor: Some((0., 0., 0., 1.))}>
            <button
              title="Add elements"
              onClick={() => self.send(Add)}
              layout={...defaultLayout, flex: 1, width: 100, height: 100}
            />
            {
              self.state.elements
              |> List.map(i =>
                   <view
                     layout={...defaultLayout, margin: 10, flex: 1}
                     style={backgroundColor: Some((145., 124., 123., 1.))}
                   />
                 )
              |> Pure.list
            }
          </view>
        </window>,
    });
};
PureMacOS.render(<Test />);