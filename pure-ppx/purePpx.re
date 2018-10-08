open Ast_helper;

open Asttypes;

open Parsetree;

open Longident;

let getenv = s =>
  try (Sys.getenv(s)) {
  | Not_found => ""
  };

let getenv_mapper = argv =>
  /* Our getenv_mapper only overrides the handling of expressions in the default mapper. */
  {
    ...Ast_mapper.default_mapper,
    expr: (mapper, expr) =>
      switch (expr) {
      /* Is this an extension node? */
      | {
          pexp_desc:
            /* Should have name "getenv". */
            [@implicit_arity] Pexp_extension({txt: "getenv", loc}, pstr),
        } =>
        switch (pstr) {
        | /* Should have a single structure item, which is evaluation of a constant string. */ PStr([
            {
              pstr_desc:
                [@implicit_arity]
                Pstr_eval(
                  {
                    pexp_loc: loc,
                    pexp_desc:
                      Pexp_constant(
                        [@implicit_arity] Const_string(sym, None),
                      ),
                  },
                  _,
                ),
            },
          ]) =>
          /* Replace with a constant string with the value from the environment. */
          Exp.constant(
            ~loc,
            [@implicit_arity] Const_string(getenv(sym), None),
          )
        | _ =>
          raise(
            Location.Error(
              Location.error(
                ~loc,
                "[%getenv] accepts a string, e.g. [%getenv \"USER\"]",
              ),
            ),
          )
        }
      /* Delegate to the default mapper. */
      | x => Ast_mapper.default_mapper.expr(mapper, x)
      },
  };

let () = Ast_mapper.register("getenv", getenv_mapper);