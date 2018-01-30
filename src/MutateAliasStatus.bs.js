// Generated by BUCKLESCRIPT VERSION 2.1.0, PLEASE EDIT WITH CARE
'use strict';

var $$Array              = require("bs-platform/lib/js/array.js");
var Curry                = require("bs-platform/lib/js/curry.js");
var Js_dict              = require("bs-platform/lib/js/js_dict.js");
var Js_json              = require("bs-platform/lib/js/js_json.js");
var Pervasives           = require("bs-platform/lib/js/pervasives.js");
var Caml_exceptions      = require("bs-platform/lib/js/caml_exceptions.js");
var Apollo$ReactTemplate = require("./apollo.bs.js");

var Graphql_error = Caml_exceptions.create("MutateAliasStatus-ReactTemplate.DisableMutation.Graphql_error");

var query = "mutation DisableAlias($input: DisableAliasInput!)  {\ndisableAlias(input: $input)  {\nerror  {\ncode  \nmessage  \n}\npayload  {\nclientMutationId  \n}\n}\n}";

function parse(value) {
  var match = Js_json.decodeObject(value);
  if (match) {
    var value$1 = match[0]["disableAlias"];
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
          var value$8 = match$7[0]["clientMutationId"];
          var match$8 = Js_json.decodeString(value$8);
          var tmp$7;
          if (match$8) {
            tmp$7 = match$8[0];
          } else {
            throw Graphql_error;
          }
          tmp$6 = {
            clientMutationId: tmp$7
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
            disableAlias: tmp
          };
  } else {
    throw Graphql_error;
  }
}

function json_of_String(value) {
  return value;
}

function json_of_optional(encoder, value) {
  if (value) {
    return Curry._1(encoder, value[0]);
  } else {
    return null;
  }
}

var json_of_array = $$Array.map;

function json_of_DisableAliasInput(value) {
  return Js_dict.fromList(/* :: */[
              /* tuple */[
                "clientMutationId",
                value.clientMutationId
              ],
              /* :: */[
                /* tuple */[
                  "name",
                  value.name
                ],
                /* [] */0
              ]
            ]);
}

function make(input, _) {
  return {
          query: query,
          variables: Js_dict.fromList(/* :: */[
                /* tuple */[
                  "input",
                  json_of_DisableAliasInput(input)
                ],
                /* [] */0
              ]),
          parse: parse
        };
}

var DisableMutation = /* module */[
  /* Graphql_error */Graphql_error,
  /* query */query,
  /* parse */parse,
  /* json_of_optional */json_of_optional,
  /* json_of_array */json_of_array,
  /* json_of_String */json_of_String,
  /* json_of_DisableAliasInput */json_of_DisableAliasInput,
  /* make */make
];

var Graphql_error$1 = Caml_exceptions.create("MutateAliasStatus-ReactTemplate.EnableMutation.Graphql_error");

var query$1 = "mutation EnableAlias($input: EnableAliasInput!)  {\nenableAlias(input: $input)  {\nerror  {\ncode  \nmessage  \n}\npayload  {\nclientMutationId  \n}\n}\n}";

function parse$1(value) {
  var match = Js_json.decodeObject(value);
  if (match) {
    var value$1 = match[0]["enableAlias"];
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
                throw Graphql_error$1;
            }
          } else {
            throw Graphql_error$1;
          }
          var value$6 = value$4["message"];
          var match$5 = Js_json.decodeString(value$6);
          var tmp$4;
          if (match$5) {
            tmp$4 = match$5[0];
          } else {
            throw Graphql_error$1;
          }
          tmp$2 = {
            code: tmp$3,
            message: tmp$4
          };
        } else {
          throw Graphql_error$1;
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
          var value$8 = match$7[0]["clientMutationId"];
          var match$8 = Js_json.decodeString(value$8);
          var tmp$7;
          if (match$8) {
            tmp$7 = match$8[0];
          } else {
            throw Graphql_error$1;
          }
          tmp$6 = {
            clientMutationId: tmp$7
          };
        } else {
          throw Graphql_error$1;
        }
        tmp$5 = /* Some */[tmp$6];
      }
      tmp = {
        error: tmp$1,
        payload: tmp$5
      };
    } else {
      throw Graphql_error$1;
    }
    return {
            enableAlias: tmp
          };
  } else {
    throw Graphql_error$1;
  }
}

function json_of_String$1(value) {
  return value;
}

function json_of_optional$1(encoder, value) {
  if (value) {
    return Curry._1(encoder, value[0]);
  } else {
    return null;
  }
}

var json_of_array$1 = $$Array.map;

function json_of_EnableAliasInput(value) {
  return Js_dict.fromList(/* :: */[
              /* tuple */[
                "clientMutationId",
                value.clientMutationId
              ],
              /* :: */[
                /* tuple */[
                  "name",
                  value.name
                ],
                /* [] */0
              ]
            ]);
}

function make$1(input, _) {
  return {
          query: query$1,
          variables: Js_dict.fromList(/* :: */[
                /* tuple */[
                  "input",
                  json_of_EnableAliasInput(input)
                ],
                /* [] */0
              ]),
          parse: parse$1
        };
}

var EnableMutation = /* module */[
  /* Graphql_error */Graphql_error$1,
  /* query */query$1,
  /* parse */parse$1,
  /* json_of_optional */json_of_optional$1,
  /* json_of_array */json_of_array$1,
  /* json_of_String */json_of_String$1,
  /* json_of_EnableAliasInput */json_of_EnableAliasInput,
  /* make */make$1
];

var Config = /* module */[];

function deconstructResponse(response) {
  return /* tuple */[
          response.disableAlias.payload,
          response.disableAlias.error
        ];
}

var DisableConfig = /* module */[/* deconstructResponse */deconstructResponse];

function deconstructResponse$1(response) {
  return /* tuple */[
          response.enableAlias.payload,
          response.enableAlias.error
        ];
}

var EnableConfig = /* module */[/* deconstructResponse */deconstructResponse$1];

var DisableRequest = Apollo$ReactTemplate.Request(DisableConfig);

var EnableRequest = Apollo$ReactTemplate.Request(EnableConfig);

function run(name, action) {
  var now = Pervasives.string_of_float(Date.now());
  var actionStr = action >= 756818595 ? "enable" : "disable";
  var input = {
    clientMutationId: name + ("///" + (actionStr + ("///" + now))),
    name: name
  };
  if (action >= 756818595) {
    return Curry._1(EnableRequest[/* send */2], /* `Mutation */[
                1035765577,
                make$1(input, /* () */0)
              ]);
  } else {
    return Curry._1(DisableRequest[/* send */2], /* `Mutation */[
                1035765577,
                make(input, /* () */0)
              ]);
  }
}

exports.DisableMutation = DisableMutation;
exports.EnableMutation  = EnableMutation;
exports.Config          = Config;
exports.DisableConfig   = DisableConfig;
exports.EnableConfig    = EnableConfig;
exports.DisableRequest  = DisableRequest;
exports.EnableRequest   = EnableRequest;
exports.run             = run;
/* DisableRequest Not a pure module */
