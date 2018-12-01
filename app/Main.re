print_float(nan);
print_newline();
let native = Dynlink.is_native ? "native" : "bytecode";
print_endline(native);
PureMacOS.render(<App />);