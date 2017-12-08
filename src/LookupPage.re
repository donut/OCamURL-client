let str = ReasonReact.stringToElement;

let optDefault = (default) => fun
  | None => default
  | Some(x) => x;

type state = {
  input: LookupInput.status
};

type action =
  | InputURLChange(LookupInput.status);

let component = ReasonReact.reducerComponent("LookupPage");

let make = (_children) => {
  let onURLInputSubmit = (url) => InputURLChange(url);

  {
    ...component,

    initialState: () => {
      { input: `Unset }
    },

    reducer: (action, state) => switch action {
      | InputURLChange(input) => ReasonReact.Update({...state, input})
    },

    render: ({ state: { input }, reduce }) => {
      let urlInQuery =
        DomRe.location |> LocationRe.search
        |> Url.Params.ofString |> Url.Params.toList |> List.rev
        |> List.find_all(
            (p:Url.Params.pair) => String.lowercase(p.key) == "url");
      let initialValue = switch urlInQuery {
          | [] => ""
          | [last, ..._] => optDefault("", last.value)
          };

      let details = switch input {
        | `Unset => ReasonReact.nullElement 
        | `Invalid(reason) => 
          <p className="error invalid-input">
            (str({j|That doesn't look like a valid URL. $reason|j}))
          </p>
        | `Valid(_url) =>
          <p> (str("The URL looks valid.")) </p>
      };

      <section>
        <h1> (str("OCamURL")) </h1>
        <p> (str("Enter a URL or alias below.")) </p>
        <LookupInput initialValue onSubmit=(reduce(onURLInputSubmit)) />
        (details)
      </section>
    }
  }
};