module type Middleware = {
  type node;
  let middleware: node => unit;
  let createNode: unit => option(node);
};

module NoopMiddleware: Middleware = {
  type node = unit;
  let middleware = _ => ();
  let createNode = () => None;
};