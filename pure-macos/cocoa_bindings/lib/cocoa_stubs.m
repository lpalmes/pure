#include <Cocoa/Cocoa.h>

#define CAML_NAME_SPACE

#include <caml/mlvalues.h>
#include <caml/memory.h>
#include <caml/callback.h>
#include <caml/alloc.h>
#include <caml/fail.h>

CAMLprim value ml_NSLog (value str)
{
  CAMLparam1 (str);
  NSLog (@"%s", String_val (str));
  CAMLreturn (Val_unit);
}

@interface MLApplicationDelegate : NSObject <NSApplicationDelegate>

@end

enum {
  ApplicationWillFinishLaunching
};

@implementation MLApplicationDelegate
{
  value applicationId;
}

- (instancetype)initWithId:(value)appId
{
  if (self = [super init]) {
    applicationId = appId;
  }
  return self;
}

- (void)applicationWillFinishLaunching:(NSNotification *)__unused not
{
  static value *closure_f = NULL;

  if (closure_f == NULL)
    closure_f = caml_named_value ("[NSApp delegate]");

  if (closure_f != NULL)
    caml_callback2 (*closure_f, applicationId, Val_int (ApplicationWillFinishLaunching));

  // NSLog (@"applicationWillFinishLaunching");

  id menubar = [NSMenu new];
  id appMenuItem = [NSMenuItem new];
  [menubar addItem:appMenuItem];
  [NSApp setMainMenu:menubar];
  id appMenu = [NSMenu new];
  id appName = [[NSProcessInfo processInfo] processName];
  id aboutMenuItem = [[NSMenuItem alloc] initWithTitle:[@"About " stringByAppendingString:appName]
                                                action:@selector(orderFrontStandardAboutPanel:)
                                         keyEquivalent:@""];
  id hideMenuItem = [[NSMenuItem alloc] initWithTitle:[@"Hide " stringByAppendingString:appName]
                                               action:@selector(hide:)
                                        keyEquivalent:@"h"];
  id hideOthersMenuItem = [[NSMenuItem alloc] initWithTitle:@"Hide Others"
                                                     action:@selector(hideOtherApplications:)
                                              keyEquivalent:@"h"];
  [hideOthersMenuItem setKeyEquivalentModifierMask:(NSEventModifierFlagOption | NSEventModifierFlagCommand)];
  id showAllMenuItem = [[NSMenuItem alloc] initWithTitle:@"Show All"
                                                  action:@selector(unhideAllApplications:)
                                           keyEquivalent:@""];
  id quitMenuItem = [[NSMenuItem alloc] initWithTitle:[@"Quit " stringByAppendingString:appName]
                                               action:@selector(terminate:)
                                        keyEquivalent:@"q"];
  [appMenu addItem:aboutMenuItem];
  [appMenu addItem:[NSMenuItem separatorItem]];
  [appMenu addItem:hideMenuItem];
  [appMenu addItem:hideOthersMenuItem];
  [appMenu addItem:showAllMenuItem];
  [appMenu addItem:[NSMenuItem separatorItem]];
  [appMenu addItem:quitMenuItem];
  [appMenuItem setSubmenu:appMenu];
}

@end

@interface MLWindowDelegate : NSObject <NSWindowDelegate>

@end

enum {
  WindowDidResize
};

@implementation MLWindowDelegate
{
  value windowId;
}

- (instancetype)initWithId:(value)winId
{
  if (self = [super init]) {
    windowId = winId;
  }
  return self;
}

- (void)windowDidResize:(NSNotification *)__unused aNotification
{
  static value *closure_f = NULL;

  if (closure_f == NULL)
    closure_f = caml_named_value ("[NSWindow delegate]");

  caml_callback2 (*closure_f, windowId, Val_int (WindowDidResize));
}

@end

@interface View: NSView 
@end

@implementation View
- (BOOL)isFlipped
{
  return YES;
}
@end

#define Val_NSWindow(v) ((value)(v))
#define NSWindow_val(v) ((NSWindow *)(v))
#define Val_NSView(v) ((value)(v))
#define NSView_val(v) ((View *)(v))

CAMLprim value ml_NSWindow_windowWithContentRect (value winId, value rect_v)
{
  CAMLparam2 (winId, rect_v);
  CGFloat x = Double_val (Field (rect_v, 0));
  CGFloat y = Double_val (Field (rect_v, 1));
  CGFloat w = Double_val (Field (rect_v, 2));
  CGFloat h = Double_val (Field (rect_v, 3));
  NSRect contentRect = NSMakeRect (x, y, w, h);
  NSUInteger styleMask =
    NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskMiniaturizable | NSWindowStyleMaskResizable;

  NSWindow *win = [[NSWindow alloc] initWithContentRect:contentRect
                                              styleMask:styleMask
                                                backing:NSBackingStoreBuffered
                                                  defer:NO];
  [win setDelegate:[[MLWindowDelegate alloc] initWithId:winId]];

  CAMLreturn (Val_NSWindow (win));
}

