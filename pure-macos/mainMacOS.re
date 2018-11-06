open PurePrimitives;

module Test = {
  type state = {elements: list(int)};
  type action =
    | Add;

  let sayHello = () => print_endline("Hello");
  let createElement = (~children as _, _) =>
    Pure.element({
      ...Pure.reducerComponent("Test"),
      initialState: () => {elements: [1, 2, 3, 10, 20, 41, 14, 125, 23, 1]},
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
                onClick=sayHello
                layout={...defaultLayout, height: 50}
              />
              <text
                title="React Native lets you build mobile apps using only JavaScript. It uses the same design as React, letting you compose a rich mobile UI from declarative components."
                layout={...defaultLayout, margin: 10}
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
                         layout={...defaultLayout, margin: 10}
                         style={
                           backgroundColor: Some((145., 124., 123., 1.)),
                         }>
                         <text
                           title={
                             Array.make(i, "Hello ")
                             |> Array.fold_left((++), "")
                           }
                         />
                       </view>
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

let lyrics = "What will grow crooked, you can't make straight
It's the price that you gotta pay
Do yourself a favor and pack you bags
Buy a ticket and get on the train
Buy a ticket and get on the train
Cause this is fucked up, fucked up
Cause this is fucked up, fucked up
People get crushed like biscuit crumbs
And laid down in the bitumen
You have tried your best to please everyone
But it just isn't happening
No, it just isn't happening
And it's fucked up, fucked up
And this is fucked up, fucked up
This your blind spot, blind spot
It should be obvious, but it's not.
But it isn't, but it isn't
You cannot kick start a dead horse
You just crush yourself and walk away
I don't care what the future holds
Cause I'm right here and I'm today
With your fingers you can touch me
I'm your black swan, black swan
But I made it to the top, made it to the top
This is fucked up, fucked up
You are fucked up, fucked up
This is fucked up, fucked up
Be your black swan, black swan
I'm for spare parts, broken up

let test = () => \"This is a test function\"";

module FontTest = {
  let createElement = (~children as _, _) =>
    Pure.element({
      ...Pure.reducerComponent("Test"),
      render: _ =>
        <window layout={...defaultLayout, flex: 1}>
          <view layout={...defaultLayout, flex: 1, flexDirection: Row}>
            <scrollView
              layout={...defaultLayout, flex: 1, overflow: Scroll, margin: 10}>
              <view>
                <text
                  title=lyrics
                  fontColor=(255., 255., 255., 1.0)
                  fontSize=30.
                />
              </view>
            </scrollView>
          </view>
        </window>,
    });
};
PureMacOS.render(<FontTest />);