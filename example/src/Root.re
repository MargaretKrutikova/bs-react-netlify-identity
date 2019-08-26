let style = ReactDOMRe.Style.make;
[%mui.withStyles
  "RootStyles"(theme =>
    {
      title:
        style(
          ~marginTop=theme |> Utils.spacing(3),
          ~marginBottom=theme |> Utils.spacing(3),
          (),
        ),
      wrapper:
        style(
          ~display="flex",
          ~alignItems="center",
          ~flexDirection="column",
          ~margin="0 auto",
          (),
        ),
    }
  )
];

[@react.component]
let make = () => {
  let identity = UserIdentity.Context.useIdentityContext();
  let (showIdentityDialog, setShowIdentityDialog) =
    React.useState(() => false);

  let classes = RootStyles.useStyles();
  <>
    <MaterialUi_CssBaseline />
    <AppBar
      showIdentityDialog
      openDialog={_ => setShowIdentityDialog(_ => true)}
      closeDialog={_ => setShowIdentityDialog(_ => false)}
    />
    <MaterialUi_Container>
      <div className={classes.wrapper}>
        <MaterialUi_Typography variant=`H2 className={classes.title}>
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
        <MaterialUi_Button
          color=`Primary
          variant=`Contained
          onClick={_ => setShowIdentityDialog(_ => true)}>
          {React.string(identity.isLoggedIn ? "Log out here" : "Log in here")}
        </MaterialUi_Button>
        <MaterialUi_Typography variant=`H4 className={classes.title}>
          {React.string(" or use the app bar")}
        </MaterialUi_Typography>
      </div>
    </MaterialUi_Container>
  </>;
};
