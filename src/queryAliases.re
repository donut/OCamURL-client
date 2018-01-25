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
  Store.dispatch(Action.AliasListLoading);

  let variables = Some({
    "url": Url.toGql(url)
  });

  Request.send(~variables)

  |> Js.Promise.then_((result:Request.result) => {
    switch (result) {

    | `Exn(exn) =>
      let message = Apollo.messageOfExn("getting alias list", ~id=url, ~exn);
      Store.dispatch(Action.AliasListLoadingFailed(message))

    | `Payload(p) =>
      let lst = p |> Array.to_list |> List.map((a) => Alias.ofGql(a));
      Store.dispatch(Action.AliasListLoaded(lst))

    };
    Js.Promise.resolve()
  })

  |> ignore
};

let reload = (~url) => {
  Store.dispatch(Action.AliasListLoading);

  Apollo.resetStore()
  
  |> Js.Promise.then_(() => {
    run(~url);
    Js.Promise.resolve()
  })

  |> Js.Promise.catch((error) => {
    let message = "Failed reseting store before reloading alias list.";
    Js.log3(message, url, error);
    Store.dispatch(Action.AliasListLoadingFailed(message));
    Js.Promise.resolve()
  })
  
  |> ignore
};
