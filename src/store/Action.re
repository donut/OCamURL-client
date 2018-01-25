
type aliasID = string;

type ReduxThunk.thunk(_) +=
  | SetLookupURL(State.lookupURLStatus)

  | AliasListLoading
  | AliasListLoadingFailed(string)
  | AliasListLoaded(list(Alias.t))
  | AliasListIsStale
   
  | GeneratingAlias(aliasID)
  | GeneratedAlias(aliasID, string)
  | GeneratingAliasFailed(aliasID, string)
;

type ReduxThunk.thunk('a) +=
  | ReplaceState ('a)
 ;
