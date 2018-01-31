
let str = ReasonReact.stringToElement;

type state = {
  id: string
};

let component = ReasonReact.reducerComponent("GenerateAliasButton");

let make = (~url, ~state as appState: State.t, _children) => {
  {
    ...component,

    initialState: (): state => {
      id: Js.Date.now() |> string_of_float
    },

    reducer: ((), _state: state) => ReasonReact.NoUpdate,

    render: ({ state: { id } }) => {
      let (isSaving, label) =
        switch (Js.Dict.get(appState.aliasStatuses, id)) {
        | Some(`Saving) => (true, "Generating...")
        | None | Some(`Saved)  => (false, "Add Alias")
        | Some(`Failed(_)) => (false, "Failed adding alias. Try again?")
        };

      <button className="add-alias"
              onClick=((_) => MutationGenerateAlias.run(~id, ~url))
              disabled=(isSaving |> Js.Boolean.to_js_boolean)>
        (str(label))
      </button>
    }
  }

};