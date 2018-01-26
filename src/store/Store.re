
let reducer = (state: State.t, action) => switch (action) {
  |  Action.SetMessage(typ, text) =>
    { ...state, messages: State.addMessage(state, typ, text) }

  |  Action.SetLookupURL(lookupURL) =>
    let aliasList = 
      (lookupURL == state.lookupURL) ? state.aliasList : `Unloaded;
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
    let messages =
      State.addMessage(state, `Info, "Generated alias " ++ name ++ ".");
    { ...state, aliasStatuses, aliasList, messages }

  | Action.GeneratingAliasFailed(id, message) =>
    let aliasStatuses = State.setAliasStatus(state, id, `Failed(message));
    let messages = State.addMessage(state, `Error, message);
    { ...state, aliasStatuses, messages }
  | _ => state
};

let reducerWrap = (state: State.t, action) => {
  let messages = State.clearExpiredMessages(state);
  let state' = { ...state, messages };
  reducer(state', action)
};

let thunkedLogger = (store, next) => 
  StoreMiddleware.thunk(store)
    @@ StoreMiddleware.logger(store)
    @@ next;

let store = Reductive.Store.create(
  ~reducer=reducerWrap, ~preloadedState=State.initial, ~enhancer=thunkedLogger,
  ()
);

let dispatch = Reductive.Store.dispatch(store);