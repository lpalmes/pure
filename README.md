# Pure

### Pure stands for Pure Reason

The idea of this project is to have cross-platform Reason code for web, desktop, mobile, that's why Pure was named as i wanted to write pure reason code (thanks to [cherta](https://twitter.com/iamcherta) for the name).

This is an experiment on some ideas i have, i will write some more in the future, but for now this can be seen as a cross platform ui library using each native platform and always providing a great developer experience like instant and consistent hot code reloading.

## Why this exists?

Electron is a successful case of cross platform code between three platforms: MacOS, Linux, Windows and the Web. But such benefits bring costs as high resource usage and most of the time core rewrites in c++ (or another language) to avoid those issues. I believe that React Native has solved this problem by using the platform (iOS and Android for now), and the desktop environments could benefit from this as well. Hence this project, which is an experiment of doing a cross platform ui library to satisfy electron use cases for an lighter alternative using native platform.

## Project structure

- **pure:** this is the bare bones definiton of pure, similar like the react package is agnostic of the renderer and compiles to all platforms
- **reconciler:** this is the fiber-esque reconciler
- **pure-dom:** the dom/web render using the reconciler above (not working now, work in progress in using bucklescript + esy, any help is appreciated :) )
- **pure-macos:** the native render using the reconciler and flex to layout the views. This has a MacOS render for now, but a Linux and Windows is in the works.
- **pure-noop:** a no op reconciler to play around, nothing too important
- **cocoa-ml:** cocoa (macos) bindings, silly name but well, naming is definitely the hardest problem in computer sience
- **pure-ppx:** a ppx that should work like reason react ppx, but nothing working for now
- **specs:** ideas and mechanics of multiple aspects of this project, more than welcome to discuss

## Take it for a spin

### Before running it

You should have [esy](https://esy.sh/) installed, and then run `esy install`, first time installing it might take a while if you haven't played around with esy.

### Native

```
esy x main
or
esy dune exec main
```

### Browser

```
broken for now :(
```

If you want to help you are more than welcome to do so. This is an experiment that i hope it turns into a viable route, i believe we deserve a better alternative for the desktop.

## Credit and inspirations

- [bsb native](https://github.com/bsansouci/bsb-native)
- [esy](https://esy.sh/)
- [react native web](https://github.com/necolas/react-native-web)
- [reasongl](https://github.com/bsansouci/reasongl) (for the cross platform code)
- [didact](https://github.com/hexacta/didact) by [pomber](https://twitter.com/pomber)
