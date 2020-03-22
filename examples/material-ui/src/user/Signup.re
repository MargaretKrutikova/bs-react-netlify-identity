open FormData;
let str = ReasonReact.string;

[@react.component]
let make = () => {
  let theme = Mui_Theme.useTheme();

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
      fullWidth=true classes=[Root(Styles.Form.formElement(theme))]>
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
    <MaterialUi_FormControl
      fullWidth=true classes=[Root(Styles.Form.formElement(theme))]>
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
      fullWidth=true classes=[Root(Styles.Form.formElement(theme))]>
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
      fullWidth=true className={Styles.Form.submitButton(theme)}>
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
