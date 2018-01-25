
let reducer = (state: State.t, action) => switch (action) {
  |  Action.SetLookupURL(lookupURL) =>
    let aliasList = 
      (lookupURL == state.lookupURL) ? state.aliasList : `Loading;
    { ...state, lookupURL, aliasList }

  | Action.AliasListLoading =>
    let aliasList =
      switch (state.aliasList) {
      | `Loaded(lst, _status) => `Loaded(lst, `Reloading)
      | _ => `Loading
      };
    { ...state, aliasList }
  
  | Action.AliasListLoadingFailed(string) =>
    { ...state, aliasList: `Failed(string) }

  | Action.AliasListLoaded(lst) =>
    { ...state, aliasList: `Loaded(lst, `Fresh) }
  
  | Action.AliasListIsStale =>
    let aliasList = State.markAliasListStale(state);
    { ...state, aliasList }

  | Action.GeneratingAlias(id) =>
    let aliasStatuses = State.setAliasStatus(state, id, `Saving);
    { ...state, aliasStatuses }

  | Action.GeneratedAlias(id, name) => 
    let aliasStatuses = State.setAliasStatus(state, id, `Saved);
    let aliasList = State.markAliasListStale(state);
    { ...state, aliasStatuses, aliasList,
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