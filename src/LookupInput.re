let str = ReasonReact.stringToElement;

type status = [ `Valid(Url.t) | `Invalid(string) | `Unset ];

let stringOfStatus = fun 
	| `Valid(_) => "valid" | `Invalid(_) => "invalid" | `Unset => "unset";

type state = {
	value: string,
	url: status,
};

type action =
	| Change(string, status)
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
			{...state, value: text},
			(_self) => onSubmit(state.url)
		)
	};
	let change = (event) => {
		let el = event |> ReactEventRe.Form.target |> ReactDOMRe.domElementToObj;
		let value = el##value;
		Change(value, checkURL(value))
	};
	let keyDown = (event) => KeyDown(ReactEventRe.Keyboard.key(event));
	let submit = (_event) => Submit;

	{
		...component,

		initialState: () => {
			value: String.trim(initialValue),
			url: checkURL(initialValue)
		},

		reducer: (action, state) => switch action {
			| Change(value, url) => ReasonReact.Update({...state, value, url})
			| Submit | KeyDown("Enter") => handleSubmit(state)
			| KeyDown(_) => ReasonReact.NoUpdate
		},

		didMount: ({ state: { url }}) => 
			ReasonReact.SideEffects((_) => onSubmit(url)),

		render: ({ state: { value, url }, reduce }) => {
			let status = stringOfStatus(url);

			<div className=("lookup " ++ status)>
				<input _type="url" placeholder="Paste a URL"
					value
					onChange=(reduce(change))
					onBlur=(reduce(submit))
					onKeyDown=(reduce(keyDown))
				/>
			</div>
		}
	}
};