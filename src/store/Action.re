
type aliasID = string;
type message = string;

type ReduxThunk.thunk(_) +=
  | SetLookupURL(State.lookupURLStatus)

  | ListIsFresh
  | ListIsStale
   
  | GeneratingAlias(aliasID)
  | GeneratedAlias(aliasID, string)
  | GeneratingAliasFailed(aliasID, message)
;

type ReduxThunk.thunk('a) +=
  | ReplaceState ('a)
 ;
