[@bs.module] external gql : ReasonApolloTypes.gql = "graphql-tag";

module Mutation = [%graphql {|
  mutation RenameAlias($input: RenameAliasInput!) {
    renameAlias(input: $input) {
      error { code, message }
      payload { actionTaken }
    }
  }
|}];

module Config = {
  type input = {.
    "clientMutationId": string,
    "name": string,
    "newName": string,
    "disableAndAddIfUsed": option(Js.boolean)
  };

  type actionTaken = 
    [ `DisableAndAdd | `Rename ];

  type payload = {.
    "actionTaken": actionTaken
  };

  type payloadOrError = {.
    "error": option(Apollo.error),
    "payload": option(payload)
  };

  type response = {. "renameAlias": payloadOrError };
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
    "disableAndAddIfUsed": Some(Js.true_)
  };
  Request.send(~request=`Mutation(Mutation.make(~input, ())))
};