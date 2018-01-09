[@bs.module] external gql : ReasonApolloTypes.gql = "graphql-tag";

let disableMutation = [@bs] gql({|
  mutation DisableAlias($input: DisableAliasInput!) {
    disableAlias(input: $input) {
      error { code, message }
      payload { clientMutationId }
    }
  }
|});

let enableMutation = [@bs] gql({|
  mutation EnableAlias($input: EnableAliasInput!) {
    enableAlias(input: $input) {
      error { code, message }
      payload { clientMutationId }
    }
  }
|});

module Config = {
  type input = {.
    "clientMutationId": string,
    "name": string
  };

  type payload = {.
    "clientMutationId": string
  };

  type payloadOrError = {.
    "error": Js.Nullable.t(Apollo.error),
    "payload": Js.Nullable.t(payload)
  };

  type variables = {. "input": input };
};

module DisableConfig = {
  include Config;

  type response = {. "disableAlias": payloadOrError };
  let request = `Mutation(disableMutation);
  let deconstructResponse = (response) =>
    (response##disableAlias##payload, response##disableAlias##error);
};

module EnableConfig = {
  include Config;

  type response = {. "enableAlias": payloadOrError };
  let request = `Mutation(enableMutation);
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
  let variables = Some({"input": input});
  
  switch action {
    | `Disable => DisableRequest.send(~variables)
    | `Enable  => EnableRequest.send(~variables)
  }
};