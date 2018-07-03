/* open GMain;

   open GdkKeysyms;

   let locale = GtkMain.Main.init();

   let main = () => {
     let window =
       GWindow.window(
         ~width=320,
         ~height=240,
         ~title="Simple lablgtk program",
         (),
       );

     let fixed = GPack.fixed(~width=320, ~height=240, ~packing=window#add, ());
     window#connect#destroy(~callback=Main.quit);
     let button = GButton.button(~label="Push me!", ());
     button#connect#clicked(~callback=() => prerr_endline("Ouch!"));
     fixed#put(Obj.magic(button), 100, 50);
     window#show();
     Main.main();
   };

   let () = main(); */