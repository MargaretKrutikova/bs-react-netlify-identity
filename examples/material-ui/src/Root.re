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

  <>
    <MaterialUi_CssBaseline />
    <AppBar
      showIdentityDialog
      openDialog={_ => setShowIdentityDialog(_ => true)}
      closeDialog={_ => setShowIdentityDialog(_ => false)}
    />
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
    </MaterialUi_Container>
  </>;
};
