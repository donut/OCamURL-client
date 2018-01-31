
let str = ReasonReact.stringToElement;

type state = {
  firstLoad: bool
};

let component = ReasonReact.reducerComponent("QueryAliases");


let make = (~url, ~aliasList: State.aliasList, _children) => {

  let renderList = (lst, status) => {
    let (enabled, disabled) =
      lst |> List.partition((a) => Alias.status(a) == `Enabled);
    let elementOfAlias = (a) => {
      <AliasWidget key=(Alias.id(a) |> string_of_int)
                    alias=a
                    onChange=((_) => QueryAliases.reload(~url)) />
    };
    let elementsOfAliases = (lst) =>
      lst |> Array.of_list |> Array.map(elementOfAlias)
          |> ReasonReact.arrayToElement;

    <div>
      <div className="enabled">
        <h2>(str("Enabled"))</h2>
        (enabled |> elementsOfAliases)
      </div>
      (switch (disabled) {
      | [] => ReasonReact.nullElement
      |  _ => 
        <div className="disabled">
          <h2>(str("Disabled"))</h2>
          (disabled |> elementsOfAliases)
        </div>
      })
    </div>
  };

  {

    ...component,

    initialState: () => { firstLoad: true },
    reducer: ((), _state: state) => ReasonReact.NoUpdate,

    didMount: (_self) => {
      QueryAliases.run(~url);
      ReasonReact.NoUpdate
    },

    willReceiveProps: ({ state }) => {
      switch (aliasList, state.firstLoad) {
      | (`Unloaded, _) =>
        QueryAliases.run(~url)
      | (`Loaded([], `Fresh), true) =>
        MutationGenerateAlias.run(~id="alias-list", ~url)
      | (`Loaded(_lst, `Stale), _) =>
        QueryAliases.reload(~url)
      | (_, _) => ()
      };

      switch (aliasList) {
      | `Unloaded     => { firstLoad: true  }
      | `Loaded(_, _) => { firstLoad: false }
      |             _ => state
      }
    },

    render: (_self) => {
      let (status, body) = switch (aliasList) {
      | `Unloaded | `Loading => 
        ( "loading", 
          <p className="status loading"> (str("Loading...")) </p> )
      | `Failed(message) =>
        ( "failed-loading",
          <p className="status failure"> (str(message)) </p> )
      | `Loaded([], _status) =>
        ( "loaded-empty",
          <p className="status empty"> (str("No aliases.")) </p> )
      | `Loaded(lst, status) =>
        let status' = switch (status) {
        | `Fresh => "fresh"
        | `Stale => "stale"
        | `Reloading => "reloading"
        };
        ( "loaded-" ++ status', 
          renderList(lst, status) )
      };
      
      <section className=("alias-list status-" ++ status)>
        <h1> (str("Aliases")) </h1>
        (body)
      </section>
    }
  }
};