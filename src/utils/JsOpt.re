
let isNull = Js.Nullable.test;

let notNull = (x) => x |> isNull |> not;

let value = (x) =>
  /* There's gotta be a more elegant solution... */
  x |> Js.Nullable.to_opt |> Opt.value;
