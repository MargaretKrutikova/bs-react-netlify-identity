external cast: Js.t({..}) => Js.Json.t = "%identity";
external castToObj: Js.Json.t => Js.t({..}) = "%identity";

type loginProvider =
  | Bitbucket
  | GitHub
  | GitLab
  | Google;

let providerToJs = provider =>
  switch (provider) {
  | Bitbucket => "bitbucket"
  | GitHub => "github"
  | GitLab => "gitlab"
  | Google => "google"
  };

type externalSettings = {
  .
  "bitbucket": bool,
  "email": bool,
  "facebook": bool,
  "github": bool,
  "gitlab": bool,
  "google": bool,
};

type settings = {
  .
  "autoconfirm": bool,
  "disable_signup": bool,
  "external": externalSettings,
};

type userJs = {
  .
  "user_metadata": Js.Nullable.t(Js.Json.t),
  "id": string,
  "email": string,
  "role": string,
};

type user('a) = {
  metaData: option(Js.t({..}) as 'a),
  id: string,
  email: string,
  role: string,
};

let fromJsUser = (user: userJs) => {
  id: user##id,
  email: user##email,
  role: user##role,
  metaData:
    user##user_metadata
    ->Js.Nullable.toOption
    ->Belt.Option.map(data => castToObj(data)),
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
  "settings": settings,
};

type reactNetlifyIdentityApi('a) = {
  user: option(user('a)),
  isLoggedIn: bool,
  signupUser:
    (~email: string, ~password: string, ~data: Js.t({..}) as 'a) =>
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
  loginProvider: loginProvider => unit,
  acceptInviteExternalUrl:
    (~provider: loginProvider, ~token: string) => string,
  settings,
};

module IdentityContextProvider = {
  [@bs.module "react-netlify-identity"] [@react.component]
  external make: (~url: string, ~children: React.element) => React.element =
    "IdentityContextProvider";
};

[@bs.module "react-netlify-identity"]
external useNetlifyIdentity: string => reactNetlifyIdentityApiJs =
  "useNetlifyIdentity";

[@bs.module "react-netlify-identity"]
external useIdentityContextJs: unit => reactNetlifyIdentityApiJs =
  "useIdentityContext";

let getUserMetadata = (user): Js.t({..} as 'a) =>
  castToObj(user##user_metadata);

let useIdentityContext = (): reactNetlifyIdentityApi('a) => {
  let identity = useIdentityContextJs();

  {
    user: identity##user->Js.Nullable.toOption->Belt.Option.map(fromJsUser),
    isLoggedIn: identity##isLoggedIn,
    signupUser: (~email, ~password, ~data) =>
      identity##signupUser(email, password, cast(data))
      |> Js.Promise.then_(user =>
           user->Js.Nullable.toOption->Belt.Option.map(fromJsUser)
           |> Js.Promise.resolve
         ),
    loginUser: (~email, ~password, ~remember=?, ()) =>
      identity##loginUser(email, password, Js.Nullable.fromOption(remember))
      |> Js.Promise.then_(user =>
           user->Js.Nullable.toOption->Belt.Option.map(fromJsUser)
           |> Js.Promise.resolve
         ),
    logoutUser: () =>
      identity##logoutUser()
      |> Js.Promise.then_(user =>
           user->Js.Nullable.toOption->Belt.Option.map(fromJsUser)
           |> Js.Promise.resolve
         ),
    requestPasswordRecovery: (~email) =>
      identity##requestPasswordRecovery(email),
    recoverAccount: (~token, ~remember=?, ()) =>
      identity##recoverAccount(token, Js.Nullable.fromOption(remember))
      |> Js.Promise.then_(user => fromJsUser(user) |> Js.Promise.resolve),
    updateUser: data =>
      identity##updateUser({"data": data})
      |> Js.Promise.then_(user =>
           user->Js.Nullable.toOption->Belt.Option.map(fromJsUser)
           |> Js.Promise.resolve
         ),
    getFreshJWT: () => identity##getFreshJWT(),
    url: identity##_url,
    loginProvider: provider =>
      identity##loginProvider(providerToJs(provider)),
    acceptInviteExternalUrl: (~provider, ~token) =>
      identity##acceptInviteExternalUrl(providerToJs(provider), token),
    settings: identity##settings,
  };
};
