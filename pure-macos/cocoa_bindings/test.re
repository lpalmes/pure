open Cocoa;

let () = {
  let app = Lazy.force(NSApplication.app);
  let f = () => {
    let w = NSWindow.windowWithContentRect(0, (0., 0., 400., 400.));
    let i = ref(0);
    let f = () => {
      log("Window Did Resize");
      incr(i);
      Printf.ksprintf(NSWindow.setTitle(w), "Resized %d times", i^);
    };

    let contentView = NSView.make((0., 0., 400., 400. *. 3.));
    let button = NSButton.make((0., 0., 100., 100.));
    let secondButton = NSButton.make((100., 100., 100., 100.));
    NSButton.setTitle(button, "Click me");
    NSButton.setCallback(
      button,
      () => {
        print_endline("Relayouuuuutl");
        NSView.removeFromSuperview(Obj.magic(button));
        NSView.setRect(contentView, (0., 0., 400., 200.));
      },
    );
    NSButton.setCallback(secondButton, () =>
      print_endline("Second callback")
    );
    NSWindow.windowDidResize(f);
    NSWindow.center(w);
    NSWindow.makeKeyAndOrderFront(w);
    NSView.setBackgroundColor(contentView, 102., 123., 133., 1.);
    NSView.addSubview(contentView, Obj.magic(button));
    NSView.addSubview(contentView, Obj.magic(secondButton));
    let a = NSView.make((0., 0., 400., 400.));
    let scrollView = NSScrollView.make((0., 0., 400., 400.));
    NSScrollView.setDocumentView(scrollView, contentView);
    NSView.addSubview(a, Obj.magic(scrollView));
    NSWindow.addSubview(w, a);
  };
  app#applicationWillFinishLaunching(f);
  app#run;
};