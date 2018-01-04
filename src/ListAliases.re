
let str = ReasonReact.stringToElement;

type status = 
  | Loading
  | Success
  | Failed(string);

type action = 
  | Error(string)
  | Loaded(array(Alias.t));

type state = {
  status: status,
  list: array(Alias.t)
};

let component = ReasonReact.reducerComponent("QueryAliases");

let make = (~url, _children) => {

  let loadList = (url, reduce) => {
    let open Js.Promise;
    QueryAliases.run(~url)
    |> then_((response:QueryAliases.Config.responseType) => {
      let maybePayload = response##aliases;
      if (JsOpt.notNull(maybePayload##error)) {
        let error = JsOpt.value(maybePayload##error);
        reduce((_self) => Error(
          "Failed loading: " ++ " [" ++ error##code ++ "] " ++ error##message 
        ))()
      }
      else if (JsOpt.isNull(maybePayload##aliases)) {
        reduce((_self) => Error("Failed loading alias list. Dunno why."))()
      }
      else {
        let lst = maybePayload##aliases
          |> JsOpt.value
          |> Js.Array.map((a) => Alias.ofGql(a)); 
        reduce((_self) => Loaded(lst))()
      };
      resolve()
    })
    |> catch((error) => {
      Js.log3("Failed loading alias list for URL.", url, error);
      reduce((_self) => Error("Failed loading alias list. See console."))();
      resolve()
    })
    |> ignore;
    ()
  };

  {
    ...component,

    initialState: () => {
      {
        status: Loading,
        list: [||]
      }
    },

    reducer: (action, _state) => switch action {
      | Error(message) =>
        ReasonReact.Update({ status: Failed(message), list: [||] })
      | Loaded(lst) =>
        ReasonReact.Update({ status: Success, list: lst })
    },

    didMount: (_state) => {
      ReasonReact.SideEffects(({ reduce }) => loadList(url, reduce))
    },

    render: ({ state: { status, list }}) => {
      let body = switch status {
      | Loading => 
        <p className="status loading"> (str("Loading...")) </p>
      | Failed(message) =>
        <p className="status failure"> (str(message)) </p>
      | Success => switch list {
        | [||] =>
          <p className="status empty"> (str("No aliases.")) </p>
        | lst => 
          ReasonReact.arrayToElement(lst |> Array.map((a) => {
            <AliasWidget key=(Alias.id(a) |> string_of_int) alias=a />
          }))
        }
      };
      
      <section className="list">
        (body)
      </section>
    }
  }
};