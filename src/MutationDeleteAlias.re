[@bs.module] external gql : ReasonApolloTypes.gql = "graphql-tag";

let mutation = [@bs] gql({|
  mutation DeleteAlias($input: DeleteAliasInput!) {
    deleteAlias(input: $input) {
      error { code, message }
      payload { acitonTaken }
    }
  }
|});

module Config = {
  type input = {.
    "clientMutationId": string,
    "name": string,
    "disableIfUsed": Js.boolean
  };

  type payload = {.
    actionTaken: string
  };

  type payloadOrError = {.
    "error": Js.Nullable.t(Apollo.error),
    "payload": Js.Nullable.t(payload)
  };

  type response = {. "deleteAlias": payloadOrError };
  type variables = {. "input": input };
  let request = `Mutation(mutation);
  let deconstructResponse = (response) =>
    (response##deleteAlias##payload, response##deleteAlias##error);
};

module Request = Apollo.Request(Config);

let run = (~name) => {
  let now = Js.Date.now() |> string_of_float;
  let input = {
    "clientMutationId": "deleteAlias///" ++ name ++ "///" ++ now,
    "name": name,
    "disableIfUsed": Js.true_
  };
  let variables = Some({"input": input});

  Request.send(~variables)
};

