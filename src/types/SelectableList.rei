type t('a);

let empty: unit => t('a);
let create: list('a) => t('a);
let createAndSelect: list('a) => t('a);

let add: ('a, t('a)) => t('a);
let remove: ('a, t('a)) => t('a);
let replace: ('a, 'a, t('a)) => t('a);
let select: ('a, t('a)) => t('a);
let deselect: t('a) => t('a);
let all: t('a) => list('a);
let selected: t('a) => option('a);