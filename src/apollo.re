
let httpLink = ApolloLinks.createHttpLink(~uri=Config.graphqlURI, ());

let makeAuthLink = (auth) => {
  let contextHandler = () => { "headers": { "authorization": auth } };
  ApolloLinks.createContextLink(contextHandler)
};

type dataObject = {. "__typename": string, "id": string, "key": string };

let inMemoryCache = ApolloInMemoryCache.createInMemoryCache(
  ~dataIdFromObject=(obj: dataObject) => obj##id,
  ()
);

module Client = ReasonApollo.CreateClient({
  let links = switch (Config.graphqlAuthHeader) {
  | None => [|httpLink|]
  | Some(auth) => [|makeAuthLink(auth), httpLink|]
  };

  let apolloClient = ReasonApollo.createApolloClient(
    ~cache=inMemoryCache,
    ~link=ApolloLinks.from(links),
    ()
  );
});


[@bs.send] external resetStoreOfClient
  : (ApolloClient.generatedApolloClient) => Js.Promise.t(unit)
  = "resetStore";

let resetStore = () => resetStoreOfClient(Client.apolloClient);


type errorCode = 
  [ `BadRequest | `InternalServerError ];

type error = {. "code": errorCode, "message": string };

exception ResponseError(errorCode, string);


module type RequestConfig = {
  type payload;
  type response;
  let deconstructResponse: (response) => (option(payload), option(error))
};

module Request = (Conf: RequestConfig) => {
  external castResponse : string => {. "data": Js.Json.t } = "%identity";            

  [@bs.module] external gql : ReasonApolloTypes.gql = "graphql-tag";

  type gqlQuery = {.
    "query": string,
    "variables": Js.Json.t,
    "parse": (Js.Json.t) => Conf.response
  };

  type request = 
    [ `Mutation(gqlQuery)
    | `Query(gqlQuery) ];

  let gqlQueryOfRequest : (request) => gqlQuery = fun
    | `Mutation(q) | `Query(q) => q;

  exception SendFailure(Js.Promise.error, gqlQuery);

  type result = 
    [ `Payload(Conf.payload)
    | `Exn(exn) ];

  let send = (~request) => {
    let module P = Js.Promise;

    let requestPromise = switch request {
    | `Query(query) => 
      Client.apolloClient##query({
        "query": [@bs] gql(query##query),
        "variables": query##variables
      })
    | `Mutation(mutation) => 
      Client.apolloClient##mutate({
        "mutation": [@bs] gql(mutation##query),
        "variables": mutation##variables
      })
    };
    
    let gqlQuery = gqlQueryOfRequest(request);

    P.make((~resolve, ~reject as _) => {
      /* This promise is never rejected since, as of writing, there does not
         seem to be any good way with dealing with the `Js.Promise.error`
         abstract type. */
      P.resolve(requestPromise)

      |> P.then_((result) => {
        let typedResult = castResponse(result)##data |> gqlQuery##parse;
        let (payload, error) = Conf.deconstructResponse(typedResult);

        switch (payload, error) {
        | (_payload, Some(error)) =>
          [@bs] resolve(`Exn(ResponseError(error##code, error##message)))
        | (None, None) =>
          [@bs] resolve(`Exn(ResponseError(`InternalServerError,
                                           "Payload and error are null.")))
        | (Some(payload), _error) =>
          [@bs] resolve(`Payload(payload))
        };

        P.resolve()
      })

      |> P.catch((error) => {
        [@bs] resolve(`Exn(SendFailure(error, gqlQuery)));
        P.resolve()
      })

      |> ignore
    })
  };

};

let messageOfExn = (failedAction, ~id, ~exn) => {
  let prefix = "Failed " ++ failedAction;
  switch exn {
  | ResponseError(_code, message) => 
    prefix ++ ": " ++ message
  | exn => 
    Js.log3(prefix, id, exn);
    prefix ++ ". See console."
  }
};