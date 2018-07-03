open Pure;

type cssLayout = {
  left: float,
  top: float,
  right: float,
  bottom: float,
  width: float,
  height: float,
};

module type HostConfig = {
  type hostNode;
  let createInstance: pureElement => hostNode;
  let createTextInstance: string => hostNode;
  let commitUpdate: (hostNode, option(props), props) => unit;
  let appendChild: (hostNode, hostNode) => unit;
  let removeChild: (hostNode, hostNode) => unit;
  let applyLayout: (hostNode, cssLayout) => unit;
};