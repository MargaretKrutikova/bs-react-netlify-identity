type tokenType =
  | Invite
  | Recovery
  | EmailChange
  | Unknown;

let tokenTypeFromJs =
  fun
  | "invite" => Invite
  | "recovery" => Recovery
  | "email_change" => EmailChange
  | _ => Unknown;

type tokenJs = {
  token: Js.Nullable.t(string),
  [@bs.as "type"]
  type_: Js.Nullable.t(string),
  error: Js.Nullable.t(string),
  status: Js.Nullable.t(int),
};

type t = {
  token: option(string),
  type_: tokenType,
  error: option(string),
  status: option(int),
};

let fromJs = (tokenJs: tokenJs) => {
  token: Js.Nullable.toOption(tokenJs.token),
  error: Js.Nullable.toOption(tokenJs.error),
  status: Js.Nullable.toOption(tokenJs.status),
  type_:
    tokenJs.type_
    ->Js.Nullable.toOption
    ->Belt.Option.getWithDefault("")
    ->tokenTypeFromJs,
};
