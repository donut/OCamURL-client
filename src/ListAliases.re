
let str = ReasonReact.stringToElement;

let component = ReasonReact.statelessComponent("QueryAliases");

let make = (~url, ~state as appState: State.t, _children) => {
  {
    ...component,

    didMount: (_self) => {
      QueryAliases.run(~url);
      ReasonReact.NoUpdate
    },

    willReceiveProps: (_self) => {
      switch (appState.aliasList) {
      | `Loaded(_lst, `Stale) => QueryAliases.reload(~url)
      | _ => ()
      };
      ()
    },

    render: (_self) => {
      let body = switch (appState.aliasList) {
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