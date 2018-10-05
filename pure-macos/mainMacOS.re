open PurePrimitives;

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
        <window layout={...defaultLayout, flex: 1}>
          <view layout={...defaultLayout, flex: 1}>
            <button
              title={self.state.show ? "Hide the button" : "Show the button"}
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
        </window>,
    });
};

PureMacOS.render(<Sample.Flex />);