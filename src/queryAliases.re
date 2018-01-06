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

type payload' = array(Alias.gqlT);

type payloadOrError = {.
  "error": Js.Nullable.t(Apollo.error),
  "aliases": Js.Nullable.t(payload')
};

type result = {. "aliases": payloadOrError };

module Config = {
  type payload = payload';
  type response = result;
  type variables = {. "url": Url.gqlT };
  let request = `Query(query);
  let deconstructResponse = (response) => 
    (response##aliases##aliases, response##aliases##error);
};

module Request = Apollo.Request(Config);

let run = (~url) => {
  let variables = Some({
    "url": Url.toGql(url)
  });

  Request.send(~variables);
};
