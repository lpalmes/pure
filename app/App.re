open Pure.Primitives;

let white = (255., 255., 255., 1.0);
let textBlack = (56., 68., 77., 1.0);

let backgroundColor = (225., 232., 237., 1.0);
let createElement = (~children as _, _) =>
  Pure.element({
    ...Pure.reducerComponent("Test"),
    render: _self =>
      <window style={Pure.Style.make(~flex=1., ())} title="Pure Native App">
        <view
          style={Pure.Style.make(
            ~flex=1.,
            ~backgroundColor=Rgba(100., 232., 237., 1.0),
            (),
          )}>
          <text
            title="Hello this is a just a text"
            style={Pure.Style.make(~fontSize=120.0, ())}
          />
        </view>
      </window>,
  });