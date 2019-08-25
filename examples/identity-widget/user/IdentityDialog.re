let style = ReactDOMRe.Style.make;
let addUnsafe = ReactDOMRe.Style.unsafeAddProp;
let combine = ReactDOMRe.Style.combine;

[%mui.withStyles
  "LoginDialogStyles"(theme =>
    {
      root:
        style(
          ~width="90vw",
          ~margin="10vh auto",
          ~maxWidth="400px!important",
          ~padding=theme |> Utils.spacing(2),
          (),
        )
        ->MaterialUi.ThemeHelpers.addBreakpoint(
            ~theme,
            ~breakpoint=`MD,
            ~style=style(~width="50vw", ()),
          ),
      dialogContent:
        style()
        ->addUnsafe(
            "&:first-child",
            style(~paddingTop=theme |> Utils.spacing(1), ())
            ->Utils.styleToString,
          ),
      title: style(~paddingLeft="0", ()),
      tabRoot: style(~marginBottom=theme |> Utils.spacing(2), ()),
    }
  )
];

type activeView =
  | Login
  | Signup
  | ForgotPassword;

let str = ReasonReact.string;

[@react.component]
let make = (~open_, ~onLogin, ~onClose) => {
  let classes = LoginDialogStyles.useStyles();
  let (activeView, setActiveView) = React.useState(() => Login);
  let identity = ReactNetlifyIdentity.useIdentityContext();

  React.useLayoutEffect1(
    () => {
      if (open_) {
        setActiveView(_ => Login);
      };
      None;
    },
    [|open_|],
  );

  let hasProviders = identity.settings.providers->Belt.Array.length > 0;

  <MaterialUi_Dialog
    open_
    onClose={(_, _) => onClose()}
    scroll=`Body
    classes=[PaperScrollBody(classes.root)]>
    <MaterialUi_DialogContent className={classes.dialogContent}>
      {switch (activeView) {
       | ForgotPassword =>
         <>
           <MaterialUi_DialogTitle className={classes.title}>
             {str("Recover password")}
           </MaterialUi_DialogTitle>
           <ForgotPassword gotoLogin={_ => setActiveView(_ => Login)} />
         </>
       | other =>
         <>
           <MaterialUi_Tabs
             value=activeView
             indicatorColor=`Primary
             textColor=`Primary
             classes=[Root(classes.tabRoot)]
             variant=`FullWidth>
             <MaterialUi_Tab
               onClick={_ => setActiveView(_ => Login)}
               label={React.string("Log in")}
             />
             <MaterialUi_Tab
               label={React.string("Sign up")}
               onClick={_ => setActiveView(_ => Signup)}
             />
           </MaterialUi_Tabs>
           {other === Login
              ? <Login
                  onLogin
                  gotoForgotPassword={_ => setActiveView(_ => ForgotPassword)}
                />
              : <Signup />}
           {hasProviders ? <Providers /> : React.null}
         </>
       }}
    </MaterialUi_DialogContent>
  </MaterialUi_Dialog>;
};
