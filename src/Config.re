
type t = {.
  "graphqlURI": string,
  "graphqlAuthHeader": Js.Nullable.t(string),
  "aliasPrefix": string
};

[@bs.val] external conf : t = "window.config";

let graphqlURI = conf##graphqlURI;
let graphqlAuthHeader = Js.Nullable.to_opt(conf##graphqlAuthHeader);
let aliasPrefix = conf##aliasPrefix;
