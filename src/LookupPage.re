let str = ReasonReact.stringToElement;

let component = ReasonReact.statelessComponent("LookupPage");

let make = (~state: State.t, ~dispatch as _, _children) => {
  {
    ...component,

    render: (_self) => {
      let details = switch (state.lookupURL) {
        | `Unset => ReasonReact.nullElement 
        | `Invalid(reason) => 
          <p className="error invalid-input">
            (str({j|That doesn't look like a valid URL: $reason|j}))
          </p>
        | `Valid(url) =>
          <div>
            <GenerateAliasButton url state />
            <ListAliases url state />
          </div>
      };

      <section>
        <h1> (str("OCamURL")) </h1>
        <p> (str("Enter a URL or alias below.")) </p>
        <LookupInput initialValue=(state.lookupInitialValue)
                     onSubmit=((u) => Store.dispatch(Action.SetLookupURL(u))) />
        (details)
      </section>
    }
  }
};