[@bs.module] external gql : ReasonApolloTypes.gql = "graphql-tag";

let str = ReasonReact.stringToElement;

let query = [@bs] gql({|
  query GetAliasesByURL($url: AliasesURLInput) {
    aliases(url: $url) {
      error { code, message }
      aliases { id, name, status }
    }
  }
|});

type payload = {.
  "error": Js.Nullable.t(Apollo.error),
  "aliases": Js.Nullable.t(array(Alias.gqlT))
};
type result = {. "aliases": payload };

module Config = {
  type response = result;
  type variables = {. "url": Url.gqlT };
  let request = `Query(query);
};

module Request = Apollo.Request(Config);

let run = (~url) => {
  let variables = Some({
    "url": Url.toGql(url)
  });

  Request.send(~variables);
};
