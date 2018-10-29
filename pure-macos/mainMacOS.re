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
          /*
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
                      layout={...defaultLayout, height: 100, width: 200}
                    />
                  )
               |> Pure.list
             }
           </view> */

            <view
              layout={...defaultLayout, flex: 1, flexDirection: Row}
              style={backgroundColor: Some((100., 200., 10., 1.))}>
              <view
                layout={...defaultLayout, flex: 1, margin: 50}
                style={backgroundColor: Some((145., 124., 123., 1.))}
              />
              <view
                layout={...defaultLayout, flex: 1, margin: 50}
                style={backgroundColor: Some((1., 124., 123., 1.))}
              />
            </view>
          </window>,
    });
};
PureMacOS.render(<Test />);