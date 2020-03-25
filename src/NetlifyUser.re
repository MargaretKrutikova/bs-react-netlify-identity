module AppMetaData = {
  type js = {
    provider: string,
    roles: Js.Nullable.t(array(string)),
  };

  type t = {
    provider: string,
    roles: option(array(string)),
  };
  let fromJs = (jsMetaData: js) => {
    provider: jsMetaData.provider,
    roles: jsMetaData.roles->Js.Nullable.toOption,
  };
};

type t('a) = {
  appMetaData: option(AppMetaData.t),
  metaData: option('a),
  id: string,
  email: string,
  role: string,
};

type userJs = {
  app_metadata: Js.Nullable.t(AppMetaData.js),
  user_metadata: Js.Nullable.t(Js.Json.t),
  id: string,
  email: string,
  role: string,
};

let fromJs = (convertMetaDataFromJs, user: userJs) => {
  id: user.id,
  email: user.email,
  role: user.role,
  appMetaData:
    user.app_metadata
    ->Js.Nullable.toOption
    ->Belt.Option.map(AppMetaData.fromJs),
  metaData:
    user.user_metadata
    ->Js.Nullable.toOption
    ->Belt.Option.map(data => convertMetaDataFromJs(data)),
};
