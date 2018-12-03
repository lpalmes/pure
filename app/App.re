open Pure.Primitives;

let white = (255., 255., 255., 1.0);
let textBlack = (56., 68., 77., 1.0);

let backgroundColor = (225., 232., 237., 1.0);

type action =
  | SetMargin(float);
let createElement = (~children as _, _) =>
  Pure.element({
    ...Pure.reducerComponent("Test"),
    initialState: () => 16.0,
    reducer: (action, _state) =>
      switch (action) {
      | SetMargin(margin) => Pure.Types.Update(margin)
      },
    render: self =>
      <window style={Pure.Style.make(~flex=1., ())} title="Pure Native App">
        <view
          style={Pure.Style.make(
            ~flex=1.,
            ~backgroundColor=Rgba(220., 232., 237., 1.0),
            ~margin=self.state,
            (),
          )}>
          <text
            title="Hello this is a just a text"
            style={Pure.Style.make(~fontSize=40.0, ())}
          />
          <text
            title={"Margin " ++ string_of_float(self.state)}
            style={Pure.Style.make(~fontSize=60.0, ())}
          />
          <button
            title="More margin"
            style={Pure.Style.make(~flex=1.0, ())}
            onClick={() => self.send(SetMargin(self.state +. 1.0))}
          />
          <button
            title="Less margin"
            style={Pure.Style.make(~flex=1.0, ())}
            onClick={() => self.send(SetMargin(self.state +. (-1.0)))}
          />
        </view>
      </window>,
  });