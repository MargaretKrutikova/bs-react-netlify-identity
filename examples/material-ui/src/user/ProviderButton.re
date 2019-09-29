let getProviderClass = (provider: ReactNetlifyIdentity.provider) =>
  switch (provider) {
  | Google => "providerGoogle"
  | GitHub => "providerGitHub"
  | GitLab => "providerGitLab"
  | Bitbucket => "providerBitbucket"
  };

[@react.component]
let make = (~provider, ~className="") => {
  let identity = ReactNetlifyIdentity.useIdentityContextSimple();
  let handleClick = _ => identity.loginProvider(provider);

  <MaterialUi_Button
    className={getProviderClass(provider)}
    onClick=handleClick
    fullWidth=true
    classes=[Root("providerButton " ++ className)]>
    {React.string(
       "Continue with " ++ ReactNetlifyIdentity.providerToString(provider),
     )}
  </MaterialUi_Button>;
};
