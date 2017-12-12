
let str = ReasonReact.stringToElement;

type state = {
  saving: bool
};

let component = ReasonReact.reducerComponent("AliasWidget");

let make = (~alias, _children) => {
  {
    ...component,

    initialState: () => {
      { saving: false }
    },

    reducer: ((), _) => ReasonReact.NoUpdate,

    render: ({ state: { saving }, reduce }) => {
      let id = Alias.id(alias) |> string_of_int;
      let className = saving ? "status-saving" : "status-saved";
      <article id=("alias-" ++ id) className>
        <h1> (Alias.name(alias) |> str) </h1>
        <div className="status"> (Alias.statusAsString(alias) |> str) </div>
      </article>
    }
  }
};