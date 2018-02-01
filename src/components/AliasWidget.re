
let str = ReasonReact.stringToElement;
let toJsBool = Js.Boolean.to_js_boolean;

module Mode = {
  type t = Static | Rename | Deleted;

  let toString = fun
    | Static => "static"
    | Rename => "rename"
    | Deleted => "deleted";
};

module SavingStatus = {
 type t = No | Yes | Error;

 let toBool = fun
  | No | Error => false
  | Yes => true;
  
 let toString = fun
  | No => "no"
  | Yes => "yes"
  | Error => "error";
};

type state = {
  mode: Mode.t,
  saving: SavingStatus.t,
  name: string,
  status: Alias.Status.t
};

type action = 
  | EnterRenameMode
  | InputChange(string)
  | Rename
  | Enable
  | Disable
  | Delete
  | Saved
  | DisabledAndAdded
  | Error(string)
  | Nevermind;

let handleExn = (failedAction, ~id, ~exn, ~reduce) => switch exn {
  | Apollo.ResponseError(_code, message) => 
    reduce((_self) => Error(failedAction ++ ": " ++ message))()
  | exn => 
    Js.log2(failedAction ++ " [" ++ id ++ "]", exn);
    reduce((_self) => Error(failedAction ++ ". See console."))()
};

let component = ReasonReact.reducerComponent("AliasWidget");

