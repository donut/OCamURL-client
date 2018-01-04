[@bs.module] external gql : ReasonApolloTypes.gql = "graphql-tag";

let query = [@bs] gql({|
  mutation RenameAlias($input: RenameAliasInput!) {
    renameAlias(input: $input) {
      error { code, message }
      payload { actionTaken }
    }
  }
|});

type input = {.
  "clientMutationId": string,
  "name": string,
  "newName": string,
  "disableAndAddIfUsed": Js.boolean
};

type payload = {.
  "actionTaken": string,
  "clientMutationId": string
};

type payloadOrError = {.
  "error": Js.Nullable.t(Apollo.error),
  "payload": Js.Nullable.t(payload)
};

module Config = {
  type responseType = {. "renameAlias": payloadOrError };
  type variables = {. "input": input };
};

module Mutation = Apollo.Query(Config);

let run = (~name, ~newName) => {
  let now = Js.Date.now() |> string_of_float;
  let input = {
    "clientMutationId": name ++ "///" ++ newName ++ "///" ++ now,
    "name": name,
    "newName": newName,
    "disableAndAddIfUsed": Js.true_
  };
  let variables = Some({"input": input});

  Mutation.send(`Mutation, ~query, ~variables)
};