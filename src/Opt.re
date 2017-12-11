
let map = (f) => fun
  | None => None
  | Some(x) => Some(f(x));

let value = fun 
  | None => raise(Not_found)
  | Some(x) => x;

let hasSome = fun
  | None => false
  | Some(_) => true;

let hasNone = (o) => o |> hasSome |> not
