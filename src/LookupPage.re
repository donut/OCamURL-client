let str = ReasonReact.stringToElement;

type state = {
  input: string
};

let component = ReasonReact.reducerComponent("LookupPage");

let make = (_children) => {
  let onSubmit = (text) => Js.log2("Bingi:", text);
  {
    ...component,
    initialState: () => {
      input: ""
    },
    reducer: ((), _) => ReasonReact.NoUpdate,
    render: ({ state: { input } }) =>
      <section>
        <h1> (str("OCamURL")) </h1>
        <p> (str("Enter a URL or alias below.")) </p>
        <LookupInput initialValue="Hi there" onSubmit />
      </section>
  }
};