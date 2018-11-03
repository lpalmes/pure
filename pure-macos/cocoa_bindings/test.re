open Cocoa;

let () = {
  let app = Lazy.force(NSApplication.app);
  let f = () => {
    let w = NSWindow.windowWithContentRect(0, (0., 0., 400., 400.));
    let i = ref(0);
    let f = () => {
      print_endline("Window Did Resize");
      incr(i);
      Printf.ksprintf(NSWindow.setTitle(w), "Resized %d times", i^);
    };

    let contentView = NSView.make((0., 0., 400., 400. *. 3.));
    let button = NSButton.make((0., 0., 100., 100.));
    let secondButton = NSButton.make((100., 100., 100., 100.));
    let textView = TextView.make((0., 0., 100., 100.));
    NSView.setBackgroundColor(Obj.magic(textView), 100., 0., 200., 1.);

    let text = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Quisque vitae vulputate nisi. Morbi ut mi luctus, efficitur dui vitae, semper sem. Fusce sagittis purus varius, ornare nisl a, pulvinar dolor. Vivamus eget lacinia magna, a fermentum nibh. Quisque eu faucibus nunc, eget varius magna. Nulla gravida id tellus in posuere. Morbi nisl ipsum, porttitor a interdum at, finibus eget quam. Nullam dapibus massa vitae nisl feugiat pulvinar. Maecenas rutrum lectus vitae aliquam suscipit. Curabitur consectetur lectus at rhoncus accumsan.

    Sed in aliquet tellus. Vestibulum tempus dolor eu nisi luctus, vitae aliquet felis sodales. Sed elementum mollis justo imperdiet lobortis. Sed laoreet diam sit amet dapibus feugiat. Sed molestie, purus id porttitor pulvinar, leo libero condimentum erat, ac gravida orci augue eu felis. Proin nunc mauris, dictum vel facilisis ac, lacinia lobortis sapien. Fusce massa lorem, pellentesque sed nulla in, dapibus fringilla ipsum. Donec fringilla, libero non mattis finibus, ante ipsum malesuada metus, vel cursus ligula leo sed ex. ";
    let attributedString = NSAttributedString.make(text);

    let (width, height) =
      NSAttributedString.measure(attributedString, 100., 0);

    NSView.setRect(Obj.magic(textView), (0., 0., width, height));
    print_endline(string_of_float(width));
    print_endline(string_of_float(height));
    TextView.setText(textView, text);
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
    NSView.addSubview(contentView, Obj.magic(textView));
    let a = NSView.make((0., 0., 400., 400.));
    let scrollView = NSScrollView.make((0., 0., 400., 400.));
    NSScrollView.setDocumentView(scrollView, contentView);
    NSView.addSubview(a, Obj.magic(scrollView));
    NSWindow.addSubview(w, a);
  };
  app#applicationWillFinishLaunching(f);
  app#run;
};