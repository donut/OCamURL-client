// Generated by BUCKLESCRIPT VERSION 2.1.0, PLEASE EDIT WITH CARE
'use strict';

var List                 = require("bs-platform/lib/js/list.js");
var $$Array              = require("bs-platform/lib/js/array.js");
var Curry                = require("bs-platform/lib/js/curry.js");
var Js_dict              = require("bs-platform/lib/js/js_dict.js");
var Js_json              = require("bs-platform/lib/js/js_json.js");
var Caml_exceptions      = require("bs-platform/lib/js/caml_exceptions.js");
var Url$ReactTemplate    = require("./url.bs.js");
var Alias$ReactTemplate  = require("./alias.bs.js");
var Store$ReactTemplate  = require("./Store/Store.bs.js");
var Action$ReactTemplate = require("./Store/Action.bs.js");
var Apollo$ReactTemplate = require("./apollo.bs.js");

function str(prim) {
  return prim;
}

var Graphql_error = Caml_exceptions.create("QueryAliases-ReactTemplate.Query.Graphql_error");

var query = "query GetAliasesByURL($url: AliasesURLInput)  {\naliases(url: $url)  {\nerror  {\ncode  \nmessage  \n}\naliases  {\nid  \nname  \nstatus  \n}\n}\n}";

function parse(value) {
  var match = Js_json.decodeObject(value);
  if (match) {
    var value$1 = match[0]["aliases"];
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
      var value$7 = value$2["aliases"];
      var match$6 = Js_json.decodeNull(value$7);
      var tmp$5;
      if (match$6) {
        tmp$5 = /* None */0;
      } else {
        var match$7 = Js_json.decodeArray(value$7);
        var tmp$6;
        if (match$7) {
          tmp$6 = $$Array.map((function (value) {
                  var match = Js_json.decodeObject(value);
                  if (match) {
                    var value$1 = match[0];
                    var value$2 = value$1["id"];
                    var match$1 = Js_json.decodeString(value$2);
                    var tmp;
                    if (match$1) {
                      tmp = match$1[0];
                    } else {
                      throw Graphql_error;
                    }
                    var value$3 = value$1["name"];
                    var match$2 = Js_json.decodeString(value$3);
                    var tmp$1;
                    if (match$2) {
                      tmp$1 = match$2[0];
                    } else {
                      throw Graphql_error;
                    }
                    var value$4 = value$1["status"];
                    var match$3 = Js_json.decodeString(value$4);
                    var tmp$2;
                    if (match$3) {
                      switch (match$3[0]) {
                        case "Disabled" : 
                            tmp$2 = /* Disabled */-709493348;
                            break;
                        case "Enabled" : 
                            tmp$2 = /* Enabled */-880661407;
                            break;
                        default:
                          throw Graphql_error;
                      }
                    } else {
                      throw Graphql_error;
                    }
                    return {
                            id: tmp,
                            name: tmp$1,
                            status: tmp$2
                          };
                  } else {
                    throw Graphql_error;
                  }
                }), match$7[0]);
        } else {
          throw Graphql_error;
        }
        tmp$5 = /* Some */[tmp$6];
      }
      tmp = {
        error: tmp$1,
        aliases: tmp$5
      };
    } else {
      throw Graphql_error;
    }
    return {
            aliases: tmp
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

function json_of_String(value) {
  return value;
}

function json_of_URLSchemeInput(value) {
  if (value >= 802644872) {
    return "HTTP";
  } else {
    return "HTTPS";
  }
}

function json_of_URLParamInput(value) {
  return Js_dict.fromList(/* :: */[
              /* tuple */[
                "value",
                json_of_optional(json_of_String, value.value)
              ],
              /* :: */[
                /* tuple */[
                  "key",
                  value.key
                ],
                /* [] */0
              ]
            ]);
}

function json_of_Int(value) {
  return value;
}

var json_of_array = $$Array.map;

function json_of_AliasesURLInput(value) {
  return Js_dict.fromList(/* :: */[
              /* tuple */[
                "fragment",
                json_of_optional(json_of_String, value.fragment)
              ],
              /* :: */[
                /* tuple */[
                  "params",
                  json_of_optional((function (param) {
                          return $$Array.map(json_of_URLParamInput, param);
                        }), value.params)
                ],
                /* :: */[
                  /* tuple */[
                    "path",
                    json_of_optional(json_of_String, value.path)
                  ],
                  /* :: */[
                    /* tuple */[
                      "port",
                      json_of_optional(json_of_Int, value.port)
                    ],
                    /* :: */[
                      /* tuple */[
                        "host",
                        value.host
                      ],
                      /* :: */[
                        /* tuple */[
                          "password",
                          json_of_optional(json_of_String, value.password)
                        ],
                        /* :: */[
                          /* tuple */[
                            "user",
                            json_of_optional(json_of_String, value.user)
                          ],
                          /* :: */[
                            /* tuple */[
                              "scheme",
                              json_of_URLSchemeInput(value.scheme)
                            ],
                            /* [] */0
                          ]
                        ]
                      ]
                    ]
                  ]
                ]
              ]
            ]);
}

function make(url, _) {
  return {
          query: query,
          variables: Js_dict.fromList(/* :: */[
                /* tuple */[
                  "url",
                  json_of_optional(json_of_AliasesURLInput, url)
                ],
                /* [] */0
              ]),
          parse: parse
        };
}

var Query = /* module */[
  /* Graphql_error */Graphql_error,
  /* query */query,
  /* parse */parse,
  /* json_of_optional */json_of_optional,
  /* json_of_array */json_of_array,
  /* json_of_URLSchemeInput */json_of_URLSchemeInput,
  /* json_of_URLParamInput */json_of_URLParamInput,
  /* json_of_String */json_of_String,
  /* json_of_Int */json_of_Int,
  /* json_of_AliasesURLInput */json_of_AliasesURLInput,
  /* make */make
];

function deconstructResponse(response) {
  return /* tuple */[
          response.aliases.aliases,
          response.aliases.error
        ];
}

var Config = /* module */[/* deconstructResponse */deconstructResponse];

var Request = Apollo$ReactTemplate.Request(Config);

function run(url) {
  Store$ReactTemplate.dispatch(Action$ReactTemplate.AliasListLoading);
  Curry._1(Request[/* send */2], /* `Query */[
          -250086680,
          make(/* Some */[Url$ReactTemplate.toGql(url)], /* () */0)
        ]).then((function (result) {
          if (result[0] >= 981919598) {
            var lst = List.map(Alias$ReactTemplate.ofGql, $$Array.to_list(result[1]));
            Store$ReactTemplate.dispatch([
                  Action$ReactTemplate.AliasListLoaded,
                  lst
                ]);
          } else {
            var message = Apollo$ReactTemplate.messageOfExn("getting alias list", url, result[1]);
            Store$ReactTemplate.dispatch([
                  Action$ReactTemplate.AliasListLoadingFailed,
                  message
                ]);
          }
          return Promise.resolve(/* () */0);
        }));
  return /* () */0;
}

function reload(url) {
  Store$ReactTemplate.dispatch(Action$ReactTemplate.AliasListLoading);
  Apollo$ReactTemplate.resetStore(/* () */0).then((function () {
            run(url);
            return Promise.resolve(/* () */0);
          })).catch((function (error) {
          var message = "Failed reseting store before reloading alias list.";
          console.log(message, url, error);
          Store$ReactTemplate.dispatch([
                Action$ReactTemplate.AliasListLoadingFailed,
                message
              ]);
          return Promise.resolve(/* () */0);
        }));
  return /* () */0;
}

exports.str     = str;
exports.Query   = Query;
exports.Config  = Config;
exports.Request = Request;
exports.run     = run;
exports.reload  = reload;
/* Request Not a pure module */
