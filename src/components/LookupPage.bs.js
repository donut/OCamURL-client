// Generated by BUCKLESCRIPT VERSION 2.1.0, PLEASE EDIT WITH CARE
'use strict';

var List                              = require("bs-platform/lib/js/list.js");
var $$Array                           = require("bs-platform/lib/js/array.js");
var React                             = require("react");
var $$String                          = require("bs-platform/lib/js/string.js");
var ReasonReact                       = require("reason-react/src/ReasonReact.js");
var State$ReactTemplate               = require("../store/State.bs.js");
var Store$ReactTemplate               = require("../store/Store.bs.js");
var Action$ReactTemplate              = require("../store/Action.bs.js");
var Config$ReactTemplate              = require("../Config.bs.js");
var ListAliases$ReactTemplate         = require("./ListAliases.bs.js");
var LookupInput$ReactTemplate         = require("./LookupInput.bs.js");
var GenerateAliasButton$ReactTemplate = require("./GenerateAliasButton.bs.js");

function str(prim) {
  return prim;
}

var component = ReasonReact.statelessComponent("LookupPage");

function make(state, _, _$1) {
  var newrecord = component.slice();
  newrecord[/* render */9] = (function () {
      var match = state[/* lookupURL */3];
      var details;
      if (typeof match === "number") {
        details = null;
      } else if (match[0] >= 144245463) {
        details = React.createElement("p", {
              className: "error invalid-input"
            }, "That doesn\'t look like a valid URL: " + (String(match[1]) + ""));
      } else {
        var url = match[1];
        details = /* array */[
          React.createElement("div", {
                key: "generateAliasButton",
                className: "button-add-alias"
              }, ReasonReact.element(/* None */0, /* None */0, GenerateAliasButton$ReactTemplate.make(url, state, /* array */[]))),
          ReasonReact.element(/* Some */["listAliases"], /* None */0, ListAliases$ReactTemplate.make(url, state[/* aliasList */1], /* array */[]))
        ];
      }
      var messages = $$Array.of_list(List.map((function (param) {
                  return React.createElement("p", {
                              className: "message " + State$ReactTemplate.stringOfMessageType(param[0])
                            }, param[1]);
                }), state[/* messages */4]));
      var bookmarklet = $$String.concat("", /* :: */[
            "javascript:window.location=\"",
            /* :: */[
              window.location.protocol,
              /* :: */[
                "//",
                /* :: */[
                  window.location.host,
                  /* :: */[
                    window.location.pathname,
                    /* :: */[
                      "?url=\"+encodeURIComponent(window.location.href)",
                      /* [] */0
                    ]
                  ]
                ]
              ]
            ]
          ]);
      return React.createElement("div", {
                  className: "doc"
                }, React.createElement("header", undefined, React.createElement("h1", undefined, Config$ReactTemplate.title), React.createElement("a", {
                          className: "bookmarklet",
                          href: bookmarklet
                        }, "Bookmarklet")), React.createElement("section", {
                      className: "lookup-page"
                    }, React.createElement("section", {
                          className: "messages"
                        }, messages), ReasonReact.element(/* None */0, /* None */0, LookupInput$ReactTemplate.make(state[/* lookupInitialValue */2], (function (u) {
                                return Store$ReactTemplate.dispatch([
                                            Action$ReactTemplate.SetLookupURL,
                                            u
                                          ]);
                              }), /* array */[])), details));
    });
  return newrecord;
}

exports.str       = str;
exports.component = component;
exports.make      = make;
/* component Not a pure module */
