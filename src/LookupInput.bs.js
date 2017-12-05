// Generated by BUCKLESCRIPT VERSION 2.1.0, PLEASE EDIT WITH CARE
'use strict';

var Block       = require("bs-platform/lib/js/block.js");
var Curry       = require("bs-platform/lib/js/curry.js");
var React       = require("react");
var $$String    = require("bs-platform/lib/js/string.js");
var ReasonReact = require("reason-react/src/ReasonReact.js");

function str(prim) {
  return prim;
}

var component = ReasonReact.reducerComponent("LookupInput");

function make(initialValue, onSubmit, _) {
  var handleSubmit = function (state) {
    var text = $$String.trim(state[/* value */0]);
    return /* UpdateWithSideEffects */Block.__(3, [
              /* record */[/* value */text],
              (function () {
                  return Curry._1(onSubmit, text);
                })
            ]);
  };
  var change = function ($$event) {
    var el = $$event.target;
    return /* Change */Block.__(0, [el.value]);
  };
  var keyDown = function ($$event) {
    return /* KeyDown */Block.__(1, [$$event.key]);
  };
  var submit = function () {
    return /* Submit */0;
  };
  var newrecord = component.slice();
  newrecord[/* render */9] = (function (param) {
      var reduce = param[/* reduce */1];
      return React.createElement("div", {
                  className: "lookup"
                }, React.createElement("input", {
                      placeholder: "Paste a URL",
                      type: "url",
                      value: param[/* state */2][/* value */0],
                      onKeyDown: Curry._1(reduce, keyDown),
                      onBlur: Curry._1(reduce, submit),
                      onChange: Curry._1(reduce, change)
                    }));
    });
  newrecord[/* initialState */10] = (function () {
      return /* record */[/* value */initialValue];
    });
  newrecord[/* reducer */12] = (function (action, state) {
      if (typeof action === "number") {
        return handleSubmit(state);
      } else if (action.tag) {
        if (action[0] === "Enter") {
          return handleSubmit(state);
        } else {
          return /* NoUpdate */0;
        }
      } else {
        return /* Update */Block.__(0, [/* record */[/* value */action[0]]]);
      }
    });
  return newrecord;
}

exports.str       = str;
exports.component = component;
exports.make      = make;
/* component Not a pure module */