type t = {
  autoconfirm: bool,
  disableSignup: bool,
  providers: array(NetlifyProvider.t),
};

type externalSettings = {
  bitbucket: bool,
  email: bool,
  facebook: bool,
  github: bool,
  gitlab: bool,
  google: bool,
};

type settingsJs = {
  [@bs.as "external"]
  external_: externalSettings,
  autoconfirm: bool,
  disable_signup: bool,
};

let addProvider = (provider, isOn, providers) =>
  if (isOn) {
    Belt.Array.concat(providers, [|provider|]);
  } else {
    providers;
  };

let fromJs = settings => {
  let jsProviders = settings.external_;

  let providers =
    [||]
    |> addProvider(NetlifyProvider.Bitbucket, jsProviders.bitbucket)
    |> addProvider(NetlifyProvider.GitHub, jsProviders.github)
    |> addProvider(NetlifyProvider.GitLab, jsProviders.gitlab)
    |> addProvider(NetlifyProvider.Google, jsProviders.google);

  {
    autoconfirm: settings.autoconfirm,
    disableSignup: settings.disable_signup,
    providers,
  };
};
