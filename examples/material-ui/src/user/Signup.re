open FormData;
let style = ReactDOMRe.Style.make;
[%mui.withStyles
  "SignupStyles"(theme =>
    {
      submitButtonElement: style(~marginTop=theme |> Utils.spacing(1), ()),
      formElement: style(~marginBottom=theme |> Utils.spacing(2), ()),
    }
  )
];

let str = ReasonReact.string;

[@react.component]
let make = () => {
  let classes = SignupStyles.useStyles();
  let (state, dispatch) = UserUtils.useReducerSafe(reducer, initState);
  let {email, password, userName, status} = state;

  let identity = UserIdentity.Context.useIdentityContext();

  let handleSignup = _ => {
    dispatch(SubmitRequest);
    // data is of type UserIdentity.userMetaData
    identity.signupUser(~email, ~password, ~data={userName: userName})
    |> Js.Promise.then_(_ => dispatch(SubmitSuccess) |> Js.Promise.resolve)
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
      handleSignup();
    }}>
    <MaterialUi_FormControl
      fullWidth=true classes=[Root(classes.formElement)]>
      <MaterialUi_FormControl
        fullWidth=true classes=[Root(classes.formElement)]>
        <MaterialUi_TextField
          autoFocus=true
          label={str("Username")}
          name="name"
          type_="text"
          fullWidth=true
          required=true
          value={`String(userName)}
          disabled={status === Submitting}
          onChange={e => {
            let value = Utils.getInputValue(e);
            dispatch(SetUserName(value));
          }}
        />
      </MaterialUi_FormControl>
      <MaterialUi_TextField
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
        value={`String(password)}
        name="password"
        fullWidth=true
        required=true
        disabled={status === Submitting}
        onChange={e => {
          let value = Utils.getInputValue(e);
          dispatch(SetPassword(value));
        }}
      />
    </MaterialUi_FormControl>
    <MaterialUi_FormControl
      fullWidth=true className={classes.submitButtonElement}>
      <MaterialUi_Button
        color=`Primary
        disabled={status === Submitting}
        variant=`Contained
        type_="submit"
        fullWidth=true>
        {str("Sign up")}
      </MaterialUi_Button>
    </MaterialUi_FormControl>
    {switch (status) {
     | Error(message) => <ErrorMessage message />
     | _ => React.null
     }}
  </form>;
};
