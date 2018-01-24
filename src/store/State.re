
type saveStatus =
  [ `Saving
  | `Saved
  | `Failed(string) ];

type messageType =
  [ `Info
  | `Error
  | `Warning ];

type t = {
  aliasStatuses: Js.Dict.t(saveStatus),
  dataIsStale: bool,
  messages: list((messageType, string))
};

let initial = {
  aliasStatuses: Js.Dict.empty(),
  dataIsStale: false,
  messages: []
};

let cloneDict = (d) => d |> Js.Dict.entries |> Js.Dict.fromArray;

let setAliasStatus = (state, id, status) => {
  let aliasStatuses = cloneDict(state.aliasStatuses);
  Js.Dict.set(aliasStatuses, id, status);
  aliasStatuses
};