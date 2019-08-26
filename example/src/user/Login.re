open FormData;

let style = ReactDOMRe.Style.make;
[%mui.withStyles
  "LoginDialogStyles"(theme =>
    {
      loginButton: style(~marginTop=theme |> Utils.spacing(1), ()),
      formElement: style(~marginBottom=theme |> Utils.spacing(2), ()),
    }
  )
];

let str = ReasonReact.string;

[@react.component]
let make = (~gotoForgotPassword, ~onLogin) => {
  let classes = LoginDialogStyles.useStyles();
  let (state, dispatch) =
    UserUtils.useReducerSafe(FormData.reducer, FormData.initState);
  let identity = UserIdentity.Context.useIdentityContext();

  let {email, password, status} = state;

  let handleLogin = () => {
    dispatch(SubmitRequest);
    identity.loginUser(~email, ~password, ~remember=true, ())
    |> Js.Promise.then_(_ => {
         dispatch(SubmitSuccess);
         onLogin() |> Js.Promise.resolve;
       })
    |> Js.Promise.catch(error =>
         dispatch(
           SubmitError(UserUtils.promiseErrorToJsObj(error)##message),
         )
         |> Js.Promise.resolve
       )
    |> ignore;
  };

  <form
    onSubmit={e => {
      ReactEvent.Form.preventDefault(e);
      handleLogin();
    }}>
    <MaterialUi_FormControl
      fullWidth=true classes=[Root(classes.formElement)]>
      <MaterialUi_TextField
        autoFocus=true
        label={str("Email Address")}
        type_="email"
        fullWidth=true
        value={`String(email)}
        required=true
        name="email"
        disabled={status === Submitting}
        onChange={e => {
          let value = Utils.getInputValue(e);
          dispatch(SetEmail(value));
        }}
      />
    </MaterialUi_FormControl>
    <MaterialUi_FormControl
      fullWidth=true classes=[Root(classes.formElement)]>
      <MaterialUi_TextField
        label={str("Password")}
        type_="password"
        name="password"
        fullWidth=true
        required=true
        disabled={status === Submitting}
        value={`String(password)}
        onChange={e => {
          let value = Utils.getInputValue(e);
          dispatch(SetPassword(value));
        }}
      />
    </MaterialUi_FormControl>
    <MaterialUi_FormControl
      fullWidth=true
      className={classes.loginButton}
      classes=[Root(classes.formElement)]>
      <MaterialUi_Button
        color=`Primary
        disabled={status === Submitting}
        variant=`Contained
        type_="submit"
        fullWidth=true>
        {str("Log in")}
      </MaterialUi_Button>
    </MaterialUi_FormControl>
    {switch (status) {
     | Error(message) => <ErrorMessage message />
     | _ => React.null
     }}
    <UserLink onClick=gotoForgotPassword> {str("Forgot password")} </UserLink>
  </form>;
};
