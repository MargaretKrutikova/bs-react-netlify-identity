let style = ReactDOMRe.Style.make;

[@react.component]
let make = () => {
  let theme = Mui_Theme.useTheme();

  let identity = UserIdentity.Context.useIdentityContext();

  <>
    <div className={Styles.Form.providerSection(theme)}>
      <MaterialUi_Divider variant=`Middle className=Styles.Form.divider />
      <MaterialUi_Typography variant=`Body2>
        {React.string("OR")}
      </MaterialUi_Typography>
      <MaterialUi_Divider variant=`Middle className=Styles.Form.divider />
    </div>
    <div id="providers">
      {identity.settings.providers
       ->Belt.Array.map(provider =>
           <ProviderButton
             key={ReactNetlifyIdentity.providerToString(provider)}
             provider
             className={Styles.Form.formElement(theme)}
           />
         )
       |> React.array}
    </div>
  </>;
};
