
module Status = {
  type t = [ `Enabled | `Disabled ];
  exception InvalidStatus(string);

  let ofString = (s) : t => switch (String.lowercase(s)) {
    | "enabled"  => `Enabled
    | "disabled" => `Disabled
    |          _ => raise(InvalidStatus(s))
  };

  let toString : t => string = fun
    | `Enabled => "enabled"
    | `Disabled => "disabled";
};

type t = {
  id: int,
  name: string,
  status: Status.t
};

let id = (t) => t.id;
let name = (t) => t.name;
let status = (t) => t.status;
let statusAsString = (t) => Status.toString(t.status);

type gqlT = {.
  "id": string,
  "name": string,
  "status": string
};

let ofGql = (g:gqlT) : t => {
  id: g##id |> int_of_string,
  name: g##name,
  status: g##status |> Status.ofString
};

