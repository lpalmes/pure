open Rereact;

module Make = (Config: ReconcilerSpec.HostConfig) => {
  module FiberTypes = FiberTypes.Make(Config);
  module Layout = ReconcilerLayout.Make(Config);
  open FiberTypes;
  let updateQueue: ref(list(fiberUpdate)) = ref([]);
  let nextUnitOfWork: ref(option(opaqueFiber)) = ref(None);
  let fiberRoot: ref(option(opaqueFiber)) = ref(None);
  let pendingCommit: ref(option(opaqueFiber)) = ref(None);
  type worker = {mutable work: unit => unit};
  let globalWorker = {work: () => ()};
  let reconcileChildrenArray = (Fiber(wipFiber), elements: list(reactElement)) => {
    let index = ref(0);
    let oldFiber =
      switch wipFiber.alternate {
      | Some(Fiber({child})) => ref(child)
      | None => ref(None)
      };
    let newFiber: ref(option(opaqueFiber)) = ref(None);
    while (index^ < List.length(elements) || oldFiber^ != None) {
      let prevFiber = newFiber^;
      let element =
        switch (List.nth(elements, index^)) {
        | item => Some(item)
        | exception (Failure(_)) => None
        };
      switch (oldFiber^, element) {
      | (None, Some(elm)) =>
        newFiber :=
          Some(
            Fiber({
              tag:
                switch elm {
                | Flat(Component(_)) => Component
                | Flat(String(_)) => Host
                | Flat(Nil) => Host
                | Nested(_, _, _) => Host
                },
              state: None,
              fiberType: elm,
              parent: Some(Fiber(wipFiber)),
              child: None,
              sibling: None,
              alternate: None,
              effectTag: Some(Placement),
              effects: [],
              stateNode: None
            })
          )
      | (Some(Fiber(oldFiber)), None) =>
        oldFiber.effectTag = Some(Deletion);
        wipFiber.effects = wipFiber.effects @ [Fiber(oldFiber)];
      | (Some(Fiber(oldFiber)), Some(elm)) =>
        let sameType =
          switch (oldFiber.fiberType, elm) {
          | (Flat(Component(_)), Flat(Component(_))) => true
          | (Flat(String(a)), Flat(String(b))) when a == b => true
          | (Flat(Nil), Flat(Nil)) => true
          | (Nested(a, _, _), Nested(b, _, _)) when a == b => true
          | _ => false
          };
        if (sameType) {
          newFiber :=
            Some(
              Fiber({
                tag:
                  switch elm {
                  | Flat(Component(_)) => Component
                  | Flat(String(_)) => Host
                  | Flat(Nil) => Host
                  | Nested(_, _, _) => Host
                  },
                state: None,
                fiberType: elm,
                parent: Some(Fiber(wipFiber)),
                child: None,
                sibling: None,
                alternate: Some(Fiber(oldFiber)),
                effectTag: Some(Update),
                effects: [],
                stateNode: oldFiber.stateNode
              })
            );
        } else {
          newFiber :=
            Some(
              Fiber({
                tag:
                  switch elm {
                  | Flat(Component(_)) => Component
                  | Flat(String(_)) => Host
                  | Flat(Nil) => Host
                  | Nested(_, _, _) => Host
                  },
                state: None,
                fiberType: elm,
                parent: Some(Fiber(wipFiber)),
                child: None,
                sibling: None,
                alternate: None,
                effectTag: Some(Placement),
                effects: [],
                stateNode: None
              })
            );
          oldFiber.effectTag = Some(Deletion);
          wipFiber.effects = wipFiber.effects @ [Fiber(oldFiber)];
        };
      | _ => ()
      };
      switch oldFiber^ {
      | Some(Fiber(old)) => oldFiber := old.sibling
      | None => ()
      };
      if (index^ == 0) {
        wipFiber.child = newFiber^;
      } else {
        switch (prevFiber, element) {
        | (Some(Fiber(prev)), Some(_)) => prev.sibling = newFiber^
        | _ => ()
        };
      };
      index := index^ + 1;
    };
  };
  let createSelf = f : self('state, 'action) =>
    switch f {
    | Fiber({fiberType: Flat(Component(component)), state: Some(state)} as fiber) => {
        state: Obj.magic(state),
        send: action => {
          let newState = component.reducer(Obj.magic(action), Obj.magic(state));
          fiber.state = (
            switch newState {
            | NoUpdate => Some(state)
            | Update(state) => Some(Obj.magic(state))
            }
          );
          updateQueue := updateQueue^ @ [Component({fiber: Fiber(fiber)})];
          globalWorker.work();
        }
      }
    | _ => assert false
    };
  let updateComponent = (Fiber(wipFiber) as fiber) =>
    switch wipFiber.fiberType {
    | Flat(Component(component)) =>
      switch wipFiber {
      | {alternate: Some(Fiber({state}))} => wipFiber.state = Obj.magic(state)
      | _ => wipFiber.state = Some(Obj.magic(component.initialState()))
      };
      let self = createSelf(fiber);
      let element = component.render(self);
      reconcileChildrenArray(Fiber(wipFiber), [element]);
    | _ => ()
    };
  let updateHost = (Fiber(wipFiber)) =>
    switch wipFiber.fiberType {
    | Nested(_, _, elements) =>
      switch wipFiber.stateNode {
      | None =>
        let node = Config.createInstance(wipFiber.fiberType);
        wipFiber.stateNode = Some(node);
      | Some(_) => ()
      };
      reconcileChildrenArray(Fiber(wipFiber), elements);
    | Flat(Component(_)) => updateComponent(Fiber(wipFiber))
    | Flat(String(value)) =>
      switch wipFiber.stateNode {
      | None =>
        let node = Config.createTextInstance(value);
        wipFiber.stateNode = Some(node);
      | Some(_) => ()
      }
    | Flat(Nil) => ()
    };
  let beginWork = (Fiber(wipFiber)) =>
    switch wipFiber.tag {
    | Host
    | HostRoot => updateHost(Fiber(wipFiber))
    | Component => updateComponent(Fiber(wipFiber))
    };
  let commitDeletion = (fiber, domParent) => {
    let node = ref(Some(fiber));
    let break = ref(false);
    while (break^ == false) {
      switch node^ {
      | Some(Fiber({tag: Component, child})) => node := child
      | Some(Fiber({stateNode: Some(domNode), sibling}) as fiberNode) =>
        Config.removeChild(domParent, domNode);
        let internalBreak = ref(false);
        while (internalBreak^ == false) {
          switch node^ {
          | Some(Fiber(internalNode) as n) =>
            if (n !== fiber) {
              internalBreak := true;
            };
            if (internalNode.sibling != None) {
              internalBreak := true;
            };
            node := internalNode.parent;
          | None => internalBreak := true
          };
        };
        if (fiberNode == fiber) {
          break := true;
        };
        node := sibling;
      | _ => break := true
      };
    };
  };
  let commitWork = (Fiber(fiber)) =>
    switch fiber.tag {
    | HostRoot => ()
    | _ =>
      switch fiber.parent {
      | None => ()
      | Some(Fiber(parent)) =>
        let parentFiber = ref(parent);
        while (parentFiber.contents.tag == Component) {
          switch parentFiber.contents.parent {
          | Some(Fiber(p)) => parentFiber := Obj.magic(p)
          | None => ()
          };
        };
        switch parentFiber.contents.stateNode {
        | None => ()
        | Some(domParent) =>
          switch fiber.effectTag {
          | Some(Placement) =>
            switch fiber.stateNode {
            | Some(node) => Config.appendChild(domParent, node)
            | None => ()
            }
          | Some(Update) =>
            switch fiber {
            | {
                fiberType: Nested(_, props, _),
                stateNode: Some(node),
                alternate: Some(Fiber({fiberType: Nested(_, oldProps, _)}))
              } =>
              Config.commitUpdate(node, Some(oldProps), props)
            | {fiberType: Nested(_, props, _), stateNode: Some(node), alternate: None} =>
              Config.commitUpdate(node, None, props)
            | _ => ()
            }
          | Some(Deletion) => commitDeletion(Fiber(fiber), domParent)
          | None => ()
          }
        };
      }
    };
  let commitAllWork = fiber => {
    List.iter(commitWork, fiber.effects);
    nextUnitOfWork := None;
    pendingCommit := None;
    fiberRoot := Some(Fiber(fiber));
  };
  let completeWork = fiber =>
    switch fiber.parent {
    | Some(Fiber(parent)) =>
      let childEffects = fiber.effects;
      let fiberEffects = fiber.effectTag != None ? [Fiber(fiber)] : [];
      let parentEffects = parent.effects;
      parent.effects = parentEffects @ childEffects @ fiberEffects;
    | None => pendingCommit := Some(Fiber(fiber))
    };
  let perfomUnitOfWork = (Fiber(wipFiber)) : option(opaqueFiber) => {
    beginWork(Fiber(wipFiber));
    switch wipFiber.child {
    | Some(Fiber(_)) => wipFiber.child
    | None =>
      let unitOfWork = ref(Some(Fiber(wipFiber)));
      let returnUnitOfWork: ref(option(opaqueFiber)) = ref(None);
      while (unitOfWork^ != None) {
        switch unitOfWork^ {
        | Some(Fiber(unit)) =>
          completeWork(unit);
          switch unit.sibling {
          | Some(sibiling) =>
            unitOfWork := None;
            returnUnitOfWork := Some(sibiling);
          | None => unitOfWork := unit.parent
          };
        | _ => ()
        };
      };
      returnUnitOfWork^;
    };
  };
  let getRoot = fiber => {
    let node = ref(fiber);
    while (node.contents.parent != None) {
      switch node.contents.parent {
      | Some(parent) => node := Obj.magic(parent)
      | _ => ()
      };
    };
    node^;
  };
  let resetNextUnitOfWork = () => {
    let update =
      switch updateQueue^ {
      | [update] =>
        updateQueue := [];
        Some(update);
      | [update, ...data] =>
        updateQueue := data;
        Some(update);
      | [] => None
      };
    switch update {
    | Some(HostRoot(update)) =>
      nextUnitOfWork :=
        Some(
          Fiber({
            tag: HostRoot,
            fiberType: update.children,
            state: None,
            parent: None,
            child: None,
            sibling: None,
            alternate: fiberRoot^,
            effectTag: None,
            effects: [],
            stateNode: Some(update.node)
          })
        )
    | Some(Component({fiber: Fiber(fiber)})) =>
      let rootFiber = getRoot(fiber);
      nextUnitOfWork :=
        Some(
          Fiber({
            tag: HostRoot,
            fiberType: rootFiber.fiberType,
            state: rootFiber.state,
            parent: None,
            child: None,
            sibling: None,
            alternate: Some(Fiber(rootFiber)),
            effectTag: None,
            effects: [],
            stateNode: rootFiber.stateNode
          })
        );
    | None => ()
    };
  };
  let perfomLayout = (width, height) =>
    switch fiberRoot^ {
    | Some(f) => Layout.layout(f, width, height)
    | None => ()
    };
  let workLoop = () => {
    switch nextUnitOfWork^ {
    | None => resetNextUnitOfWork()
    | _ => ()
    };
    while (nextUnitOfWork^ != None) {
      nextUnitOfWork :=
        (
          switch nextUnitOfWork^ {
          | Some(unitOfWork) => perfomUnitOfWork(unitOfWork)
          | None => None
          }
        );
    };
    switch pendingCommit^ {
    | Some(Fiber(pendingCommit)) =>
      commitAllWork(pendingCommit);
      perfomLayout(400, 400);
    | None => ()
    };
  };
  let rec perfomWork = () => {
    workLoop();
    let moreWork =
      switch nextUnitOfWork^ {
      | Some(_) => true
      | None => false
      };
    globalWorker.work = perfomWork;
    if (moreWork || List.length(updateQueue^) > 0) {
      perfomWork();
    };
  };
};