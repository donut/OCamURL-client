

let reducer = (state: State.t, action) => switch (action) {
  |  Action.SetLookupURL(lookupURL) => { ...state, lookupURL }

  | Action.ListIsFresh => { ...state, listIsStale: false }
  | Action.ListIsStale => { ...state, listIsStale: true  }

  | Action.GeneratingAlias(id) =>
    let aliasStatuses = State.setAliasStatus(state, id, `Saving);
    { ...state, aliasStatuses }

  | Action.GeneratedAlias(id, name) => 
    let aliasStatuses = State.setAliasStatus(state, id, `Saved);
    { ...state, aliasStatuses, listIsStale: true,
      messages: [(`Info, "Generated alias " ++ name ++ ".")] }

  | Action.GeneratingAliasFailed(id, message) =>
    let aliasStatuses = State.setAliasStatus(state, id, `Failed(message));
    { ...state, aliasStatuses, messages: [(`Error, message)] }
  | _ => state
};

let thunkedLogger = (store, next) => 
  StoreMiddleware.thunk(store) @@ StoreMiddleware.logger(store) @@ next;

let store = Reductive.Store.create(
  ~reducer, ~preloadedState=State.initial, ~enhancer=thunkedLogger,
  ()
);

let dispatch = Reductive.Store.dispatch(store);