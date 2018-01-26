
type saveStatus =
  [ `Saving
  | `Saved
  | `Failed(string) ];

type messageType =
  [ `Info
  | `Error
  | `Warning ];

type lookupURLStatus =
  [ `Valid(Url.t)
  | `Invalid(string)
  | `Unset ];

type aliasListStatus =
  [ `Fresh
  | `Stale
  | `Reloading ];

type aliasList = 
  [ `Unloaded
  | `Loading
  | `Failed(string)
  | `Loaded(list(Alias.t), aliasListStatus) ];

type t = {
  aliasStatuses: Js.Dict.t(saveStatus),
  aliasList: aliasList,
  lookupInitialValue: string,
  lookupURL: lookupURLStatus,
  messages: list((messageType, string, float))
};

let lookupInitialValue = {
  let urlInQuery =
    DomRe.location |> LocationRe.search
    |> Url.Params.ofString |> Url.Params.toList |> List.rev
    |> List.find_all((p:Url.Params.pair) => String.lowercase(p.key) == "url");
  switch (urlInQuery) {
    | [] => ""
    | [last, ..._] => switch (last.value) { | Some(u) => u | None => "" }
  }
};

let initial = {
  aliasStatuses: Js.Dict.empty(),
  aliasList: `Unloaded,
  lookupInitialValue,
  lookupURL: `Unset,
  messages: []
};

let cloneDict = (d) => d |> Js.Dict.entries |> Js.Dict.fromArray;

let markAliasListStale = (state) =>
  switch (state.aliasList) {
  | `Loaded(lst, `Fresh) => `Loaded(lst, `Stale)
  | _ => state.aliasList
  };

let setAliasStatus = (state, id, status) => {
  let aliasStatuses = cloneDict(state.aliasStatuses);
  Js.Dict.set(aliasStatuses, id, status);
  aliasStatuses
};

let stringOfMessageType = fun
  | `Info    =>    "info"
  | `Warning => "warning"
  | `Error   =>   "error";

let addMessage = (state, typ, message) => {
  let expires = Js.Date.now() +. 500.;
  [(typ, message, expires), ...state.messages]
};

let clearExpiredMessages = (state) => {
    let now = Js.Date.now();
    state.messages |> List.filter(((_, _, expires)) => expires > now)
};