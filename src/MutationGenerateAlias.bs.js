// Generated by BUCKLESCRIPT VERSION 2.1.0, PLEASE EDIT WITH CARE
'use strict';

var Curry                = require("bs-platform/lib/js/curry.js");
var Pervasives           = require("bs-platform/lib/js/pervasives.js");
var GraphqlTag           = require("graphql-tag");
var Url$ReactTemplate    = require("./url.bs.js");
var Store$ReactTemplate  = require("./Store/Store.bs.js");
var Action$ReactTemplate = require("./Store/Action.bs.js");
var Apollo$ReactTemplate = require("./apollo.bs.js");

var mutation = GraphqlTag("\n  mutation GenerateAlias($input: GenerateAliasInput!) {\n    generateAlias(input: $input) {\n      error { code, message }\n      payload {\n        alias { id, name }\n      }\n    }\n  }\n");

var request = /* `Mutation */[
  1035765577,
  mutation
];

function deconstructResponse(response) {
  return /* tuple */[
          response.generateAlias.payload,
          response.generateAlias.error
        ];
}

var Config = /* module */[
  /* request */request,
  /* deconstructResponse */deconstructResponse
];

var Request = Apollo$ReactTemplate.Request(Config);

function run(id, url) {
  Store$ReactTemplate.dispatch([
        Action$ReactTemplate.GeneratingAlias,
        id
      ]);
  var now = Pervasives.string_of_float(Date.now());
  var input = {
    clientMutationId: "generateAlias///" + now,
    url: Url$ReactTemplate.toGql(url)
  };
  var variables = /* Some */[{
      input: input
    }];
  Curry._1(Request[/* send */3], variables).then((function (result) {
          if (result[0] >= 981919598) {
            Store$ReactTemplate.dispatch([
                  Action$ReactTemplate.GeneratedAlias,
                  id,
                  result[1].alias.name
                ]);
          } else {
            var message = Apollo$ReactTemplate.messageOfExn("generating alias", url, result[1]);
            Store$ReactTemplate.dispatch([
                  Action$ReactTemplate.GeneratingAliasFailed,
                  id,
                  message
                ]);
          }
          return Promise.resolve(/* () */0);
        }));
  return /* () */0;
}

exports.mutation = mutation;
exports.Config   = Config;
exports.Request  = Request;
exports.run      = run;
/* mutation Not a pure module */
