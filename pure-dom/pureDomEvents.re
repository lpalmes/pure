type events = {mutable onClick: option(MouseEventRe.t => unit)};

type domEvents = {mutable onClick: option(unit => unit)};

let attachedEvents: events = {onClick: None};

let dispatchEvents =
    (
      event: MouseEventRe.t,
      target: Dom.eventTarget,
      isClick: bool,
      name: string,
    ) => {
  /* Js.log(event); */
  let domEvents: option(domEvents) =
    Js.Dict.get(Obj.magic(target), "pureEvents");
  switch (domEvents) {
  | Some({onClick: Some(onClick)}) => onClick()
  | _ => ()
  };
  /* Js.log(domEvents);
     Js.log(isClick);
     Js.log(name); */
};

let attachEventToDocument = (name: string) => {
  open Webapi.Dom;
  let documentEvent = e => {
    let eventType = MouseEventRe.type_(e);

    let isClick = eventType == "click" || eventType == "dblclick";
    if (isClick && MouseEventRe.button(e) != 0) {
      MouseEventRe.stopPropagation(e);
    };

    let target = MouseEventRe.target(e);

    dispatchEvents(e, target, isClick, name);
  };

  Webapi.Dom.Element.addClickEventListener(
    documentEvent,
    Obj.magic(Webapi.Dom.document),
  );
  documentEvent;
};

let handleEvent =
    (
      name: string,
      eventFunc: option(unit => unit),
      dom: Dom.element,
      rootDom: Dom.element,
    ) => {
  Js.Dict.set(Obj.magic(dom), "pureEvents", {onClick: None});
  let domEvents: option(domEvents) =
    Js.Dict.get(Obj.magic(dom), "pureEvents");
  switch (domEvents) {
  | Some(events) => events.onClick = eventFunc
  | None => ()
  };
  switch (attachedEvents.onClick) {
  | Some(_) => ()
  | None =>
    attachedEvents.onClick = Some(attachEventToDocument("click"));
    Js.log("Attached click event to document");
  };
};