let make = (~alias, ~onChange, _children) => {
  let handleHeaderClick = (_event) => EnterRenameMode;

  let handleChange = (event) => {
    let el = event |> ReactEventRe.Form.target |> ReactDOMRe.domElementToObj;
    InputChange(el##value)
  };
  
  let actOnEnter = (action, event) => 
    switch (ReactEventRe.Keyboard.key(event) |> String.lowercase) {
    | "enter" => action
    |       _ => Nevermind
    };

  let renameAlias = (name, reduce) => {
    let oldName = Alias.name(alias);

    Js.Promise.(
      MutateAliasName.run(~name=oldName, ~newName=name)

      |> then_((result:MutateAliasName.Request.result) => {
        switch result {
        | `Exn(exn) =>
          handleExn("Failed renaming alias", ~id=Alias.name(alias),
                    ~exn, ~reduce)
        | `Payload(p) => switch (p##actionTaken) {
          | `DisableAndAdd =>
            let message = {j|
              A new alias [$name] was created and [$oldName] was disabled as 
              it already has been used. 
            |j};
            Store.dispatch(Action.SetMessage(`Warning, message));
            reduce((_self) => DisabledAndAdded)()
          | _ => reduce((_self) => Saved)()
          }
        };
        resolve()
      })

      |> ignore
    );
    ()
  };

  let disableAlias = (name, reduce) => {
    MutateAliasStatus.run(~name, `Disable) 
    |> Js.Promise.then_((result:MutateAliasStatus.DisableRequest.result) => {
      switch result {
      | `Exn(exn) =>
        handleExn("Failed disabling alias", ~id=Alias.name(alias),
                  ~exn, ~reduce)
      | `Payload(_) =>
        reduce((_self) => Saved)()
      };
      Js.Promise.resolve()
    })
    |> ignore;
    ()
  };

  let enableAlias = (name, reduce) => {
    MutateAliasStatus.run(~name, `Enable) 

    |> Js.Promise.then_((result:MutateAliasStatus.EnableRequest.result) => {
      switch result {
      | `Exn(exn) =>
        handleExn("Failed enabling alias", ~id=Alias.name(alias),
                  ~exn, ~reduce)
      | `Payload(_) =>
        reduce((_self) => Saved)()
      };
      Js.Promise.resolve()
    })
    |> ignore;

    ()
  };

  let deleteAlias = (name, reduce) => {
    MutationDeleteAlias.run(~name)

    |> Js.Promise.then_((result:MutationDeleteAlias.Request.result) => {
      switch result {
      | `Exn(exn) => 
        handleExn("Failed deleting alias", ~id=Alias.name(alias), ~exn, ~reduce)
      | `Payload(p) =>
        if (p##actionTaken == `Disable) {
          let message = {j|
            The alias [$name] was only disabled since it's already been used
            and so cannot be deleted.
          |j};
          Store.dispatch(Action.SetMessage(`Warning, message))
        };
        reduce((_self) => Saved)()
      };
      Js.Promise.resolve()
    })
    |> ignore;

    ()
  };

  {
    ...component,

    initialState: () => {
      {
        mode: Static,
        saving: No,
        name: Alias.name(alias),
        status: Alias.status(alias)
      }
    },

    reducer: (action, state) => {
      let currentlySaving = SavingStatus.toBool(state.saving);
      switch (action, currentlySaving) {
      | (EnterRenameMode, false) =>
        ReasonReact.Update({...state, mode: Rename})
      | (InputChange(name), false) =>
        ReasonReact.Update({...state, name})
      | (Rename, false) => switch (state.name == Alias.name(alias)) {
        | true => ReasonReact.Update({...state, mode: Static })
        | false =>
          ReasonReact.UpdateWithSideEffects(
            {...state, mode: Static, saving: Yes },
            ({ state: { name }, reduce }) => renameAlias(name, reduce)
          )
        }
      | (Enable, false) => switch (state.status == `Enabled) {
        | true => ReasonReact.NoUpdate
        | false => 
          ReasonReact.UpdateWithSideEffects(
            {...state, saving: Yes, status: `Enabled},
            ({ state: { name }, reduce }) => enableAlias(name, reduce)
          )
        }
      | (Disable, false) => switch (state.status == `Disabled) {
        | true => ReasonReact.NoUpdate
        | false => 
          ReasonReact.UpdateWithSideEffects(
            {...state, saving: Yes, status: `Disabled},
            ({ state: { name }, reduce }) => disableAlias(name, reduce)
          )
        }
      | (Delete, false) =>
        ReasonReact.UpdateWithSideEffects(
          {...state, mode: Deleted, saving: Yes, status: `Disabled },
          ({ state: { name }, reduce }) => deleteAlias(name, reduce)
        )
      | (Saved, true) =>
        ReasonReact.UpdateWithSideEffects(
          {...state, saving: No},
          (_self) => onChange()
        )
      | (DisabledAndAdded, true) =>
        ReasonReact.UpdateWithSideEffects(
          {...state, name: Alias.name(alias), status: `Disabled, saving: No},
          (_self) => onChange()
        )
      | (Error(message), _saving) => 
        ReasonReact.UpdateWithSideEffects(
          { ...state,
            name: Alias.name(alias),
            status: Alias.status(alias),
            saving: Error },
          (_self) => Action.SetMessage(`Error, message) |> Store.dispatch
        )
      | (Nevermind, _saving)
      | (        _, _saving) =>  
        ReasonReact.NoUpdate
      }
    },

    render: ({ state: { mode, saving, name, status }, reduce }) => {
      let id = Alias.id(alias) |> string_of_int |> (++)("alias-");
      
      let header = switch (mode, saving) {
        | (Deleted, _) | (_, Yes) =>
          <span> (name |> str) </span>
        | (Static, _) => 
          <span onClick=(reduce(handleHeaderClick))>
            (name |> str)
          </span>
        | (Rename, _) => 
          <input value=name required=Js.true_ autoFocus=Js.true_ 
            onChange=(reduce(handleChange))
            onBlur=(reduce((_) => Rename))
            onKeyDown=(reduce(actOnEnter(Rename)))
          />
      };

      let className = "saving-" ++ SavingStatus.toString(saving);

      let message = switch saving {
      | No | Error => ReasonReact.nullElement
      | Yes => <div className="saving-status"> (str("Saving...")) </div>
      };

      let statusToggle = {
        let (label, action) = switch status {
        | `Disabled => ("Enable", Enable)
        | `Enabled => ("Disable", Disable)
        };
        <button className=("status " ++ Js.String.toLowerCase(label))
                onClick=(reduce((_) => action))
                disabled=(SavingStatus.toBool(saving) |> toJsBool)>
          (str(label))
        </button>
      }; 

      let deleteAction = {
        <button className="delete"
                onClick=(reduce((_) => Delete))
                disabled=(SavingStatus.toBool(saving) |> toJsBool)>
          (str("Delete"))
        </button>
      };
      
      <article id className>
        <h1> (header) </h1>
        <div className="status"> (Alias.Status.toString(status) |> str) </div>
        <div className="actions">
          <CopyToClipboard text=(Config.aliasPrefix ++ name)>
            <button className="copy">(str("Copy"))</button>
          </CopyToClipboard>
          (statusToggle)
          (deleteAction)
        </div>
        (message)
      </article>
    }
  }
};