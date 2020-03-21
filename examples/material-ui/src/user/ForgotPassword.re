let str = ReasonReact.string;

type state = {
  email: string,
  status: FormData.status,
};

let initState = {email: "", status: NotAsked};

type action =
  | SetEmail(string)
  | SubmitRequest
  | SubmitError(string)
  | SubmitSuccess;

let reducer = (state, action) => {
  switch (action) {
  | SetEmail(email) => {...state, email}
  | SubmitRequest => {...state, status: Submitting}
  | SubmitError(msg) => {...state, status: Error(msg)}
  | SubmitSuccess => {...state, status: Success}
  };
};

[@react.component]
let make = (~gotoLogin) => {
  let theme = Mui_Theme.useTheme();

  let (state, dispatch) = UserUtils.useReducerSafe(reducer, initState);
  let identityContext = UserIdentity.Context.useIdentityContext();

  let {email, status} = state;

  let handleRecoverPassword = () => {
    dispatch(SubmitRequest);
    identityContext.requestPasswordRecovery(~email)
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
      handleRecoverPassword();
    }}>
    <MaterialUi_FormControl
      fullWidth=true classes=[Root(Styles.Form.formElement(theme))]>
      <MaterialUi_TextField
        autoFocus=true
        label={str("Email address")}
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
      fullWidth=true
      className={Styles.Form.submitButton(theme)}
      classes=[Root(Styles.Form.formElement(theme))]>
      <MaterialUi_Button
        color=`Primary
        disabled={status === Submitting}
        variant=`Contained
        type_="submit"
        fullWidth=true>
        {str("Send recovery email")}
      </MaterialUi_Button>
    </MaterialUi_FormControl>
    {switch (status) {
     | Error(message) => <ErrorMessage message />
     | Success => str("Message sent.")
     | _ => React.null
     }}
    <UserLink onClick=gotoLogin> {str("Back to login")} </UserLink>
  </form>;
};
