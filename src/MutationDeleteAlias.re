[@bs.module] external gql : ReasonApolloTypes.gql = "graphql-tag";

module Mutation = [%graphql {|
  mutation DeleteAlias($input: DeleteAliasInput!) {
    deleteAlias(input: $input) {
      error { code, message }
      payload { actionTaken }
    }
  }
|}];

module Config = {
  type input = {.
    "clientMutationId": string,
    "name": string,
    "disableIfUsed": option(Js.boolean)
  };

  type payload = {.
    "actionTaken": [ `Delete | `Disable ]
  };

  type payloadOrError = {.
    "error": option(Apollo.error),
    "payload": option(payload)
  };

  type response = {. "deleteAlias": payloadOrError };
  let deconstructResponse = (response) =>
    (response##deleteAlias##payload, response##deleteAlias##error);
};

module Request = Apollo.Request(Config);

let run = (~name) => {
  let now = Js.Date.now() |> string_of_float;
  let input = {
    "clientMutationId": "deleteAlias///" ++ name ++ "///" ++ now,
    "name": name,
    "disableIfUsed": Some(Js.true_)
  };
  Request.send(~request=`Mutation(Mutation.make(~input, ())))
};

