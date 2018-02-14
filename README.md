# OCamURL Client #

A web client for [OCamURL Server URL Shortener][ocurl-server].

[ocurl-server]: /rightthisminute/ocamurl-server

## Building ##

```
$ yarn install
$ yarn build
$ cp config.example.js config.js
# Set your OCamURL-server settings in config.js.
```

Point your web server to [/public/index.html].

## Areas for Improvement ##

This was my first project in ReasonML and it shows. I am sure there is more room for improvement than I can see at this point. However, here are some big items.

### Replace Reductive with Reducer Components ###

I decided to use [Reductive][], an implementation of Redux in ReasonML, just over halfway into the project. For some reason the built-in reducers in [ReasonReact][] just weren't clicking for me at the project level. I've used Redux in TypeScript/JS projects and so I modeled my use of Reductive on that. However, with that experience came clarity of how to use ReasonReact's reducer components to fill the same hole.

Probably the first major refactor should be to move the state handling to reducer components, passing down state and reducer functions as props. This will make it easier to see which components modify state and what state they actually care about. Global state can live in the root component.

[Reductive]: https://github.com/reasonml-community/reductive
[ReasonReact]: https://github.com/reasonml/reason-react


### Look into HOC for GraphQL Queries ###

[Reason Apollo][] basically only has one way to handle quries and mutations: higher-order components (HOC). This hasn't clicked for me, yet. The main issues I see are how side-effects and state updates are handled with HOC queries and mutations. Some more [details on my confusion][my-reasons]. And someone else who's [running into trouble][issue].

For now I've built around this with the [`Apollo.Request`][Apollo.Request] functor, which generates non-component modules for queries and mutations. It also includes special error handlers for how [the unconventional way OCamURL-server handles errors][server-err-handling].

[Reason Apollo]: https://github.com/apollographql/reason-apollo
[my-reasons]: https://github.com/apollographql/reason-apollo/pull/6#issuecomment-352019099
[issue]: https://github.com/apollographql/reason-apollo/issues/33
[Apollo.Request]: blob/master/src/apollo.re
[server-err-handling]: /rightthisminute/ocamurl-server#graphql-error-handling


## Project Expectations ##

This is a project for use at RightThisMinute. There is no promise of support or continued work in this public repo. It is being released mainly to benefit those who are starting out in ReasonML. It's likely a poor example, but being simpler may help make some things clear. This isn't to say we're not accepting issues and pull requests, just know that we may have selfish motivations in what we integrate.