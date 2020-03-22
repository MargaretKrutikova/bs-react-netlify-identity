type provider = NetlifyProvider.t;
let providerToString: NetlifyProvider.t => string;

type maybeUserPromise('a) = Js.Promise.t(option(NetlifyUser.t('a)));
type settings = NetlifySettings.t;
type user('a) = NetlifyUser.t('a);

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

module IdentityContextProvider: {
  [@bs.obj]
  external makeProps:
    (~url: string, ~children: React.element, ~key: string=?, unit) =>
    {
      .
      "children": React.element,
      "url": string,
    } =
    "";
  [@bs.module "react-netlify-identity"]
  external make:
    React.componentLike(
      {
        .
        "children": React.element,
        "url": string,
      },
      React.element,
    ) =
    "IdentityContextProvider";
};

let useIdentityContextSimple: unit => reactNetlifyIdentityApi(Js.t({..}));

module type Config = {
  type userDataJs;
  type userData;
  let convertToJs: userData => userDataJs;
  let convertFromJs: userDataJs => userData;
};
module Make:
  (Config: Config) =>
   {let useIdentityContext: unit => reactNetlifyIdentityApi(Config.userData);};
