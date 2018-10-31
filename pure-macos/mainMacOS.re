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
        <window layout={...defaultLayout, flex: 1}>
          <view layout={...defaultLayout, flex: 1, flexDirection: Row}>
            <button
              title="Add elements"
              onClick={() => self.send(Add)}
              layout={
                ...defaultLayout,
                flex: 1,
                width: 100,
                height: 100,
                maxWidth: 200,
              }
            />
            {
              Array.make(1, 1)
              |> Array.to_list
              |> List.map(_ =>
                   <scrollView
                     layout={...defaultLayout, flex: 1, overflow: Scroll}>
                     <view
                       style={backgroundColor: Some((0., 124., 123., 1.))}
                       layout={...defaultLayout, padding: 50}>
                       {
                         self.state.elements
                         |> List.map(i =>
                              <view
                                layout={
                                  ...defaultLayout,
                                  margin: 10,
                                  height: 200,
                                }
                                style={
                                  backgroundColor:
                                    Some((145., 124., 123., 1.)),
                                }
                              />
                            )
                         |> Pure.list
                       }
                     </view>
                   </scrollView>
                 )
              |> Pure.list
            }
          </view>
        </window>,
    });
};
PureMacOS.render(<Test />);