type provider =
  | Bitbucket
  | GitHub
  | GitLab
  | Google;

let providerToString: provider => string;

type settings = {
  autoconfirm: bool,
  disableSignup: bool,
  providers: array(provider),
};

type appMetaData = {
  provider: string,
  roles: option(array(string)),
};

type user('a) = {
  appMetaData: option(appMetaData),
  metaData: option('a),
  id: string,
  email: string,
  role: string,
};

type reactNetlifyIdentityApi('a) = {
  user: option(user('a)),
  isLoggedIn: bool,
  signupUser:
    (~email: string, ~password: string, ~data: 'a) =>
    Js.Promise.t(option(user('a))),
  loginUser:
    (~email: string, ~password: string, ~remember: bool=?, unit) =>
    Js.Promise.t(option(user('a))),
  logoutUser: unit => Js.Promise.t(option(user('a))),
  requestPasswordRecovery: (~email: string) => Js.Promise.t(unit),
  recoverAccount:
    (~token: string, ~remember: bool=?, unit) => Js.Promise.t(user('a)),
  updateUser: Js.Json.t => Js.Promise.t(option(user('a))),
  getFreshJWT: unit => Js.Promise.t(string),
  url: string,
  loginProvider: provider => unit,
  acceptInviteExternalUrl: (~provider: provider, ~token: string) => string,
  settings,
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
