
[@bs.val] external encodeURIComponent : string => string = "encodeURIComponent";
[@bs.val] external decodeURIComponent : string => string = "decodeURIComponent";

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
    switch (s |> Js.String.replaceByRe([%bs.re "/^\\?/"], "")) {
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
    params: Opt.map(Params.ofString, query),
    fragment
  };

exception MissingPart(string, string);

let ofString = (s) => {
  let a : Js.t({..}) = [%bs.raw "document.createElement('a')"];
  ignore @@ [%bs.raw {| a.href = s |}];

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
    ~port =? propOpt("port") |> Opt.map(int_of_string),
    ~path = prop("pathname"),
    ~query =? propOpt("search"),
    ~fragment =? propOpt("hash"),
    ()
  )
};

let ofStringOpt = (s) => 
  try (Some(ofString(s))) { | _ => None };

type gqlParamPair = {.
  "key": string, "value": Js.Nullable.t(string)
};

type gqlT = {.
  "scheme": string,
  "user": Js.Nullable.t(string),
  "password": Js.Nullable.t(string),
  "host": string,
  "port": Js.Nullable.t(int),
  "path": string,
  "params": Js.Nullable.t(array(gqlParamPair)),
  "fragment": Js.Nullable.t(string)
};

let toGql = (t) : gqlT => {
  let nullOfOpt = Js.Nullable.from_opt;
  let paramsToGql = (p:Params.t) =>
    p |> Params.toList
      |> List.map((p) => Params.({
        "key": p.key,
        "value": Js.Nullable.from_opt(p.value)
      }))
      |> Array.of_list;
  {
    "scheme": t.scheme |> Scheme.toString |> String.uppercase,
    "user": t.user |> nullOfOpt,
    "password": t.pass |> nullOfOpt,
    "host": t.host,
    "port": t.port |> nullOfOpt,
    "path": Path.toString(t.path),
    "params": t.params |> Opt.map(paramsToGql) |> nullOfOpt,
    "fragment": t.fragment |> nullOfOpt
  }
};