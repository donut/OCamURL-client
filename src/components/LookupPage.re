
[@bs.val] external winLocation : Dom.location = "window.location";

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
          [| <div key="generateAliasButton" className="button-add-alias">
               <GenerateAliasButton url state />
             </div>,
             <ListAliases key="listAliases" url aliasList=state.aliasList /> |]
          |> ReasonReact.arrayToElement
            
      };

      let messages = state.messages
        |> List.map(((typ, text, _expires)) => {
          <p className=("message " ++ State.stringOfMessageType(typ))>
            (str(text))
          </p>
        })
        |> Array.of_list
        |> ReasonReact.arrayToElement;

      let bookmarklet = {
        [ "javascript:window.location=\"",
          LocationRe.protocol(winLocation), "//", 
          LocationRe.host(winLocation),
          LocationRe.pathname(winLocation),
          "?url=\"+encodeURIComponent(window.location.href)"
        ]
        |> String.concat("")
      };

      <div className="doc">
        <header>
          <h1> (str(Config.title)) </h1>
          <a className="bookmarklet" href=bookmarklet>
            (str("Bookmarklet"))
          </a>
        </header>
        <section className="lookup-page">
          <section className="messages">
            (messages)
          </section>
          <LookupInput initialValue=(state.lookupInitialValue)
                      onSubmit=((u) => Store.dispatch(Action.SetLookupURL(u))) />
          (details)
        </section>
      </div>
    }
  }
};