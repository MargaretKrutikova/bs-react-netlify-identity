external castToJson: 'a => Js.Json.t = "%identity";
external castFromJson: Js.Json.t => 'a = "%identity";

type provider = NetlifyProvider.t;
let providerToString = NetlifyProvider.toString;
type settings = NetlifySettings.t;
type user('a) = NetlifyUser.t('a);

type maybeUserPromiseJs = Js.Promise.t(Js.Nullable.t(NetlifyUser.userJs));
type maybeUserPromise('a) = Js.Promise.t(option(NetlifyUser.t('a)));

let toMaybeUserFromJs = (convertMetaDataFromJs, userJs) =>
  userJs
  ->Js.Nullable.toOption
  ->Belt.Option.map(NetlifyUser.fromJs(convertMetaDataFromJs));

let toMaybeUserPromise = (convertMetaDataFromJs, userPromiseJs) =>
  userPromiseJs
  |> Js.Promise.then_(user =>
       user |> toMaybeUserFromJs(convertMetaDataFromJs) |> Js.Promise.resolve
     );

type reactNetlifyIdentityApiJs = {
  user: Js.Nullable.t(NetlifyUser.userJs),
  isConfirmedUser: bool,
  isLoggedIn: bool,
  signupUser: (string, string, Js.Json.t) => maybeUserPromiseJs,
  loginUser: (string, string, Js.Nullable.t(bool)) => maybeUserPromiseJs,
  logoutUser: unit => maybeUserPromiseJs,
  requestPasswordRecovery: string => Js.Promise.t(unit),
  recoverAccount: Js.Nullable.t(bool) => maybeUserPromiseJs,
  updateUser: Js.Json.t => maybeUserPromiseJs,
  getFreshJWT: unit => Js.Promise.t(string),
  _url: string,
  loginProvider: string => unit,
  acceptInviteExternalUrl: string => string,
  settings: NetlifySettings.settingsJs,
  param: NetlifyToken.tokenJs,
};

type reactNetlifyIdentityApi('a) = {
  user: option(NetlifyUser.t('a)),
  isConfirmedUser: bool,
  isLoggedIn: bool,
  signupUser:
    (~email: string, ~password: string, ~data: 'a) => maybeUserPromise('a),
  loginUser:
    (~email: string, ~password: string, ~remember: bool=?, unit) =>
    maybeUserPromise('a),
  logoutUser: unit => maybeUserPromise('a),
  requestPasswordRecovery: (~email: string) => Js.Promise.t(unit),
  recoverAccount: (~remember: bool=?, unit) => maybeUserPromise('a),
  updateUser: Js.Json.t => maybeUserPromise('a),
  getFreshJWT: unit => Js.Promise.t(string),
  url: string,
  loginProvider: NetlifyProvider.t => unit,
  acceptInviteExternalUrl: (~token: string) => string,
  settings: NetlifySettings.t,
  param: NetlifyToken.t,
};

module IdentityContextProvider = {
  [@bs.module "react-netlify-identity"] [@react.component]
  external make:
    (~url: string, ~children: React.element, unit) => React.element =
    "IdentityContextProvider";
};

[@bs.module "react-netlify-identity"]
external useIdentityContextJs: unit => reactNetlifyIdentityApiJs =
  "useIdentityContext";

let useIdentityContext =
    (convertFromJs, convertToJs): reactNetlifyIdentityApi('a) => {
  let identity = useIdentityContextJs();

  {
    user: identity.user |> toMaybeUserFromJs(convertFromJs),
    isLoggedIn: identity.isLoggedIn,
    isConfirmedUser: identity.isConfirmedUser,
    signupUser: (~email, ~password, ~data) =>
      identity.signupUser(email, password, convertToJs(data))
      |> toMaybeUserPromise(convertFromJs),
    loginUser: (~email, ~password, ~remember=?, ()) =>
      identity.loginUser(email, password, Js.Nullable.fromOption(remember))
      |> toMaybeUserPromise(convertFromJs),
    logoutUser: () =>
      identity.logoutUser() |> toMaybeUserPromise(convertFromJs),
    requestPasswordRecovery: (~email) =>
      identity.requestPasswordRecovery(email),
    recoverAccount: (~remember=?, ()) =>
      identity.recoverAccount(Js.Nullable.fromOption(remember))
      |> toMaybeUserPromise(convertFromJs),
    updateUser: data =>
      identity.updateUser(data) |> toMaybeUserPromise(convertFromJs),
    getFreshJWT: () => identity.getFreshJWT(),
    url: identity._url,
    loginProvider: provider =>
      identity.loginProvider(provider |> NetlifyProvider.toString),
    acceptInviteExternalUrl: (~token) =>
      identity.acceptInviteExternalUrl(token),
    settings: NetlifySettings.fromJs(identity.settings),
    param: NetlifyToken.fromJs(identity.param),
  };
};

let useIdentityContextSimple = (): reactNetlifyIdentityApi(Js.t({..})) => {
  let identity =
    useIdentityContext(json => castFromJson(json), obj => castToJson(obj));
  identity;
};

module type Config = {
  type userDataJs;
  type userData;
  let convertToJs: userData => userDataJs;
  let convertFromJs: userDataJs => userData;
};

module Make = (Config: Config) => {
  let useIdentityContext = () => {
    let identity: reactNetlifyIdentityApi(Config.userData) =
      useIdentityContext(
        jsonObj => Config.convertFromJs(jsonObj |> castFromJson),
        obj => Config.convertToJs(obj) |> castToJson,
      );
    identity;
  };
};
