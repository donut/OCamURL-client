
[@bs.val] external encodeURIComponent : string => string = "encodeURIComponent";
[@bs.val] external decodeURIComponent : string => string = "decodeURIComponent";

module Scheme = {
	type t = [ `HTTP | `HTTPS ];
	exception UnsupportedScheme(string);
	let ofString = (s) : t => switch (String.lowercase(s)) {
		| "http"  | "http:"  => `HTTP
		| "https" | "https:" => `HTTPS
		| _ => raise(UnsupportedScheme(s))
	};
	let ofStringOpt = (s) : option(t) =>
		try (Some(ofString(s))) { | _ => None };
	let toString : t => string = fun
		| `HTTP => "http" | `HTTPS => "https";
};

module Path = {
	type t = string;
	let ofString = (s) => s;
	let toString = (t) => 
		String.length(t) > 0 && String.get(t, 0) != '/'
		? "/" ++ t
		: t;
};

module Params = {
	type pair = { key: string, value: option(string) };
	type t = list(pair);

	let ofList = (l) => l;
	let toList = (t) => t;

	let ofString = (s:string) : t => {
		let dec = decodeURIComponent;
		switch (s |> Js.String.replaceByRe([%bs.re "/^\\?/"], "")) {
		| "" => []
		|  s => s
			|> Js.String.split("&") |> Array.to_list
			|> List.map(Js.String.split("=")) |> List.map(Array.to_list)
			|> List.map(fun
				| [] => { key: "", value: None }
				| [key] => { key: dec(key), value: None }
				| [key, value] => { key: dec(key), value: Some(dec(value)) }
				| [key, ...value] => {
					key: dec(key),
					value: Some(String.concat("=", value) |> dec)
				}
			)
		}
	};

	let toString = (t) => {
		let pairUp = ({key, value}) => {
			let enc = encodeURIComponent;
			let value' = switch value {
				| None => ""
				| Some(v) => "=" ++ enc(v) };
			enc(key) ++ value'
		};
		t |> List.map(pairUp) |> String.concat("&")
	};
};

type t = {
	id: option(int),
	scheme: Scheme.t,
	user: option(string),
	pass: option(string),
	host: string,
	path: Path.t,
	port: option(int),
	params: option(Params.t),
	fragment: option(string),
};

let make = (~id=?, ~scheme, ~user=?, ~pass=?, ~host, ~port=?, ~path="",
						~query=?, ~fragment=?, ()) =>
	{
		id,
		scheme,
		user,
		pass,
		host,
		port,
		path: Path.ofString(path),
		params: Opt.map(Params.ofString, query),
		fragment
	};

exception MissingPart(string, string);

let ofString = (s) => {
	let a : Js.t({..}) = [%bs.raw "document.createElement('a')"];
	ignore @@ [%bs.raw {| a.href = s |}];

	let prop = (name) => {
		let get : (string, Js.t({..})) => string = [%bs.raw {|
			function (key, obj) { return obj[key] }
			|}];
		get(name, a)
	};
	let propOpt = (name) => switch (prop(name)) {
		| "" => None
		|  s => Some(s)
	};

	/* Since we set the passed URL `s` to the `href` of an <a>, if what is 
	 * passed doesn't include a protocol and domain, it assumes it's relative
	 * to the current protocol and domain. That's likely not the intention of 
	 * the user. This verifies what may have been assumed actually exists in the
	 * passed URL. */
	let scheme = prop("protocol");
	if (not @@ Js.String.includes(scheme, s)) {
		raise(MissingPart("protocol (\"http://\" or \"https://\")", s))
	};

	let hostname = prop("hostname");
	if (not @@ Js.String.includes(hostname, s)) {
		raise(MissingPart("domain (example.com)", s))
	};

	make(
		~scheme = scheme |> Scheme.ofString,
		~user =? propOpt("username"),
		~pass =? propOpt("password"),
		~host = hostname,
		~port =? propOpt("port") |> Opt.map(int_of_string),
		~path = prop("pathname"),
		~query =? propOpt("search"),
		~fragment =? propOpt("hash"),
		()
	)
};

let ofStringOpt = (s) => 
	try (Some(ofString(s))) { | _ => None };

type gqlParamPair = {.
	"key": string, "value": option(string)
};

type gqlT = {.
	"scheme": Scheme.t,
	"user": option(string),
	"password": option(string),
	"host": string,
	"port": option(int),
	"path": option(string),
	"params": option(array(gqlParamPair)),
	"fragment": option(string)
};

let toGql = (t) : gqlT => {
	let paramsToGql = (p:Params.t) =>
		p |> Params.toList
			|> List.map((p) => Params.({
				"key": p.key,
				"value": p.value
			}))
			|> Array.of_list;
	{
		"scheme": t.scheme,
		"user": t.user,
		"password": t.pass,
		"host": t.host,
		"port": t.port,
		"path": Some(Path.toString(t.path)),
		"params": t.params |> Opt.map(paramsToGql),
		"fragment": t.fragment
	}
};