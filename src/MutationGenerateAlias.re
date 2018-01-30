
[@bs.module] external gql : ReasonApolloTypes.gql = "graphql-tag";

module Mutation = [%graphql {|
  mutation GenerateAlias($input: GenerateAliasInput!) {
    generateAlias(input: $input) {
      error { code, message }
      payload {
        alias { id, name }
      }
    }
  }
|}];

module Config = {
  type input = {.
    "clientMutationId": string,
    "url": Url.gqlT
  };

  type alias = {.
    "id": string,
    "name": string
  };

  type payload = {.
    "alias": alias
  };

  type payloadOrError = {.
    "error": option(Apollo.error),
    "payload": option(payload)
  };

  type response = {. "generateAlias": payloadOrError };
  let deconstructResponse = (response) => 
    (response##generateAlias##payload, response##generateAlias##error);
};

module Request = Apollo.Request(Config);

let run = (~id, ~url) => {
  Store.dispatch(Action.GeneratingAlias(id));

  let now = Js.Date.now() |> string_of_float;
  let input = {
    "clientMutationId": "generateAlias///" ++ now,
    "url": Url.toGql(url)
  };
  let request = `Mutation(Mutation.make(~input, ()));

  Request.send(~request)

  |> Js.Promise.then_((result:Request.result) => {
    switch (result) {
    | `Exn(exn) =>
      let message = Apollo.messageOfExn("generating alias", ~id=url, ~exn);
      Store.dispatch(Action.GeneratingAliasFailed(id, message))
    | `Payload(p) => 
      Store.dispatch(Action.GeneratedAlias(id, p##alias##name))
    };
    Js.Promise.resolve()
  })
  |> ignore;

  ()
};

