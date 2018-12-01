open Cocoa;

type hostNative =
  | Window(nsWindow)
  | View(nsView)
  | ImageView(NSImageView.imageView)
  | Text(TextView.textView)
  | Button(nsButton)
  | ScrollView(NSScrollView.nsScrollView);
/* (Cocoa View, reference id) */
type hostNativeView = (hostNative, int);