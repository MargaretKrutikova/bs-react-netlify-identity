external castToJson: 'a => Js.Json.t = "%identity";
external castFromJson: Js.Json.t => 'a = "%identity";

type provider =
  | Bitbucket
  | GitHub
  | GitLab
  | Google;

let providerToString = provider =>
  switch (provider) {
  | Bitbucket => "bitbucket"
  | GitHub => "github"
  | GitLab => "gitlab"
  | Google => "google"
  };

type externalSettingsJs = {
  .
  "bitbucket": bool,
  "email": bool,
  "facebook": bool,
  "github": bool,
  "gitlab": bool,
  "google": bool,
};

[@bs.deriving abstract]
type settingsJs = {
  [@bs.as "external"]
  external_: externalSettingsJs,
  autoconfirm: bool,
  disable_signup: bool,
};

type settings = {
  autoconfirm: bool,
  disableSignup: bool,
  providers: array(provider),
};

let addProvider = (provider, isOn, providers) =>
  if (isOn) {
    Belt.Array.concat(providers, [|provider|]);
  } else {
    providers;
  };

let fromSettingsJs = settings => {
  let jsProviders = external_Get(settings);
  let providers =
    [||]
    |> addProvider(Bitbucket, jsProviders##bitbucket)
    |> addProvider(GitHub, jsProviders##github)
    |> addProvider(GitLab, jsProviders##gitlab)
    |> addProvider(Google, jsProviders##google);

  {
    autoconfirm: autoconfirmGet(settings),
    disableSignup: disable_signupGet(settings),
    providers,
  };
};

type appMetaDataJs = {
  .
  "provider": string,
  "roles": Js.Nullable.t(array(string)),
};

type appMetaData = {
  provider: string, // TOOD: use provider variant
  roles: option(array(string)),
};

type userJs = {
  .
  "app_metadata": Js.Nullable.t(appMetaDataJs),
  "user_metadata": Js.Nullable.t(Js.Json.t),
  "id": string,
  "email": string,
  "role": string,
};

type user('a) = {
  appMetaData: option(appMetaData),
  metaData: option('a),
  id: string,
  email: string,
  role: string,
};

let fromUserJs = (user: userJs, convertMetaDataFromJs) => {
  id: user##id,
  email: user##email,
  role: user##role,
  appMetaData:
    user##app_metadata
    ->Js.Nullable.toOption
    ->Belt.Option.map(data =>
        {provider: data##provider, roles: data##roles->Js.Nullable.toOption}
      ),
  metaData:
    user##user_metadata
    ->Js.Nullable.toOption
    ->Belt.Option.map(data => convertMetaDataFromJs(data)),
};

type reactNetlifyIdentityApiJs = {
  .
  "user": Js.Nullable.t(userJs),
  "isConfirmedUser": bool,
  "isLoggedIn": bool,
  [@bs.meth]
  "signupUser":
    (string, string, Js.Json.t) => Js.Promise.t(Js.Nullable.t(userJs)),
  [@bs.meth]
  "loginUser":
    (string, string, Js.Nullable.t(bool)) =>
    Js.Promise.t(Js.Nullable.t(userJs)),
  [@bs.meth] "logoutUser": unit => Js.Promise.t(Js.Nullable.t(userJs)),
  [@bs.meth] "requestPasswordRecovery": string => Js.Promise.t(unit),
  [@bs.meth]
  "recoverAccount": (string, Js.Nullable.t(bool)) => Js.Promise.t(userJs),
  [@bs.meth]
  "updateUser":
    {. "data": Js.Json.t} => Js.Promise.t(Js.Nullable.t(userJs)),
  [@bs.meth] "getFreshJWT": unit => Js.Promise.t(string),
  "_url": string,
  [@bs.meth] "loginProvider": string => unit,
  [@bs.meth] "acceptInviteExternalUrl": (string, string) => string,
  "settings": settingsJs,
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

module IdentityContextProvider = {
  [@bs.module "react-netlify-identity"] [@react.component]
  external make: (~url: string, ~children: React.element) => React.element =
    "IdentityContextProvider";
};

[@bs.module "react-netlify-identity"]
external useIdentityContextJs: unit => reactNetlifyIdentityApiJs =
  "useIdentityContext";

let useIdentityContext =
    (convertFromJs, convertToJs): reactNetlifyIdentityApi('a) => {
  let identity = useIdentityContextJs();

  let convertUser = userJs => fromUserJs(userJs, convertFromJs);
  {
    user: identity##user->Js.Nullable.toOption->Belt.Option.map(convertUser),
    isLoggedIn: identity##isLoggedIn,
    signupUser: (~email, ~password, ~data) =>
      identity##signupUser(email, password, convertToJs(data))
      |> Js.Promise.then_(user =>
           user->Js.Nullable.toOption->Belt.Option.map(convertUser)
           |> Js.Promise.resolve
         ),
    loginUser: (~email, ~password, ~remember=?, ()) =>
      identity##loginUser(email, password, Js.Nullable.fromOption(remember))
      |> Js.Promise.then_(user =>
           user->Js.Nullable.toOption->Belt.Option.map(convertUser)
           |> Js.Promise.resolve
         ),
    logoutUser: () =>
      identity##logoutUser()
      |> Js.Promise.then_(user =>
           user->Js.Nullable.toOption->Belt.Option.map(convertUser)
           |> Js.Promise.resolve
         ),
    requestPasswordRecovery: (~email) =>
      identity##requestPasswordRecovery(email),
    recoverAccount: (~token, ~remember=?, ()) =>
      identity##recoverAccount(token, Js.Nullable.fromOption(remember))
      |> Js.Promise.then_(user => convertUser(user) |> Js.Promise.resolve),
    updateUser: data =>
      identity##updateUser({"data": data})
      |> Js.Promise.then_(user =>
           user->Js.Nullable.toOption->Belt.Option.map(convertUser)
           |> Js.Promise.resolve
         ),
    getFreshJWT: () => identity##getFreshJWT(),
    url: identity##_url,
    loginProvider: provider =>
      identity##loginProvider(providerToString(provider)),
    acceptInviteExternalUrl: (~provider, ~token) =>
      identity##acceptInviteExternalUrl(providerToString(provider), token),
    settings: fromSettingsJs(identity##settings),
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
