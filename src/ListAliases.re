
let str = ReasonReact.stringToElement;

type status = 
  | Loading
  | Success
  | Failed(string);

type action = 
  | Error(string)
  | Loaded(array(Alias.t))
  | AliasChange;

type state = {
  status: status,
  list: array(Alias.t)
};

let component = ReasonReact.reducerComponent("QueryAliases");

let make = (~url, ~state as appState: State.t, _children) => {

  let loadList = (url, reduce) => {
    Js.log2("Loading aliases of", url);
    let open Js.Promise;
    QueryAliases.run(~url)
    |> then_((result:QueryAliases.Request.result) => {
      switch result {
      | `Exn(exn) => switch exn {
        | Apollo.ResponseError(_code, message) => 
          reduce((_self) => Error("Failed loading: " ++ message))()
        | exn => 
          Js.log3("Failed loading alias list for URL.", url, exn);
          reduce((_self) => Error("Failed loading alias list. See console."))()
        }
      | `Payload(payload) => 
        let lst = payload |> Js.Array.map((a) => Alias.ofGql(a)); 
        Store.dispatch(Action.ListIsFresh);
        reduce((_self) => Loaded(lst))()
      };
      resolve()
    })
    |> ignore;
    ()
  };

  let reloadList = (url, reduce) => {
    Apollo.resetStore()
    |> Js.Promise.then_(() => {
      loadList(url, reduce);
      Js.Promise.resolve()
    })
    |> Js.Promise.catch((error) => {
      Js.log3("Failed reloading list.", url, error);
      reduce((_self) => Error("Failed reloading list. See console."))();
      Js.Promise.resolve()
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
      | AliasChange =>
        ReasonReact.SideEffects(({ reduce }) => reloadList(url, reduce))
    },

    didMount: (_state) => {
      ReasonReact.SideEffects(({ reduce }) => loadList(url, reduce))
    },

    willReceiveProps: ({ state, reduce }) => {
      if (appState.listIsStale) {
        reloadList(url, reduce)
      };
      state
    },

    render: ({ state: { status, list }, reduce }) => {
      let body = switch status {
      | Loading => 
        <p className="status loading"> (str("Loading...")) </p>
      | Failed(message) =>
        <p className="status failure"> (str(message)) </p>
      | Success => switch list {
        | [||] =>
          <p className="status empty"> (str("No aliases.")) </p>
        | lst => 
          let (enabled, disabled) =
            lst |> Array.to_list
                |> List.partition((a) => Alias.status(a) == `Enabled);
          let elementOfAlias = (a) => {
            <AliasWidget key=(Alias.id(a) |> string_of_int)
                         alias=a
                         onChange=(reduce((_) => AliasChange)) />
          };
          let elementsOfAliases = (lst) =>
            lst |> Array.of_list |> Array.map(elementOfAlias)
                |> ReasonReact.arrayToElement;

          <div>
            <div className="enabled">
              <h2>(str("Enabled"))</h2>
              (enabled |> elementsOfAliases)
            </div>
            <div className="disabled">
              <h2>(str("Disabled"))</h2>
              (disabled |> elementsOfAliases)
            </div>
          </div>
        }
      };
      
      <section className="list">
        (body)
      </section>
    }
  }
};