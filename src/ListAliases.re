
let str = ReasonReact.stringToElement;

type state = {
  firstLoad: bool
};

let component = ReasonReact.reducerComponent("QueryAliases");

let make = (~url, ~aliasList: State.aliasList, _children) => {
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
      | (`Loaded([], `Fresh), true) =>
        MutationGenerateAlias.run(~id="alias-list", ~url)
      | (`Loaded(_lst, `Stale), _) =>
        QueryAliases.reload(~url)
      | (_, _) => ()
      };

      switch (aliasList) {
      | `Loaded(_, _) => { firstLoad: false }
      | _ => state
      }
    },

    render: (_self) => {
      let body = switch (aliasList) {
      | `Loading => 
        <p className="status loading"> (str("Loading...")) </p>
      | `Failed(message) =>
        <p className="status failure"> (str(message)) </p>
      | `Loaded([], _status) =>
          <p className="status empty"> (str("No aliases.")) </p>
      | `Loaded(lst, status) =>
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
          (switch (status) {
          | `Fresh | `Stale => ReasonReact.nullElement
          | `Reloading => 
            <p className="status reloading"> (str("Reloading...")) </p>
          })
          <div className="enabled">
            <h2>(str("Enabled"))</h2>
            (enabled |> elementsOfAliases)
          </div>
          <div className="disabled">
            <h2>(str("Disabled"))</h2>
            (disabled |> elementsOfAliases)
          </div>
        </div>
      };
      
      <section className="list">
        (body)
      </section>
    }
  }
};