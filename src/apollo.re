
module Client = ReasonApollo.Create({
  let uri = "http://mgmt.ocamurl.dev/graphql";
});

type request = 
  [ `Mutation(ReasonApolloTypes.queryString)
  | `Query(ReasonApolloTypes.queryString)
  ];

let queryStringOfRequest = fun
  | `Mutation(q) | `Query(q) => q;

module type RequestConfig = {
  type response;
  type variables;
  let request: request;
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

      P.resolve(requestPromise)
      |> P.then_((result) => {
        let typedResult = cast(result)##data;
        [@bs] resolve(typedResult);
        P.resolve()
      })
      |> P.catch((error) => {
        let queryString = queryStringOfRequest(RequestConfig.request);
        [@bs] reject(SendFailure(error, queryString, variables));
        P.resolve()
      })
      |> ignore;
      ()
    })
  };

};

type error = {. "code": string, "message": string };