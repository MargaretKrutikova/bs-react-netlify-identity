type t('a) = {
  metaData: option('a),
  id: string,
  email: string,
  role: string,
};

type userJs = {
  user_metadata: Js.Nullable.t(Js.Json.t),
  id: string,
  email: string,
  role: string,
};

let fromJs = (convertMetaDataFromJs, user: userJs) => {
  id: user.id,
  email: user.email,
  role: user.role,
  metaData:
    user.user_metadata
    ->Js.Nullable.toOption
    ->Belt.Option.map(data => convertMetaDataFromJs(data)),
};