CAMLprim value ml_NSWindowSetMinSize (value win_v, value rect_v)
{
  CAMLparam2 (win_v, rect_v);
  CGFloat width = Double_val (Field (rect_v, 0));
  CGFloat height = Double_val (Field (rect_v, 1));
  NSWindow *win = NSWindow_val (win_v);
  [win setMinSize:NSMakeSize(width, height)];
  CAMLreturn (Val_unit);
}


CAMLprim value ml_NSWindow_isVisible (value win_v)
{
  CAMLparam1 (win_v);
  NSWindow *win = NSWindow_val (win_v);
  BOOL b = [win isVisible];
  CAMLreturn (Val_bool (b));
}

CAMLprim value ml_NSWindow_center (value win_v)
{
  CAMLparam1 (win_v);
  NSWindow *win = NSWindow_val (win_v);
  [win center];
  CAMLreturn (Val_unit);
}

CAMLprim value ml_NSWindow_makeKeyAndOrderFront (value win_v)
{
  CAMLparam1 (win_v);
  NSWindow *win = NSWindow_val (win_v);
  [win makeKeyAndOrderFront:nil];
  CAMLreturn (Val_unit);
}

CAMLprim value ml_NSWindow_setTitle (value win_v, value str_v)
{
  CAMLparam2 (win_v, str_v);
  NSWindow *win = NSWindow_val (win_v);
  NSString *str = [NSString stringWithUTF8String:String_val (str_v)];
  [win setTitle:str];
  CAMLreturn (Val_unit);
}

CAMLprim value ml_NSWindow_title (value win_v)
{
  CAMLparam1 (win_v);
  CAMLlocal1 (str_v);
  NSWindow *win = NSWindow_val (win_v);
  NSString *str = [win title];
  const char *bytes = [str UTF8String];
  int len = strlen (bytes);
  str_v = caml_alloc_string (len);
  memcpy (String_val (str_v), bytes, len);
  CAMLreturn (str_v);
}

#define Val_NSApplication(v) ((value)(v))
#define NSApplication_val(v) ((NSApplication *)(v))

CAMLprim value ml_NSApplication_NSApp (value appId)
{
  CAMLparam1 (appId);
  NSApplication *app = [NSApplication sharedApplication];
  [app setActivationPolicy:NSApplicationActivationPolicyRegular];
  [app activateIgnoringOtherApps:YES];
  [app setDelegate:[[MLApplicationDelegate alloc] initWithId:appId]];
  CAMLreturn (Val_NSApplication (app));
}

CAMLprim value ml_NSApplication_run (value app_v)
{
  CAMLparam1 (app_v);
  @autoreleasepool {
    NSApplication *app = NSApplication_val (app_v);
    [app run];
  }
  CAMLreturn (Val_unit);
}

CAMLprim value ml_NSViewWithContentRect (value rect_v)
{
  CAMLparam1 (rect_v);
  CGFloat x = Double_val (Field (rect_v, 0));
  CGFloat y = Double_val (Field (rect_v, 1));
  CGFloat w = Double_val (Field (rect_v, 2));
  CGFloat h = Double_val (Field (rect_v, 3));

  NSRect contentRect = NSMakeRect (x, y, w, h);
  View *view = [[View alloc] initWithFrame: contentRect];


  CAMLreturn (Val_NSView (view));
}

CAMLprim value View_setBackgroundColor(value view_v, value red_v, value green_v, value blue_v, value alpha_v)
{
  CAMLparam5(view_v, red_v, blue_v, green_v, alpha_v);

  View *view = NSView_val(view_v);

  CGFloat red = Double_val(red_v) / 255;
  CGFloat blue = Double_val(blue_v) / 255;
  CGFloat green = Double_val(green_v) / 255;
  CGFloat alpha = Double_val(alpha_v);

  [view setWantsLayer:YES];
  [view.layer setBackgroundColor:[[NSColor colorWithRed:red green:green blue:blue alpha:alpha] CGColor]];

  CAMLreturn(Val_unit);
}

// CAMLprim value View_setBackgroundColor(value view, double red, double green, double blue, double alpha) {
//   CAMLparam1 (view);
//   View *nsView = NSView_val (view);

//   NSLog(@"Red: %f Green: %f Blue: %f Alpha: %f", red, green, blue, alpha);
//   [nsView setWantsLayer:true];
//   [nsView.layer setBackgroundColor:[NSColor colorWithRed:1.0 green:green blue:blue alpha:alpha].CGColor];
//   [nsView.layer setNeedsDisplay];
//   [nsView setNeedsDisplay:YES];

//   CAMLreturn (Val_unit);
// }

