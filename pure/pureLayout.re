module Node = {
  type context = unit;
  let nullContext = ();
};

include Flex.Layout.Create(Node, Flex.FloatEncoding);