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

type error = {."code": string, "message": string };
type alias = {. "id": string, "name": string, "status": string };
type payload = {.
  "error": Js.Nullable.t(error),
  "aliases": Js.Nullable.t(array(alias))
};
type result = {. "aliases": payload };

module Config = {
  type responseType = result;
  type variables = {. "url": Url.gqlT };
};

module FetchAliases = Apollo.Client.Query(Config);

let component = ReasonReact.statelessComponent("QueryAliases");

let make = (~url, _children) => {
  {
    ...component,
    render: (_self) => {
      let variables = {
        "url": Url.toGql(url)
      };
      <FetchAliases query variables>
        (fun
        | Loading => <div> (str("Loading aliases.")) </div>
        | Failed(error) => <div> (str("Failed loading aliases: " ++ error)) </div>
        | Loaded(result) =>  {
          let payload = result##aliases;
          if (JsOpt.notNull(payload##error)) {
            let error = JsOpt.value(payload##error);
            let className = "error-" ++ error##code;
            <div className>
              (str("Failed loading aliases: " ++ error##message))
            </div>
          }
          else if (JsOpt.isNull(payload##aliases)) {
            <div>
              (str("An unknown error occurred."))
            </div>
          }
          else {
            ReasonReact.arrayToElement(
              payload##aliases |> JsOpt.value |>  Array.map((alias) => 
                <div key=(alias##id)>
                  (str(alias##name ++ " : " ++ alias##status))
                </div>
              )
            )
          }
        }
        )
      </FetchAliases>
    }
  }
};