// Generated by BUCKLESCRIPT VERSION 2.1.0, PLEASE EDIT WITH CARE
'use strict';

var $$Array              = require("bs-platform/lib/js/array.js");
var Curry                = require("bs-platform/lib/js/curry.js");
var Js_dict              = require("bs-platform/lib/js/js_dict.js");
var Js_json              = require("bs-platform/lib/js/js_json.js");
var Pervasives           = require("bs-platform/lib/js/pervasives.js");
var Caml_exceptions      = require("bs-platform/lib/js/caml_exceptions.js");
var Apollo$ReactTemplate = require("./apollo.bs.js");

var Graphql_error = Caml_exceptions.create("MutationDeleteAlias-ReactTemplate.Mutation.Graphql_error");

var query = "mutation DeleteAlias($input: DeleteAliasInput!)  {\ndeleteAlias(input: $input)  {\nerror  {\ncode  \nmessage  \n}\npayload  {\nactionTaken  \n}\n}\n}";

function parse(value) {
  var match = Js_json.decodeObject(value);
  if (match) {
    var value$1 = match[0]["deleteAlias"];
    var match$1 = Js_json.decodeObject(value$1);
    var tmp;
    if (match$1) {
      var value$2 = match$1[0];
      var value$3 = value$2["error"];
      var match$2 = Js_json.decodeNull(value$3);
      var tmp$1;
      if (match$2) {
        tmp$1 = /* None */0;
      } else {
        var match$3 = Js_json.decodeObject(value$3);
        var tmp$2;
        if (match$3) {
          var value$4 = match$3[0];
          var value$5 = value$4["code"];
          var match$4 = Js_json.decodeString(value$5);
          var tmp$3;
          if (match$4) {
            switch (match$4[0]) {
              case "BadRequest" : 
                  tmp$3 = /* BadRequest */-365540310;
                  break;
              case "InternalServerError" : 
                  tmp$3 = /* InternalServerError */-493013592;
                  break;
              default:
                throw Graphql_error;
            }
          } else {
            throw Graphql_error;
          }
          var value$6 = value$4["message"];
          var match$5 = Js_json.decodeString(value$6);
          var tmp$4;
          if (match$5) {
            tmp$4 = match$5[0];
          } else {
            throw Graphql_error;
          }
          tmp$2 = {
            code: tmp$3,
            message: tmp$4
          };
        } else {
          throw Graphql_error;
        }
        tmp$1 = /* Some */[tmp$2];
      }
      var value$7 = value$2["payload"];
      var match$6 = Js_json.decodeNull(value$7);
      var tmp$5;
      if (match$6) {
        tmp$5 = /* None */0;
      } else {
        var match$7 = Js_json.decodeObject(value$7);
        var tmp$6;
        if (match$7) {
          var value$8 = match$7[0]["actionTaken"];
          var match$8 = Js_json.decodeString(value$8);
          var tmp$7;
          if (match$8) {
            switch (match$8[0]) {
              case "Delete" : 
                  tmp$7 = /* Delete */527250507;
                  break;
              case "Disable" : 
                  tmp$7 = /* Disable */-22441528;
                  break;
              default:
                throw Graphql_error;
            }
          } else {
            throw Graphql_error;
          }
          tmp$6 = {
            actionTaken: tmp$7
          };
        } else {
          throw Graphql_error;
        }
        tmp$5 = /* Some */[tmp$6];
      }
      tmp = {
        error: tmp$1,
        payload: tmp$5
      };
    } else {
      throw Graphql_error;
    }
    return {
            deleteAlias: tmp
          };
  } else {
    throw Graphql_error;
  }
}

function json_of_optional(encoder, value) {
  if (value) {
    return Curry._1(encoder, value[0]);
  } else {
    return null;
  }
}

function json_of_Boolean(value) {
  return value;
}

function json_of_String(value) {
  return value;
}

var json_of_array = $$Array.map;

function json_of_DeleteAliasInput(value) {
  return Js_dict.fromList(/* :: */[
              /* tuple */[
                "clientMutationId",
                value.clientMutationId
              ],
              /* :: */[
                /* tuple */[
                  "disableIfUsed",
                  json_of_optional(json_of_Boolean, value.disableIfUsed)
                ],
                /* :: */[
                  /* tuple */[
                    "name",
                    value.name
                  ],
                  /* [] */0
                ]
              ]
            ]);
}

function make(input, _) {
  return {
          query: query,
          variables: Js_dict.fromList(/* :: */[
                /* tuple */[
                  "input",
                  json_of_DeleteAliasInput(input)
                ],
                /* [] */0
              ]),
          parse: parse
        };
}

var Mutation = /* module */[
  /* Graphql_error */Graphql_error,
  /* query */query,
  /* parse */parse,
  /* json_of_optional */json_of_optional,
  /* json_of_array */json_of_array,
  /* json_of_String */json_of_String,
  /* json_of_DeleteAliasInput */json_of_DeleteAliasInput,
  /* json_of_Boolean */json_of_Boolean,
  /* make */make
];

function deconstructResponse(response) {
  return /* tuple */[
          response.deleteAlias.payload,
          response.deleteAlias.error
        ];
}

var Config = /* module */[/* deconstructResponse */deconstructResponse];

var Request = Apollo$ReactTemplate.Request(Config);

function run(name) {
  var now = Pervasives.string_of_float(Date.now());
  var input = {
    clientMutationId: "deleteAlias///" + (name + ("///" + now)),
    name: name,
    disableIfUsed: /* Some */[true]
  };
  return Curry._1(Request[/* send */2], /* `Mutation */[
              1035765577,
              make(input, /* () */0)
            ]);
}

exports.Mutation = Mutation;
exports.Config   = Config;
exports.Request  = Request;
exports.run      = run;
/* Request Not a pure module */
