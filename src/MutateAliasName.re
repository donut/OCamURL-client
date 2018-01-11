[@bs.module] external gql : ReasonApolloTypes.gql = "graphql-tag";

let mutation = [@bs] gql({|
  mutation RenameAlias($input: RenameAliasInput!) {
    renameAlias(input: $input) {
      error { code, message }
      payload { actionTaken }
    }
  }
|});


module Config = {
  type input = {.
    "clientMutationId": string,
    "name": string,
    "newName": string,
    "disableAndAddIfUsed": Js.boolean
  };

  type payload = {.
    "actionTaken": string
  };

  type payloadOrError = {.
    "error": Js.Nullable.t(Apollo.error),
    "payload": Js.Nullable.t(payload)
  };

  type response = {. "renameAlias": payloadOrError };
  type variables = {. "input": input };
  let request = `Mutation(mutation);
  let deconstructResponse = (response) => 
    (response##renameAlias##payload, response##renameAlias##error);
};

module Request = Apollo.Request(Config);

let run = (~name, ~newName) => {
  let now = Js.Date.now() |> string_of_float;
  let input = {
    "clientMutationId": name ++ "///" ++ newName ++ "///" ++ now,
    "name": name,
    "newName": newName,
    "disableAndAddIfUsed": Js.true_
  };
  let variables = Some({"input": input});

  Request.send(~variables)
};