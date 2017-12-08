
[@bs.val] external encodeURIComponent : string => string = "encodeURIComponent";
[@bs.val] external decodeURIComponent : string => string = "decodeURIComponent";

let optMap = (f) => fun
  | None => None
  | Some(x) => Some(f(x));

let optValue = fun 
  | None => raise(Not_found)
  | Some(x) => x;

module Scheme = {
  type t = [ `HTTP | `HTTPS ];
  exception UnsupportedScheme(string);
  let ofString = (s) : t => switch (String.lowercase(s)) {
    | "http"  | "http:"  => `HTTP
    | "https" | "https:" => `HTTPS
    | _ => raise(UnsupportedScheme(s))
  };
  let ofStringOpt = (s) : option(t) =>
    try (Some(ofString(s))) { | _ => None };
  let toString : t => string = fun
    | `HTTP => "http" | `HTTPS => "https";
};

module Path = {
  type t = string;
  let ofString = (s) => s;
  let toString = (t) => 
    String.length(t) > 0 && String.get(t, 0) != '/'
    ? "/" ++ t
    : t;
};

module Params = {
  type pair = { key: string, value: option(string) };
  type t = list(pair);

  let ofList = (l) => l;
  let toList = (t) => t;

  let ofString = (s:string) : t => {
    let dec = decodeURIComponent;
    switch (s |> Js.String.replaceByRe([%bs.re "/^\?/"], "")) {
    | "" => []
    |  s => s
      |> Js.String.split("&") |> Array.to_list
      |> List.map(Js.String.split("=")) |> List.map(Array.to_list)
      |> List.map(fun
        | [] => { key: "", value: None }
        | [key] => { key: dec(key), value: None }
        | [key, value] => { key: dec(key), value: Some(dec(value)) }
        | [key, ...value] => {
          key: dec(key),
          value: Some(String.concat("=", value) |> dec)
        }
      )
    }
  };

  let toString = (t) => {
    let pairUp = ({key, value}) => {
      let enc = encodeURIComponent;
      let value' = switch value {
        | None => ""
        | Some(v) => "=" ++ enc(v) };
      enc(key) ++ value'
    };
    t |> List.map(pairUp) |> String.concat("&")
  };
};

type t = {
  id: option(int),
  scheme: Scheme.t,
  user: option(string),
  pass: option(string),
  host: string,
  path: Path.t,
  port: option(int),
  params: option(Params.t),
  fragment: option(string),
};

let make = (~id=?, ~scheme, ~user=?, ~pass=?, ~host, ~port=?, ~path="",
            ~query=?, ~fragment=?, ()) =>
  {
    id,
    scheme,
    user,
    pass,
    host,
    port,
    path: Path.ofString(path),
    params: optMap(Params.ofString, query),
    fragment
  };

exception MissingPart(string, string);

let ofString = (s) => {
  let a : Js.t({..}) = [%bs.raw "document.createElement('a')"];
  [%bs.raw {| a.href = s |}];

  let prop = (name) => {
    let get : (string, Js.t({..})) => string = [%bs.raw {|
      function (key, obj) { return obj[key] }
      |}];
    get(name, a)
  };
  let propOpt = (name) => switch (prop(name)) {
    | "" => None
    |  s => Some(s)
  };
  let propExn = (name) => switch (propOpt(name)) {
    | None => raise(MissingPart(name, s))
    | Some(s) => s
  };

  make(
    ~scheme = prop("protocol") |> Scheme.ofString,
    ~user =? propOpt("username"),
    ~pass =? propOpt("password"),
    ~host = propExn("hostname"),
    ~port =? propOpt("port") |> optMap(int_of_string),
    ~path = prop("pathname"),
    ~query =? propOpt("search"),
    ~fragment =? propOpt("hash"),
    ()
  )
};

let ofStringOpt = (s) => 
  try (Some(ofString(s))) { | _ => None }