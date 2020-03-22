module Styles = {
  open Css;
  let title = theme =>
    style([
      marginTop(px(theme |> Utils.spacingPx(3))),
      marginBottom(px(theme |> Utils.spacingPx(3))),
    ]);

  let wrapper =
    style([
      display(`flex),
      flexDirection(column),
      alignItems(`center),
      margin2(~h=auto, ~v=px(0)),
    ]);
};

[@react.component]
let make = () => {
  let theme = Mui_Theme.useTheme();

  let identity = UserIdentity.Context.useIdentityContext();
  let (showIdentityDialog, setShowIdentityDialog) =
    React.useState(() => false);
  let (showRecoveryDialog, setShowRecoveryDialog) =
    React.useState(() => false);

  // Handle recovery token.
  switch (identity.param.token, identity.param.type_) {
  | (Some(_), NetlifyToken.Recovery) =>
    identity.recoverAccount(~remember=false, ())
    |> Js.Promise.(then_(_ => setShowRecoveryDialog(_ => true) |> resolve))
    |> ignore
  | _ => ignore()
  };

  <>
    <MaterialUi_CssBaseline />
    <AppBar openDialog={_ => setShowIdentityDialog(_ => true)} />
    <MaterialUi_Container>
      <div className=Styles.wrapper>
        <MaterialUi_Typography variant=`H2 className={Styles.title(theme)}>
          {React.string("Welcome")}
          {switch (
             identity.isLoggedIn,
             identity.user->Belt.Option.flatMap(u => u.metaData),
           ) {
           | (true, Some(data)) =>
             React.string(", " ++ data.userName ++ "!")
           | (_, _) => React.string("!")
           }}
        </MaterialUi_Typography>
        {!identity.isLoggedIn
           ? <>
               <MaterialUi_Button
                 color=`Primary
                 variant=`Contained
                 onClick={_ => setShowIdentityDialog(_ => true)}>
                 {React.string("Log in here")}
               </MaterialUi_Button>
               <MaterialUi_Typography
                 variant=`H4 className={Styles.title(theme)}>
                 {React.string(" or use the app bar")}
               </MaterialUi_Typography>
             </>
           : <MaterialUi_Typography
               variant=`H4 className={Styles.title(theme)}>
               {React.string("Log out using the app bar")}
             </MaterialUi_Typography>}
      </div>
      <IdentityDialog
        open_=showIdentityDialog
        onLogin={_ => ignore()}
        onClose={_ => setShowIdentityDialog(_ => false)}
      />
      <RecoveryDialog
        open_=showRecoveryDialog
        onClose={_ => setShowRecoveryDialog(_ => false)}
      />
    </MaterialUi_Container>
  </>;
};
