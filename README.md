# Rereact Native

This is an experiment on some ideas i have, i will write some more in the future, but for now this can be seen as a cross platform ui library using each native platform and always providing a great developer experience like instant and consistent hot code reloading.

## Why this exists?

Electron is a successful case of cross platform code between three platforms: MacOS, Linux, Windows and the Web. But such benefits bring costs as high resource usage and most of the time core rewrites in c++ (or another language) to avoid those issues. I belive that React Native has solved this problem by using the platform (iOS and Android for now), and the desktop environments could benefit from this as well. Hence this project, which is an experiment of doing a cross platform ui library to satisfy electron use cases for an lighter alternative using native platform.

## Project structure

The structure is similar to react:

* **src:** this is cross platform code between native and js. Includes the reconciler and layout (flex)
* **src_dom:** the dom/web render using the reconciler above
* **src_native:** the native render using the same reconciler. This has a MacOS render for now, but a Linux and Windows is in the works.

## Take it for a spin

```
npm install
npm run build
npm run run-app
```

If you want to help you are more than welcome to do so. This is an experiment that i hope it turns into a viable route, i belive we deserve a better alternative for the desktop.

## Credit and inspirations

* [bsb native](https://github.com/bsansouci/bsb-native)
* [react native web](https://github.com/necolas/react-native-web)
* [reasongl](https://github.com/bsansouci/reasongl) (for the cross platform code)
* [didact](https://github.com/hexacta/didact) by [pomber](https://twitter.com/pomber)
