type layoutNode = {
  left: float,
  top: float,
  width: float,
  height: float,
};

module type HostConfig = {
  include Reconciler.Spec.HostConfig;
  let setPerfomLayout: (hostNode, (float, float) => unit) => unit;
  let getDimensions: hostNode => (float, float);
  let nullContextView: unit => hostNode;
  let applyLayout: (hostNode, layoutNode) => unit;
  let textMeasure:
    (hostNode, Pure.Types.pureElement) => option(float => (float, float));
};