
module StoreProvider = {
  let make = Reductive.Provider.createMake(Store.store);
};


ReactDOMRe.renderToElementWithId(
  <StoreProvider component=LookupPage.make />,
  "index"
);