CAMLprim value ml_NSViewSetContentRect (value view, value rect_v)
{

  CAMLparam1 (view);
  CAMLlocal1 (rect);

  CGFloat x = Double_val (Field (rect_v, 0));
  CGFloat y = Double_val (Field (rect_v, 1));
  CGFloat w = Double_val (Field (rect_v, 2));
  CGFloat h = Double_val (Field (rect_v, 3));
  View *nsView = NSView_val (view);

  NSRect contentRect = NSMakeRect (x, y, w, h);
  [nsView setFrame:contentRect];

  CAMLreturn (Val_unit);
}

CAMLprim value ml_NSViewGetWidth (value view) {
  CAMLparam1 (view);
  View *nsView = NSView_val (view);
  CAMLreturn(caml_copy_double(nsView.frame.size.width));
}

CAMLprim value ml_NSViewGetHeight (value view) {
  CAMLparam1 (view);
  View *nsView = NSView_val (view);
  CAMLreturn(caml_copy_double(nsView.frame.size.height));
}

CAMLprim value ml_NSViewGetContentRect (value view)
{
  CAMLparam1 (view);
  CAMLlocal1 (rect);
  View *nsView = NSView_val (view);
  rect = caml_alloc_small(4, 0);

  double x = nsView.frame.origin.x;
  double y = nsView.frame.origin.y;
  double w = nsView.frame.size.width;
  double h = nsView.frame.size.height;


  Double_field(rect, 0) = x;
  Double_field(rect, 1) = y;
  Double_field(rect, 2) = w;
  Double_field(rect, 3) = h;

  CAMLreturn (rect);
}


CAMLprim value ml_NSViewAddSubview (value viewA, value viewB)
{
  CAMLparam2 (viewA, viewB);
  View *view1 = NSView_val (viewA);
  View *view2 = NSView_val (viewB);
  [view1 addSubview: view2];
  CAMLreturn (Val_unit);
}

CAMLprim value ml_NSViewRemoveFromSuperview (value view_v)
{
  CAMLparam1 (view_v);
  View *view = NSView_val (view_v);
  [view removeFromSuperview];
  CAMLreturn (Val_unit);
}


CAMLprim value ml_NSWindowGetContentView (value win_v)
{
  CAMLparam1 (win_v);
  NSWindow *win = NSWindow_val (win_v);
  CAMLreturn (Val_NSView (win.contentView));
}

CAMLprim value ml_NSWindowAddSubview (value win_v, value view_v)
{
  CAMLparam2 (win_v, view_v);
  NSWindow *win = NSWindow_val (win_v);
  View *view = NSView_val (view_v);
  [win.contentView addSubview: view];
  CAMLreturn (Val_unit);
}

typedef void (^ActionBlock)();

@interface Button : NSButton

@property(nonatomic, copy) ActionBlock _actionBlock;

- (void)onClick:(ActionBlock)action;
@end

@implementation Button

- (void)onClick:(ActionBlock)action
{
  self._actionBlock = action;
  [self setTarget:self];
  [self setAction:@selector(callActionBlock:)];
}

- (void)callActionBlock:(id)sender
{
#pragma unused(sender)
  self._actionBlock();
}
@end

#define Val_View(v) ((value)(v))
#define View_val(v) ((__bridge View *)(value)(v))

#define Val_Button(v) ((value)(v))
#define Button_val(v) ((__bridge Button *)(value)(v))

dispatch_queue_t ml_q;



#define Val_NSButton(v) ((value)(v))
#define NSButton_val(v) ((Button *)(v))

CAMLprim value ml_NSButtonWithContentRect (value rect_v)
{
  CAMLparam1 (rect_v);
  CGFloat x = Double_val (Field (rect_v, 0));
  CGFloat y = Double_val (Field (rect_v, 1));
  CGFloat w = Double_val (Field (rect_v, 2));
  CGFloat h = Double_val (Field (rect_v, 3));

  NSRect contentRect = NSMakeRect (x, y, w, h);
  Button *button = [[Button alloc] initWithFrame:contentRect];
  [button setTitle:@"Click me"];
  [button setButtonType:NSButtonTypeToggle]; //Set what type button You want
  [button setBezelStyle:NSBezelStyleRounded]; //Set what style You want

  CAMLreturn (Val_NSButton (button));
}

CAMLprim value ml_NSButtonSetTitle (value button_v, value str_v)
{
  CAMLparam2 (button_v, str_v);
  Button *button = NSButton_val (button_v);
  NSString *str = [NSString stringWithUTF8String:String_val (str_v)];
  [button setTitle:str];
  CAMLreturn (Val_unit);
}


CAMLprim value Button_setCallback(value btn_v, value callback_v)
{
  CAMLparam2(btn_v, callback_v);

  Button *btn = Button_val(btn_v);
  value callback = callback_v;

  caml_register_global_root(&callback);

  [btn onClick:^{
    caml_callback(callback, Val_unit);
  }];

  CAMLreturn(Val_Button(btn));
}
