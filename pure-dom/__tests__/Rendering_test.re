open Jest;

open PurePrimitives;

describe("Rendering", () => {
  open Expect;
  let container =
    Webapi.Dom.Document.createElement("div", Webapi.Dom.document);
  test("Should render a proper <div></div>", () => {
    PureDom.render(<view />, Obj.magic(container));
    let inner = Webapi.Dom.Document.innerHTML(Obj.magic(container));
    Js.log(inner);
    expect(inner) |> toBe("<div><div></div></div>");
  });
});

describe("Expect", () =>
  Expect.(test("toBe", () =>
            expect(1 + 2) |> toBe(3)
          ))
);

describe("Expect.Operators", () => {
  open Expect;
  open! Expect.Operators;
  test("==", () =>
    expect(1 + 2) === 3
  );
});