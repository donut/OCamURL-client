
module HTTPLink = ApolloLinks.CreateHttpLink({
  let uri = "http://mgmt.ocamurl.dev/graphql";
});

/* This should be a good start for HTTP Basic auth */
/* module AuthLink = CreateContextLink({
  let contextHandler = () => {
    let headers = {
      "headers": {
        "authorization": {j|Auth magic here|j}
      }
    };
    headers
  };
}); */

module InMemoryCache = ApolloInMemoryCache.CreateInMemoryCache({
  type dataObject;
  let inMemoryCacheObject = Js.Nullable.undefined;
});

module Client = ReasonApollo.CreateClient({
  let apolloClient = ReasonApollo.createApolloClient(
    ~cache=InMemoryCache.cache,
    ~link=ApolloLinks.from([|HTTPLink.link|]),
    ()
);

});

[@bs.send] external resetStoreOfClient
  : (ApolloClient.generatedApolloClient) => Js.Promise.t(unit)
  = "resetStore";

let resetStore = () => resetStoreOfClient(Client.apolloClient);

exception ResponseError(string, string);

type error = {. "code": string, "message": string };

type request = 
  [ `Mutation(ReasonApolloTypes.queryString)
  | `Query(ReasonApolloTypes.queryString)
  ];

let queryStringOfRequest = fun
  | `Mutation(q) | `Query(q) => q;

module type RequestConfig = {
  type payload;
  type response;
  type variables;
  let request: request;
  let deconstructResponse:
    (response) => (Js.nullable(payload), Js.nullable(error))
};

module Request = (RequestConfig: RequestConfig) => {

  module CastApolloClient = ApolloClient.Cast({
    type variables = RequestConfig.variables
  });
  let apolloClient = CastApolloClient.castClient(Client.apolloClient);

  external cast : string => {. 
    "data": RequestConfig.response, "loading": bool
  } = "%identity";

  exception SendFailure(Js.Promise.error,
                        ReasonApolloTypes.queryString,
                        option(RequestConfig.variables));

  type result = 
    [ `Payload(RequestConfig.payload)
    | `Exn(exn) ];

  let send = (~variables) => {
    let module P = Js.Promise;

    P.make((~resolve, ~reject) => {
      let requestPromise = switch RequestConfig.request {
      | `Query(query) => {
          let conf = CastApolloClient.getJSQueryConfig(
            ~query, ~variables=?variables, ());
          apolloClient##query(conf) 
        }
      | `Mutation(mutation) => {
          let conf = CastApolloClient.getJSMutationConfig(
            ~mutation, ~variables=?variables, ());
          apolloClient##mutate(conf)
        }
      };

      /* This promise is never rejected since as of writing there does not seem
         to be any good way with dealing with the `Js.Promise.error` abstract
         type. */
      P.resolve(requestPromise)
      |> P.then_((result) => {
        let typedResult = cast(result)##data;
        let (payload, error) = RequestConfig.deconstructResponse(typedResult);
        if (JsOpt.notNull(error)) {
          let error' = JsOpt.value(error);
          [@bs] resolve(`Exn(ResponseError(error'##code, error'##message)))
        }
        else if (JsOpt.isNull(payload)) {
          [@bs] resolve(`Exn(ResponseError("InternalServerError",
                                           "Payload and error are null.")))
        }
        else {
          [@bs] resolve(`Payload(JsOpt.value(payload)))
        };
        P.resolve()
      })
      |> P.catch((error) => {
        let queryString = queryStringOfRequest(RequestConfig.request);
        [@bs] resolve(`Exn(SendFailure(error, queryString, variables)));
        P.resolve()
      })
      |> ignore;
      ()
    })
  };

};