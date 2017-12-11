// Generated by BUCKLESCRIPT VERSION 2.1.0, PLEASE EDIT WITH CARE
'use strict';

var $$Array              = require("bs-platform/lib/js/array.js");
var Curry                = require("bs-platform/lib/js/curry.js");
var React                = require("react");
var ReasonReact          = require("reason-react/src/ReasonReact.js");
var GraphqlTag           = require("graphql-tag");
var Url$ReactTemplate    = require("./url.bs.js");
var JsOpt$ReactTemplate  = require("./JsOpt.bs.js");
var Apollo$ReactTemplate = require("./apollo.bs.js");

function str(prim) {
  return prim;
}

var query = GraphqlTag("\n  query GetAliasesByURL($url: AliasesURLInput) {\n    aliases(url: $url) {\n      error { code, message }\n      aliases { id, name, status }\n    }\n  }\n");

var Config = /* module */[];

var FetchAliases = Curry._1(Apollo$ReactTemplate.Client[/* Query */3], Config);

var component = ReasonReact.statelessComponent("QueryAliases");

function make(url, _) {
  var newrecord = component.slice();
  newrecord[/* render */9] = (function () {
      var variables = {
        url: Url$ReactTemplate.toGql(url)
      };
      return ReasonReact.element(/* None */0, /* None */0, Curry._3(FetchAliases[/* make */4], query, /* Some */[variables], /* array */[(function (param) {
                          if (typeof param === "number") {
                            return React.createElement("div", undefined, "Loading aliases.");
                          } else if (param.tag) {
                            return React.createElement("div", undefined, "Failed loading aliases: " + param[0]);
                          } else {
                            var payload = param[0].aliases;
                            if (JsOpt$ReactTemplate.notNull(payload.error)) {
                              var error = JsOpt$ReactTemplate.value(payload.error);
                              var className = "error-" + error.code;
                              return React.createElement("div", {
                                          className: className
                                        }, "Failed loading aliases: " + error.message);
                            } else if (JsOpt$ReactTemplate.isNull(payload.aliases)) {
                              return React.createElement("div", undefined, "An unknown error occurred.");
                            } else {
                              return $$Array.map((function (alias) {
                                            return React.createElement("div", {
                                                        key: alias.id
                                                      }, alias.name + (" : " + alias.status));
                                          }), JsOpt$ReactTemplate.value(payload.aliases));
                            }
                          }
                        })]));
    });
  return newrecord;
}

exports.str          = str;
exports.query        = query;
exports.Config       = Config;
exports.FetchAliases = FetchAliases;
exports.component    = component;
exports.make         = make;
/* query Not a pure module */
