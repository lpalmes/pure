open Jest;
open Webapi.Dom;

open PurePrimitives;
let unsafelyUnwrapOption =
  fun
  | Some(v) => v
  | None => raise(Invalid_argument("Passed `None` to unsafelyUnwrapOption"));

describe("Rendering", () => {
  open Expect;

  let body =
    document
    |> Document.asHtmlDocument
    |> unsafelyUnwrapOption
    |> HtmlDocument.body
    |> unsafelyUnwrapOption;

  let container = ref(Document.createElement("div", Webapi.Dom.document));

  beforeEach(() => {
    container := Document.createElement("div", document);
    Element.appendChild(container^, body);
  });

  afterEach(() => {
    Webapi.Dom.Element.removeChild(container^, body) |> ignore;
    PureDom.DOMReconciler.fiberRoot := None;
  });

  test("Should render a proper <div></div>", () => {
    PureDom.render(<view />, Obj.magic(container.contents));
    let inner = Element.innerHTML(container^);
    expect(inner) |> toBe("<div></div>");
  });

  test(
    "Should render <div></div> and then replace for a <button></button>", () => {
    PureDom.render(<view />, Obj.magic(container.contents));
    let inner = Element.innerHTML(container^);
    expect(inner) |> toBe("<div></div>");
    PureDom.render(<button />, Obj.magic(container.contents));
    let inner = Element.innerHTML(container^);
    expect(inner) |> toBe("<button></button>");
  });

  test("Should render a proper <div><div></div></div>", () => {
    PureDom.render(<view> <button /> </view>, Obj.magic(container.contents));
    let inner = Element.innerHTML(container^);
    expect(inner) |> toBe("<div><button></button></div>");
  });

  test(
    "Should render a proper <div><div></div></div> and delete it's child", () => {
    Js.log("Rendering <view><button/> </view>");
    PureDom.render(<view> <button /> </view>, Obj.magic(container.contents));
    Js.log("Rendering <view/>");
    PureDom.render(<view />, Obj.magic(container.contents));
    Js.log("Rendering <view><button/> </view>");
    PureDom.render(<view> <button /> </view>, Obj.magic(container.contents));
    let inner = Element.innerHTML(container^);
    expect(inner) |> toBe("<div><button></button></div>");
  });

  test("Should delete a sibiling node", () => {
    PureDom.render(
      <view> <button /> <view /> </view>,
      Obj.magic(container.contents),
    );
    PureDom.render(<view> <button /> </view>, Obj.magic(container.contents));
    PureDom.render(<view> <button /> </view>, Obj.magic(container.contents));
    let inner = Element.innerHTML(container^);
    expect(inner) |> toBe("<div><button></button></div>");
  });
});

describe("Expect", () =>
  Pure.(
    Expect.(
      test("Nested modules", () =>
        expect(<view> <button> {Pure.string("Hello")} </button> </view>)
        |> toEqual(
             Nested(
               View,
               defaultProps,
               [Nested(Button, defaultProps, [Flat(String("Hello"))])],
             ),
           )
      )
    )
  )
);