
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

type t = {
  aliasStatuses: Js.Dict.t(saveStatus),
  listIsStale: bool,
  lookupInitialValue: string,
  lookupURL: lookupURLStatus,
  messages: list((messageType, string))
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
  listIsStale: false,
  lookupInitialValue,
  lookupURL: `Unset,
  messages: []
};

let cloneDict = (d) => d |> Js.Dict.entries |> Js.Dict.fromArray;

let setAliasStatus = (state, id, status) => {
  let aliasStatuses = cloneDict(state.aliasStatuses);
  Js.Dict.set(aliasStatuses, id, status);
  aliasStatuses
};