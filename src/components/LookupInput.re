let str = ReasonReact.stringToElement;

[@bs.val] external setTimeout : (unit => unit, int) => int = "";

type status = [ `Valid(Url.t) | `Invalid(string) | `Unset ];

let stringOfStatus = fun 
  | `Valid(_) => "valid" | `Invalid(_) => "invalid" | `Unset => "unset";

type state = {
  value: string,
  url: status,
  wasPasted: bool
};

type action =
  | Change(string, status)
  | ChangeAndSubmit(string, status)
  | Pasted
  | Submit
  | KeyDown(string);

let component = ReasonReact.reducerComponent("LookupInput");

let make = (~initialValue, ~onSubmit, _children) => {
  let checkURL = (url) => {
    switch (String.trim(url)) {
    | "" => `Unset
    | url => switch (Url.ofString(url)) {
      | url => `Valid(url)
      | exception Url.Scheme.UnsupportedScheme(s) => switch s {
        | "" => 
          `Invalid("The URL must start with HTTP or HTTPS.")
        |  x => 
          `Invalid({j|The scheme [$x] is not supported, only HTTP and HTTPS.|j})
        }
      | exception Url.MissingPart(p, _) => 
        `Invalid({j|Missing the $p part of the URL.|j})
      }
    }
  };
  let handleSubmit = (state) => {
    let text = String.trim(state.value);
    ReasonReact.UpdateWithSideEffects(
      { ...state, value: text },
      ({ state: { url } }) => onSubmit(url)
    )
  };
  let change = (pasted, event) => {
    let el = event |> ReactEventRe.Form.target |> ReactDOMRe.domElementToObj;
    let value = pasted ? Js.String.trim(el##value) : el##value;
    let url = checkURL(value);
    pasted ? ChangeAndSubmit(value, url) : Change(value, url)
  };
  let keyDown = (event) => KeyDown(ReactEventRe.Keyboard.key(event));

  {
    ...component,

    initialState: () => {
      value: String.trim(initialValue),
      url: checkURL(initialValue),
      wasPasted: false
    },

    reducer: (action, state) => switch action {
      | Change(value, url) => ReasonReact.Update({ ...state, value, url })
      | ChangeAndSubmit(value, url) =>
        ReasonReact.UpdateWithSideEffects(
          { value, url, wasPasted: false },
          ({ state: { url } }) => onSubmit(url)
        )
      | Pasted => ReasonReact.Update({ ...state, wasPasted: true })
      | Submit | KeyDown("Enter") => handleSubmit(state)
      | KeyDown(_) => ReasonReact.NoUpdate
    },

    didMount: ({ state: { url }}) => 
      ReasonReact.SideEffects((_) => onSubmit(url)),

    render: ({ state: { value, url, wasPasted }, reduce }) => {
      let status = stringOfStatus(url);

      <section className=("lookup-form " ++ status)>
        <label htmlFor="lookup-form-url-input"> (str("Enter a URL:")) </label>
        <input _type="url" id="lookup-form-url-input"
               placeholder="Paste a URL" autoFocus=Js.true_
               value
               onChange=(reduce(change(wasPasted)))
               onBlur=(reduce((_) => Submit))
               onPaste=(reduce((_) => Pasted))
               onKeyDown=(reduce(keyDown))
        />
      </section>
    }
  }
};