[@bs.module] external gql : ReasonApolloTypes.gql = "graphql-tag";

module DisableMutation = [%graphql {|
  mutation DisableAlias($input: DisableAliasInput!) {
    disableAlias(input: $input) {
      error { code, message }
      payload { clientMutationId }
    }
  }
|}];

module EnableMutation = [%graphql {|
  mutation EnableAlias($input: EnableAliasInput!) {
    enableAlias(input: $input) {
      error { code, message }
      payload { clientMutationId }
    }
  }
|}];

module Config = {
  type input = {.
    "clientMutationId": string,
    "name": string
  };

  type payload = {.
    "clientMutationId": string
  };

  type payloadOrError = {.
    "error": option(Apollo.error),
    "payload": option(payload)
  };
};

module DisableConfig = {
  include Config;

  type response = {. "disableAlias": payloadOrError };
  let deconstructResponse = (response) =>
    (response##disableAlias##payload, response##disableAlias##error);
};

module EnableConfig = {
  include Config;

  type response = {. "enableAlias": payloadOrError };
  let deconstructResponse = (response) =>
    (response##enableAlias##payload, response##enableAlias##error);
};

module DisableRequest = Apollo.Request(DisableConfig);
module EnableRequest  = Apollo.Request(EnableConfig);

let run = (~name, action) => {
  let now = Js.Date.now() |> string_of_float;
  let actionStr = switch action { 
    | `Disable => "disable"
    | `Enable => "enable"
  };
  let input = {
    "clientMutationId": name ++ "///" ++ actionStr ++ "///" ++ now,
    "name": name
  };

  switch action {
    | `Disable =>
      let request = `Mutation(DisableMutation.make(~input, ()));
      DisableRequest.send(~request)
    | `Enable  =>
      let request = `Mutation(EnableMutation.make(~input, ()));
      EnableRequest.send(~request)
  }
};