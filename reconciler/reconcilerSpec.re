type cssLayout = {
  left: float,
  top: float,
  right: float,
  bottom: float,
  width: float,
  height: float,
};

/* module type HostConfig = {
     type hostNode;
     let createInstance: pureElement => hostNode;
     let createTextInstance: string => hostNode;
     let commitUpdate: (hostNode, option(props), props) => unit;
     let appendChild: (hostNode, hostNode) => unit;
     let removeChild: (hostNode, hostNode) => unit;
   };

   /* module LayoutMiddleware: Middleware = {
        type node = LayoutTypes.No
      }; */

   module type Middleware = {
     type node;
     let middleware: node => unit;
     let createNode: unit => option(node);
   };

   module NoopMiddleware: Middleware = {
     type node = unit;
     let middleware = _ => ();
     let createNode = () => None;
   }; */