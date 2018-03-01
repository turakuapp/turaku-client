type domElement = {. "value": string};

type domDocument = {. [@bs.meth] "getElementById": string => domElement};

[@bs.val] external document : domDocument = "document";

[@bs.send]
external preventEventDefault : ReactEventRe.Form.t => unit = "preventDefault";