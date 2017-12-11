// Generated by BUCKLESCRIPT VERSION 2.1.0, PLEASE EDIT WITH CARE
'use strict';

var List              = require("bs-platform/lib/js/list.js");
var $$Array           = require("bs-platform/lib/js/array.js");
var Curry             = require("bs-platform/lib/js/curry.js");
var $$String          = require("bs-platform/lib/js/string.js");
var Caml_format       = require("bs-platform/lib/js/caml_format.js");
var Caml_string       = require("bs-platform/lib/js/caml_string.js");
var Caml_exceptions   = require("bs-platform/lib/js/caml_exceptions.js");
var Js_null_undefined = require("bs-platform/lib/js/js_null_undefined.js");
var Opt$ReactTemplate = require("./Opt.bs.js");

var UnsupportedScheme = Caml_exceptions.create("Url-ReactTemplate.Scheme.UnsupportedScheme");

function ofString(s) {
  var match = $$String.lowercase(s);
  switch (match) {
    case "http" : 
    case "http:" : 
        return /* HTTP */802644872;
    case "https" : 
    case "https:" : 
        return /* HTTPS */748663755;
    default:
      throw [
            UnsupportedScheme,
            s
          ];
  }
}

function ofStringOpt(s) {
  try {
    return /* Some */[ofString(s)];
  }
  catch (exn){
    return /* None */0;
  }
}

function toString(param) {
  if (param >= 802644872) {
    return "http";
  } else {
    return "https";
  }
}

var Scheme = /* module */[
  /* UnsupportedScheme */UnsupportedScheme,
  /* ofString */ofString,
  /* ofStringOpt */ofStringOpt,
  /* toString */toString
];

function ofString$1(s) {
  return s;
}

function toString$1(t) {
  var match = +(t.length > 0 && Caml_string.get(t, 0) !== /* "/" */47);
  if (match !== 0) {
    return "/" + t;
  } else {
    return t;
  }
}

var Path = /* module */[
  /* ofString */ofString$1,
  /* toString */toString$1
];

function ofList(l) {
  return l;
}

function toList(t) {
  return t;
}

function ofString$2(s) {
  var s$1 = s.replace((/^\?/), "");
  if (s$1 === "") {
    return /* [] */0;
  } else {
    return List.map((function (param) {
                  if (param) {
                    var value = param[1];
                    var key = param[0];
                    if (value) {
                      if (value[1]) {
                        return /* record */[
                                /* key */decodeURIComponent(key),
                                /* value : Some */[decodeURIComponent($$String.concat("=", value))]
                              ];
                      } else {
                        return /* record */[
                                /* key */decodeURIComponent(key),
                                /* value : Some */[decodeURIComponent(value[0])]
                              ];
                      }
                    } else {
                      return /* record */[
                              /* key */decodeURIComponent(key),
                              /* value : None */0
                            ];
                    }
                  } else {
                    return /* record */[
                            /* key */"",
                            /* value : None */0
                          ];
                  }
                }), List.map($$Array.to_list, List.map((function (param) {
                          return param.split("=");
                        }), $$Array.to_list(s$1.split("&")))));
  }
}

function toString$2(t) {
  var pairUp = function (param) {
    var value = param[/* value */1];
    var value$prime = value ? "=" + encodeURIComponent(value[0]) : "";
    return encodeURIComponent(param[/* key */0]) + value$prime;
  };
  return $$String.concat("&", List.map(pairUp, t));
}

var Params = /* module */[
  /* ofList */ofList,
  /* toList */toList,
  /* ofString */ofString$2,
  /* toString */toString$2
];

function make(id, scheme, user, pass, host, port, $staropt$star, query, fragment, _) {
  var path = $staropt$star ? $staropt$star[0] : "";
  return /* record */[
          /* id */id,
          /* scheme */scheme,
          /* user */user,
          /* pass */pass,
          /* host */host,
          /* path */path,
          /* port */port,
          /* params */Opt$ReactTemplate.map(ofString$2, query),
          /* fragment */fragment
        ];
}

var MissingPart = Caml_exceptions.create("Url-ReactTemplate.MissingPart");

function ofString$3(s) {
  var a = (document.createElement('a'));
  (( a.href = s ));
  var prop = function (name) {
    var get = (
      function (key, obj) { return obj[key] }
      );
    return Curry._2(get, name, a);
  };
  var propOpt = function (name) {
    var s = prop(name);
    if (s === "") {
      return /* None */0;
    } else {
      return /* Some */[s];
    }
  };
  var propExn = function (name) {
    var match = propOpt(name);
    if (match) {
      return match[0];
    } else {
      throw [
            MissingPart,
            name,
            s
          ];
    }
  };
  return make(/* None */0, ofString(prop("protocol")), propOpt("username"), propOpt("password"), propExn("hostname"), Opt$ReactTemplate.map(Caml_format.caml_int_of_string, propOpt("port")), /* Some */[prop("pathname")], propOpt("search"), propOpt("hash"), /* () */0);
}

function ofStringOpt$1(s) {
  try {
    return /* Some */[ofString$3(s)];
  }
  catch (exn){
    return /* None */0;
  }
}

function toGql(t) {
  var paramsToGql = function (p) {
    return $$Array.of_list(List.map((function (p) {
                      return {
                              key: p[/* key */0],
                              value: Js_null_undefined.from_opt(p[/* value */1])
                            };
                    }), p));
  };
  return {
          scheme: $$String.uppercase(toString(t[/* scheme */1])),
          user: Js_null_undefined.from_opt(t[/* user */2]),
          password: Js_null_undefined.from_opt(t[/* pass */3]),
          host: t[/* host */4],
          port: Js_null_undefined.from_opt(t[/* port */6]),
          path: toString$1(t[/* path */5]),
          params: Js_null_undefined.from_opt(Opt$ReactTemplate.map(paramsToGql, t[/* params */7])),
          fragment: Js_null_undefined.from_opt(t[/* fragment */8])
        };
}

exports.Scheme      = Scheme;
exports.Path        = Path;
exports.Params      = Params;
exports.make        = make;
exports.MissingPart = MissingPart;
exports.ofString    = ofString$3;
exports.ofStringOpt = ofStringOpt$1;
exports.toGql       = toGql;
/* No side effect */
