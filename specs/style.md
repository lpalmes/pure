# Style

Pure needs a style primitive for all host platforms to share styling and layout like [React Native does](https://facebook.github.io/react-native/docs/style), that ultimatley gets transformed to a [cssStyle record](https://github.com/jordwalke/flex/blob/master/src/lib/LayoutTypes.re#L88).

This style solution should also include properties that are not layout related and are not enumerated in the cssStyle record (eg: backgroundColor, borderRadius, etc).

## What we have in React Native Styles

We have a couple of styles divided in primitives:

- [View](https://facebook.github.io/react-native/docs/view-style-props)
- [Text](https://facebook.github.io/react-native/docs/text-style-props)
- [Image](https://facebook.github.io/react-native/docs/image-style-props)

## Other solutions (Js or Reason)

- [jsxstyle](https://github.com/lpalmes/jsxnice)
- [styled-components](https://github.com/styled-components/styled-components)
- [bs-nice](https://github.com/threepointone/bs-nice)
- [ReasonReact Style](https://reasonml.github.io/reason-react/docs/en/style)

## Style module

This should be the primitive for all style libraries and the primary way to communicate with the style api.
This will include the primary `style` that will be used by the Flex library, and hosts implementations to implement styling.

There is a small implementation ongoing over here [style](https://github.com/lpalmes/pure/blob/master/pure/style.re).

The simplest operation would be a `Style.make` function with labeled default arguments.

```reason
let style = Style.make(~backgroundColor=Blue, ~justifyContent=SpaceAround):

<View style>
    <Text>
        {Pure.string("Hello, this text box will be blue")}
    </Text>
    <Text>
        {Pure.string("And will have multiple children space around")}
    </Text>
</View>
```

Style is a record, so it would be sensible to think we can combine both records by spreading them into a new one, but in fact this will be a compiler error.
So `Style.merge` comes into play. The second argument properties will always override the first.

```reason
let a = Style.make(~backgroundColor=Blue, ~justifyContent=SpaceAround);
let b = Style.make(~color=White, ~backgroundColor=Yellow, ~flex=3);
let c = Style.merge(a,b);

<View style={Style.merge(a, Style.make(~backgroundColor=Green))}>
```

`Style.make` can create styles by using a labeled default argument, but it might feel quite verbose to write them so a great piece to steal... get some inspiration , can be the great bs-nice list style definition that can be transformed from `styleList` to `style`.

```reason
let style = Style.lstyle([
  Display(Flex),
  JustifyContent(FlexStart),
  AlignItems(Center),
  Color(Red),
]);
```
