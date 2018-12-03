open Pure;
open Types;

module Spec = {
  module type HostConfig = {
    type hostNode;
    let createInstance: Pure.Types.pureElement => hostNode;
    let createTextInstance: string => hostNode;
    let commitUpdate:
      (hostNode, option(Pure.Types.props), Pure.Types.props) => unit;
    let appendChild: (hostNode, hostNode) => unit;
    let removeChild: (hostNode, hostNode) => unit;
    let afterCommit: unit => unit;
  };
};

module Make = (Config: Spec.HostConfig) => {
  type fiberTag =
    | Host
    | Component
    | HostRoot;
  type effectTag =
    | Placement
    | Deletion
    | Update;
  type fiber('state) = {
    tag: fiberTag,
    fiberType: option(Pure.Types.pureElement),
    parent: option(opaqueFiber),
    mutable state: option('state),
    mutable child: option(opaqueFiber),
    mutable sibling: option(opaqueFiber),
    alternate: option(opaqueFiber),
    mutable effectTag: option(effectTag),
    mutable stateNode: option(Config.hostNode),
    mutable effects: list(opaqueFiber),
    /* middlewareNode: option(Middleware.node), */
  }
  and opaqueFiber =
    | Fiber(fiber('state)): opaqueFiber;
  type fiberUpdateHost = {
    node: option(Config.hostNode),
    children: Pure.Types.pureElement,
  };
  type fiberUpdateComponent = {fiber: opaqueFiber};
  type fiberUpdate =
    | HostRootUpdate(fiberUpdateHost)
    | ComponentUpdate(fiberUpdateComponent);

  /* module Debug = PureDebug.Make(Config, Middleware); */
  let updateQueue: ref(list(fiberUpdate)) = ref([]);
  let nextUnitOfWork: ref(option(opaqueFiber)) = ref(None);
  let fiberRoot: ref(option(opaqueFiber)) = ref(None);
  let pendingCommit: ref(option(opaqueFiber)) = ref(None);
  type worker = {mutable work: unit => unit};
  let globalWorker = {work: () => ()};
  let reconcileChildrenArray =
      (Fiber(wipFiber), elements: list(Pure.Types.pureElement)) => {
    let index = ref(0);
    let oldFiber =
      switch (wipFiber.alternate) {
      | Some(Fiber(fib)) => ref(fib.child)
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

      let sameType =
        switch (oldFiber.contents, element) {
        | (Some(Fiber(oldFiber)), Some(elm)) =>
          let same =
            switch (oldFiber.fiberType, elm) {
            | (Some(Flat(Component(_))), Flat(Component(_))) => true
            | (Some(Flat(String(a))), Flat(String(b))) when a == b => true
            | (Some(Flat(Nil)), Flat(Nil)) => true
            | (Some(Nested(a, _, _)), Nested(b, _, _)) when a == b => true
            | _ => false
            };
          if (same) {
            newFiber :=
              Some(
                Fiber({
                  tag: oldFiber.tag,
                  state: oldFiber.state,
                  fiberType: Some(elm),
                  parent: Some(Fiber(wipFiber)),
                  child: None,
                  sibling: None,
                  alternate: Some(Fiber(oldFiber)),
                  effectTag: Some(Update),
                  effects: [],
                  stateNode: oldFiber.stateNode,
                  /* middlewareNode: None, */
                }),
              );
          };
          same;
        | _ => false
        };

      switch (element, sameType) {
      | (Some(elm), false) =>
        newFiber :=
          Some(
            Fiber({
              tag:
                switch (elm) {
                | Flat(Component(_)) => Component
                | Flat(List(_)) => Host
                | Flat(String(_)) => Host
                | Flat(Nil) => Host
                | Nested(_, _, _) => Host
                },
              state: None,
              fiberType: element,
              parent: Some(Fiber(wipFiber)),
              child: None,
              sibling: None,
              alternate: None,
              effectTag: Some(Placement),
              effects: [],
              stateNode: None,
              /* middlewareNode: None, */
            }),
          )
      | _ => ()
      };

      switch (oldFiber.contents, sameType) {
      | (Some(Fiber(oldFiber)), false) =>
        oldFiber.effectTag = Some(Deletion);
        wipFiber.effects = wipFiber.effects @ [Fiber(oldFiber)];
      | _ => ()
      };

      switch (oldFiber^) {
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
  let createSelf = (f): self('state, 'action) =>
    switch (f) {
    | Fiber(
        {
          fiberType: Some(Flat(Component(component))),
          state: Some(state),
          _,
        } as fiber,
      ) => {
        state: Obj.magic(state),
        send: action => {
          let newState =
            component.reducer(Obj.magic(action), Obj.magic(state));
          fiber.state = (
            switch (newState) {
            | NoUpdate => Some(state)
            | Update(state) => Some(Obj.magic(state))
            }
          );
          updateQueue :=
            updateQueue^ @ [ComponentUpdate({fiber: Fiber(fiber)})];
          globalWorker.work();
        },
      }
    | _ => assert(false)
    };
  let updateComponent = (Fiber(wipFiber) as fiber) =>
    switch (wipFiber.fiberType) {
    | Some(Flat(Component(component))) =>
      switch (wipFiber) {
      | {alternate: Some(Fiber({state, _})), _} =>
        wipFiber.state = Obj.magic(state)
      | _ => wipFiber.state = Some(Obj.magic(component.initialState()))
      };
      let self = createSelf(fiber);
      let element = component.render(self);
      reconcileChildrenArray(Fiber(wipFiber), [element]);
    | _ => ()
    };
  let updateHost = (Fiber(wipFiber) as fiber) =>
    switch (wipFiber.fiberType) {
    | Some(Nested(_, _, elements) as element) =>
      switch (wipFiber.stateNode) {
      | None =>
        let node = Config.createInstance(element);
        wipFiber.stateNode = Some(node);
      | Some(_) => ()
      };
      reconcileChildrenArray(fiber, elements);
    | Some(Flat(List(elements))) => reconcileChildrenArray(fiber, elements)
    | Some(Flat(String(value))) =>
      switch (wipFiber.stateNode) {
      | None =>
        let node = Config.createTextInstance(value);
        wipFiber.stateNode = Some(node);
      | Some(_) => ()
      }
    | Some(Flat(Component(_))) => ()
    | Some(Flat(Nil)) => ()
    | None => ()
    };

  let updateHostRoot = (Fiber(wipFiber) as fiber) =>
    switch (wipFiber.fiberType) {
    | Some(element) => reconcileChildrenArray(fiber, [element])
    | None => assert(false)
    };

  let beginWork = (Fiber(wipFiber) as fiber) =>
    switch (wipFiber.tag) {
    | Host => updateHost(fiber)
    | HostRoot => updateHostRoot(fiber)
    | Component => updateComponent(fiber)
    };
  let commitDeletion = (fiber, domParent) => {
    let node = ref(Some(fiber));
    let break = ref(false);
    while (break^ == false) {
      switch (node^) {
      | Some(Fiber({tag: Component, child, _})) => node := child
      | Some(Fiber({fiberType: Some(Flat(List(_))), child, _})) =>
        node := child
      | Some(Fiber({stateNode: Some(domNode), sibling, _})) =>
        Config.removeChild(domParent, domNode);
        let internalBreak = ref(false);
        while (internalBreak^ == false) {
          switch (node^) {
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
        node := sibling;
      | _ => break := true
      };
    };
  };
  let commitWork = (Fiber(fiber)) =>
    switch (fiber.tag) {
    | HostRoot => ()
    | _ =>
      switch (fiber.parent) {
      | None => ()
      | Some(Fiber(parent)) =>
        let parentFiber = ref(parent);
        switch (parent) {
        | {fiberType: Some(Flat(List(_))), parent: Some(Fiber(p)), _} =>
          parentFiber := Obj.magic(p)
        | _ => ()
        };
        while (parentFiber.contents.tag == Component) {
          switch (parentFiber.contents.parent) {
          | Some(Fiber(p)) => parentFiber := Obj.magic(p)
          | None => ()
          };
        };
        switch (parentFiber.contents.stateNode) {
        | None => ()
        | Some(parentNode) =>
          switch (fiber.effectTag) {
          | Some(Placement) =>
            switch (fiber.stateNode) {
            | Some(node) => Config.appendChild(parentNode, node)
            | None => ()
            }
          | Some(Update) =>
            switch (fiber) {
            | {
                fiberType: Some(Nested(_, props, _)),
                stateNode: Some(node),
                alternate:
                  Some(
                    Fiber({fiberType: Some(Nested(_, oldProps, _)), _}),
                  ),
                _,
              } =>
              Config.commitUpdate(node, Some(oldProps), props)
            | {
                fiberType: Some(Nested(_, props, _)),
                stateNode: Some(node),
                alternate: None,
                _,
              } =>
              Config.commitUpdate(node, None, props)
            | _ => ()
            }
          | Some(Deletion) => commitDeletion(Fiber(fiber), parentNode)
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
    Config.afterCommit();
    /* Middleware.apply(Obj.magic(fiber)); */
  };
  let completeWork = fiber =>
    switch (fiber.parent) {
    | Some(Fiber(parent)) =>
      let childEffects = fiber.effects;
      let fiberEffects = fiber.effectTag != None ? [Fiber(fiber)] : [];
      let parentEffects = parent.effects;
      parent.effects = parentEffects @ childEffects @ fiberEffects;
    | None => pendingCommit := Some(Fiber(fiber))
    };
  let perfomUnitOfWork = (Fiber(wipFiber)): option(opaqueFiber) => {
    beginWork(Fiber(wipFiber));
    switch (wipFiber.child) {
    | Some(Fiber(_)) => wipFiber.child
    | None =>
      let unitOfWork = ref(Some(Fiber(wipFiber)));
      let returnUnitOfWork: ref(option(opaqueFiber)) = ref(None);
      while (unitOfWork^ != None) {
        switch (unitOfWork.contents) {
        | Some(Fiber(unit)) =>
          completeWork(unit);
          switch (unit.sibling) {
          | Some(sibiling) =>
            unitOfWork := None;
            returnUnitOfWork := Some(sibiling);
          | None => unitOfWork := unit.parent
          };
        | None => ()
        };
      };
      returnUnitOfWork.contents;
    };
  };
  let getRoot = fiber => {
    let node = ref(fiber);
    let loop = ref(0);
    while (node.contents.parent != None && loop.contents < 3000) {
      switch (node.contents.parent) {
      | Some(Fiber(parent)) => node.contents = Obj.magic(parent)
      | _ => ()
      };
      loop.contents = loop.contents + 1;
    };
    node.contents;
  };
  let resetNextUnitOfWork = () => {
    let update =
      switch (updateQueue^) {
      | [update] =>
        updateQueue := [];
        Some(update);
      | [update, ...data] =>
        updateQueue := data;
        Some(update);
      | [] => None
      };
    switch (update) {
    | Some(HostRootUpdate(update)) =>
      nextUnitOfWork :=
        Some(
          Fiber({
            tag: HostRoot,
            fiberType: Some(update.children),
            state: None,
            parent: None,
            child: None,
            sibling: None,
            alternate: fiberRoot^,
            effectTag: None,
            effects: [],
            stateNode: update.node,
            /* middlewareNode: None, */
          }),
        )
    | Some(ComponentUpdate({fiber: Fiber(fiber)})) =>
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
            stateNode: rootFiber.stateNode,
            /* middlewareNode: None, */
          }),
        );
    | None => ()
    };
  };

  let rec countAlternateFibers = (Fiber(fiber), count) =>
    switch (fiber.alternate) {
    | Some(f) => countAlternateFibers(f, count + 1)
    | None => count
    };
  let workLoop = () => {
    switch (nextUnitOfWork^) {
    | None => resetNextUnitOfWork()
    | _ => ()
    };
    while (nextUnitOfWork^ != None) {
      nextUnitOfWork :=
        (
          switch (nextUnitOfWork^) {
          | Some(unitOfWork) => perfomUnitOfWork(unitOfWork)
          | None => None
          }
        );
    };
    switch (pendingCommit^) {
    | Some(Fiber(pendingCommit)) => commitAllWork(pendingCommit)
    | None => ()
    };
  };
  let rec perfomWork = () => {
    workLoop();
    let moreWork =
      switch (nextUnitOfWork^) {
      | Some(_) => true
      | None => false
      };

    /* switch (fiberRoot.contents) {
       | Some(f) =>
         f |> Debug.printTree |> print_endline;
         let Fiber(fiber) = f;
         switch (fiber.alternate) {
         | Some(alternate) =>
           "Alternate: \n\n" ++ (alternate |> Debug.printTree) |> print_endline
         | None => print_endline("No alternate fiber")
         };
       | None => ()
       }; */
    if (moreWork || List.length(updateQueue^) > 0) {
      perfomWork();
    };
  };

  globalWorker.work = perfomWork;

  let renderWithRoot = (element, rootNode) => {
    updateQueue :=
      updateQueue^
      @ [HostRootUpdate({node: Some(rootNode), children: element})];
    perfomWork();
  };
  let render = element => {
    updateQueue :=
      updateQueue^ @ [HostRootUpdate({node: None, children: element})];
    perfomWork();
  };
};