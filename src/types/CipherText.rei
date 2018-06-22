type t;

let create: ArrayBuffer.t => t;

let toArrayBuffer: t => ArrayBuffer.t;
let fromString: string => t;
let toString: t => string;