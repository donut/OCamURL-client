let str = ReasonReact.stringToElement;

type state = {
	value: string
};

type action =
	| Change(string)
	| Submit
	| KeyDown(string);

let component = ReasonReact.reducerComponent("LookupInput");

let make = (~initialValue, ~onSubmit, _children) => {
	let handleSubmit = (state) => {
		let text = String.trim(state.value);
		ReasonReact.UpdateWithSideEffects(
			{...state, value: text},
			(_self) => onSubmit(text)
		)
	};
	let change = (event) => {
		let el = event |> ReactEventRe.Form.target |> ReactDOMRe.domElementToObj;
		Change(el##value)
	};
	let keyDown = (event) => KeyDown(ReactEventRe.Keyboard.key(event));
	let submit = (_event) => Submit;
	{
		...component,

		initialState: () => {
			value: initialValue
		},

		reducer: (action, state) => switch action {
			| Change(value) =>
				ReasonReact.Update({...state, value})
			| Submit | KeyDown("Enter") =>
				handleSubmit(state)
			| KeyDown(_) => ReasonReact.NoUpdate
		},

		render: ({ state: { value }, reduce }) =>
			<div className="lookup">
				<input _type="url" placeholder="Paste a URL" value
					onChange=(reduce(change))
					onBlur=(reduce(submit))
					onKeyDown=(reduce(keyDown))
				/>
			</div>
	}
};