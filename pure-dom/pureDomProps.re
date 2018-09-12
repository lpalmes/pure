open Pure;

let reconcileStringProp =
    (
      domElement: Dom.element,
      prevProps: option(props),
      prop: option(string),
      attributeName: string,
    ) =>
  switch (prevProps, prop) {
  | (None, Some(value)) =>
    Webapi.Dom.Element.setAttribute(attributeName, value, domElement)
  | (Some({src: Some(prevSrc)}), Some(src)) =>
    if (prevSrc != src) {
      Webapi.Dom.Element.setAttribute(attributeName, src, domElement);
    }
  | (Some({src: None}), Some(src)) =>
    Webapi.Dom.Element.setAttribute(attributeName, src, domElement)
  | (_, None) =>
    Webapi.Dom.Element.removeAttribute(attributeName, domElement)
  };

let reconcile =
    (domElement: Dom.element, prevProps: option(props), props: props) => {
  reconcileStringProp(domElement, prevProps, props.id, "id");
  reconcileStringProp(domElement, prevProps, props.src, "src");
  reconcileStringProp(domElement, prevProps, props.value, "value");
  reconcileStringProp(
    domElement,
    prevProps,
    props.placeholder,
    "placeholder",
  );
  switch (props.disabled) {
  | Some(value) =>
    if (value) {
      Webapi.Dom.Element.setAttribute("disabled", "true", domElement);
    } else {
      Webapi.Dom.Element.removeAttribute("disabled", domElement);
    }
  | None => Webapi.Dom.Element.removeAttribute("disabled", domElement)
  };
  PureDomEvents.handleEvent("onClick", props.onClick, domElement, domElement);
  /* switch (prevProps, props.onClick) {
     | (None, Some(func)) =>
       Js.log("adding event listener");
       Webapi.Dom.Element.addClickEventListener(_ => func(), domElement);
     | (Some({onClick: Some(prevFunc)}), Some(func)) when prevFunc !== func =>
       Webapi.Dom.Element.removeClickEventListener(_ => prevFunc(), domElement);
       Webapi.Dom.Element.addClickEventListener(_ => func(), domElement);
       Js.log("removing and adding new event listener");
     | (Some({onClick: Some(prevFunc)}), None) =>
       Webapi.Dom.Element.removeClickEventListener(_ => prevFunc(), domElement);
       Js.log("removing event listener");
     | _ => ()
     }; */
  switch (prevProps, props.onChangeText) {
  | (None, Some(func)) =>
    Webapi.Dom.Element.addEventListener(
      "input",
      e => Obj.magic(e)##target##value |> func,
      domElement,
    )
  | (Some({onChangeText: Some(prevFunc)}), Some(func))
      when prevFunc !== func =>
    Webapi.Dom.Element.removeEventListener(
      "input",
      e => Obj.magic(e)##target##value |> prevFunc,
      domElement,
    );
    Webapi.Dom.Element.addEventListener(
      "input",
      e => Obj.magic(e)##target##value |> func,
      domElement,
    );
  | (Some({onChangeText: Some(prevFunc)}), None) =>
    Webapi.Dom.Element.removeEventListener(
      "input",
      e => Obj.magic(e)##target##value |> prevFunc,
      domElement,
    )
  | _ => ()
  };
};