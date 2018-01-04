
module Client = ReasonApollo.Create({
  let uri = "http://mgmt.ocamurl.dev/graphql";
});

module Query = (ClientConfig: ReasonApollo.ClientConfig) => {

  module CastApolloClient = ApolloClient.Cast({
    type variables = ClientConfig.variables
  });
  let apolloClient = CastApolloClient.castClient(Client.apolloClient);

  external cast : string => {. 
    "data": ClientConfig.responseType, "loading": bool
  } = "%identity";

  type state = 
    | Loading
    | Loaded(ClientConfig.responseType)
    | Failed(string);


  exception SendFailure(Js.Promise.error,
                        ReasonApolloTypes.queryString,
                        option(ClientConfig.variables));

  let send = (typ, ~query, ~variables) => {
    let module P = Js.Promise;

    P.make((~resolve, ~reject) => {
      let requestPromise = switch typ {
      | `Query => {
          let conf = CastApolloClient.getJSQueryConfig(
            ~query, ~variables=?variables, ());
          apolloClient##query(conf) 
        }
      | `Mutation => {
          let conf = CastApolloClient.getJSMutateConfig(
            ~mutation=query, ~variables=?variables, ());
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
        [@bs] reject(SendFailure(error, query, variables));
        P.resolve()
      })

      |> ignore;
      ()
    })
  };

};

type error = {. "code": string, "message": string };