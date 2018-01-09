
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
  status: Alias.Status.t,
  error: string
};

type action = 
  | EnterRenameMode
  | InputChange(string)
  | Rename
  | Enable
  | Disable
  | Delete
  | Saved
  | Error(string)
  | Nevermind;

let component = ReasonReact.reducerComponent("AliasWidget");

let make = (~alias, _children) => {
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
    Js.Promise.(
      MutateAliasName.run(~name=Alias.name(alias), ~newName=name)
      |> then_((result:MutateAliasName.Request.result) => {
        switch result {
        | `Exn(exn) => switch exn {
          | MutateAliasName.Request.ResponseError(_code, message) => 
            reduce((_self) => Error("Failed renaming: " ++ message))()
          | exn => 
            Js.log2(
              "Failed renaming alias [" ++ Alias.name(alias) ++ "] to ["
                ++ name ++ "]",
              exn
            );
            reduce((_self) => Error("Failed renaming alias. See console."))()
          }
        | `Payload(_) => 
          reduce((_self) => Saved)()
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
      | `Exn(exn) => switch exn {
        | MutateAliasStatus.DisableRequest.ResponseError(_code, message) => 
          reduce((_self) => Error("Failed disabling: " ++ message))()
        | exn => 
          Js.log2("Failed disabling alias [" ++ Alias.name(alias) ++ "].", exn);
          reduce((_self) => Error("Failed disabling alias. See console."))()
        }
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
      | `Exn(exn) => switch exn {
        | MutateAliasStatus.EnableRequest.ResponseError(_code, message) => 
          reduce((_self) => Error("Failed enabling: " ++ message))()
        | exn => 
          Js.log2("Failed enabling alias [" ++ Alias.name(alias) ++ "].", exn);
          reduce((_self) => Error("Failed enabling alias. See console."))()
        }
      | `Payload(_) =>
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
        error: "",
        name: Alias.name(alias),
        status: Alias.status(alias)
      }
    },

    reducer: (action, state) => switch action {
      | EnterRenameMode =>
        ReasonReact.Update({...state, mode: Rename})
      | InputChange(name) =>
        ReasonReact.Update({...state, name})
      | Rename => switch (state.name == Alias.name(alias)) {
        | true => ReasonReact.Update({...state, mode: Static })
        | false =>
          ReasonReact.UpdateWithSideEffects(
            {...state, mode: Static, saving: Yes },
            ({ state: { name }, reduce }) => renameAlias(name, reduce)
          )
        }
      | Enable => switch (state.status == `Enabled) {
        | true => ReasonReact.NoUpdate
        | false => 
          ReasonReact.UpdateWithSideEffects(
            {...state, saving: Yes, status: `Enabled},
            ({ state: { name }, reduce }) => enableAlias(name, reduce)
          )
        }
      | Disable => switch (state.status == `Disabled) {
        | true => ReasonReact.NoUpdate
        | false => 
          ReasonReact.UpdateWithSideEffects(
            {...state, saving: Yes, status: `Disabled},
            ({ state: { name }, reduce }) => disableAlias(name, reduce)
          )
        }
      | Delete => 
        ReasonReact.Update({...state,
          mode: Deleted, saving: Yes, status: `Disabled })
      | Saved =>
        ReasonReact.Update({...state, saving: No})
      | Error(error) => 
        ReasonReact.Update({...state, saving: Error, error})
      | Nevermind =>  
        ReasonReact.NoUpdate
    },

    render: ({ state: { mode, saving, error, name, status }, reduce }) => {
      let id = Alias.id(alias) |> string_of_int |> (++)("alias-");
      
      let header = switch (mode, saving) {
        | (Deleted, _) | (_, Yes) =>
          (name |> str)
        | (Static, _) => 
          <button _type="button" onClick=(reduce(handleHeaderClick))>
            (name |> str)
          </button>
        | (Rename, _) => 
          <input value=name required=Js.true_ autoFocus=Js.true_ 
            onChange=(reduce(handleChange))
            onBlur=(reduce((_) => Rename))
            onKeyDown=(reduce(actOnEnter(Rename)))
          />
      };

      let className = "saving-" ++ SavingStatus.toString(saving);

      let message = switch saving {
      | No => ReasonReact.nullElement
      | Yes => <div className="saving-status"> (str("Saving...")) </div>
      | Error => <div className="saving-status error"> (str(error)) </div>
      };

      let statusToggle = switch status {
      | `Disabled =>
        <button className="status enable"
                onClick=(reduce((_) => Enable))
                disabled=(SavingStatus.toBool(saving) |> toJsBool)>
          (str("Enable"))
        </button>
      | `Enabled => 
        <button className="status disable"
                onClick=(reduce((_) => Disable))
                disabled=(SavingStatus.toBool(saving) |> toJsBool)>
          (str("Disable"))
        </button>
      };
      
      <article id className>
        <h1> (header) </h1>
        <div className="status"> (Alias.Status.toString(status) |> str) </div>
        <div className="actions">
          (statusToggle)
        </div>
        (message)
      </article>
    }
  }
};