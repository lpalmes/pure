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
            <view layout={...defaultLayout, flex: 1}>
              <button
                title="Add tweet"
                onClick={() => self.send(Add)}
                layout={...defaultLayout, height: 50}
              />
              <button
                title="Add column"
                onClick={() => self.send(Add)}
                layout={...defaultLayout, height: 50}
              />
            </view>
            <scrollView
              layout={...defaultLayout, flex: 1, overflow: Scroll, margin: 10}>
              <view style={backgroundColor: Some((0., 124., 123., 1.))}>
                {
                  Array.make(20, 1)
                  |> Array.to_list
                  |> List.map(i =>
                       <view
                         layout={...defaultLayout, margin: 10, height: 50}
                         style={
                           backgroundColor: Some((145., 124., 123., 1.)),
                         }
                       />
                     )
                  |> Pure.list
                }
              </view>
            </scrollView>
            <scrollView
              layout={...defaultLayout, flex: 1, overflow: Scroll, margin: 10}>
              <view style={backgroundColor: Some((0., 124., 123., 1.))}>
                {
                  self.state.elements
                  |> List.map(i =>
                       <view
                         layout={...defaultLayout, margin: 10, height: 50}
                         style={
                           backgroundColor: Some((145., 124., 123., 1.)),
                         }
                       />
                     )
                  |> Pure.list
                }
              </view>
            </scrollView>
            <scrollView
              layout={...defaultLayout, flex: 1, overflow: Scroll, margin: 10}>
              <view style={backgroundColor: Some((0., 124., 123., 1.))}>
                {
                  Array.make(7, 1)
                  |> Array.to_list
                  |> List.map(i =>
                       <view
                         layout={...defaultLayout, margin: 10, height: 50}
                         style={
                           backgroundColor: Some((145., 124., 123., 1.)),
                         }
                       />
                     )
                  |> Pure.list
                }
              </view>
            </scrollView>
          </view>
        </window>,
    });
};
PureMacOS.render(<Test />);