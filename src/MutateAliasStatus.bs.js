// Generated by BUCKLESCRIPT VERSION 2.1.0, PLEASE EDIT WITH CARE
'use strict';

var Curry                = require("bs-platform/lib/js/curry.js");
var Pervasives           = require("bs-platform/lib/js/pervasives.js");
var GraphqlTag           = require("graphql-tag");
var Apollo$ReactTemplate = require("./apollo.bs.js");

var disableMutation = GraphqlTag("\n  mutation DisableAlias($input: DisableAliasInput!) {\n    disableAlias(input: $input) {\n      error { code, message }\n      payload { clientMutationId }\n    }\n  }\n");

var enableMutation = GraphqlTag("\n  mutation EnableAlias($input: EnableAliasInput!) {\n    enableAlias(input: $input) {\n      error { code, message }\n      payload { clientMutationId }\n    }\n  }\n");

var Config = /* module */[];

var request = /* `Mutation */[
  1035765577,
  disableMutation
];

function deconstructResponse(response) {
  return /* tuple */[
          response.disableAlias.payload,
          response.disableAlias.error
        ];
}

var DisableConfig = /* module */[
  /* request */request,
  /* deconstructResponse */deconstructResponse
];

var request$1 = /* `Mutation */[
  1035765577,
  enableMutation
];

function deconstructResponse$1(response) {
  return /* tuple */[
          response.enableAlias.payload,
          response.enableAlias.error
        ];
}

var EnableConfig = /* module */[
  /* request */request$1,
  /* deconstructResponse */deconstructResponse$1
];

var DisableRequest = Apollo$ReactTemplate.Request(DisableConfig);

var EnableRequest = Apollo$ReactTemplate.Request(EnableConfig);

function run(name, action) {
  var now = Pervasives.string_of_float(Date.now());
  var actionStr = action >= 756818595 ? "enable" : "disable";
  var input = {
    clientMutationId: name + ("///" + (actionStr + ("///" + now))),
    name: name
  };
  var variables = /* Some */[{
      input: input
    }];
  if (action >= 756818595) {
    return Curry._1(EnableRequest[/* send */3], variables);
  } else {
    return Curry._1(DisableRequest[/* send */3], variables);
  }
}

exports.disableMutation = disableMutation;
exports.enableMutation  = enableMutation;
exports.Config          = Config;
exports.DisableConfig   = DisableConfig;
exports.EnableConfig    = EnableConfig;
exports.DisableRequest  = DisableRequest;
exports.EnableRequest   = EnableRequest;
exports.run             = run;
/* disableMutation Not a pure module */