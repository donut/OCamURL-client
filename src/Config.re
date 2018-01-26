
type t = {.
  "graphqlURI": string,
  "aliasPrefix": string
};

[@bs.val] external conf : t = "window.config";

let graphqlURI = conf##graphqlURI;
let aliasPrefix = conf##aliasPrefix;
