module Make = (Config: ReconcilerSpec.HostConfig) => {
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
    fiberType: Pure.pureElement,
    parent: option(opaqueFiber),
    mutable state: option('state),
    mutable child: option(opaqueFiber),
    mutable sibling: option(opaqueFiber),
    alternate: option(opaqueFiber),
    mutable effectTag: option(effectTag),
    mutable stateNode: option(Config.hostNode),
    mutable effects: list(opaqueFiber),
  }
  and opaqueFiber =
    | Fiber(fiber('state)): opaqueFiber;
  type fiberUpdateHost = {
    node: Config.hostNode,
    children: Pure.pureElement,
  };
  type fiberUpdateComponent = {fiber: opaqueFiber};
  type fiberUpdate =
    | HostRoot(fiberUpdateHost)
    | Component(fiberUpdateComponent);
};