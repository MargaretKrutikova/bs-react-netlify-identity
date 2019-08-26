let str = ReasonReact.string;
let style = ReactDOMRe.Style.make;
[%mui.withStyles
  "Styles"(theme =>
    {
      submitButton: style(~marginTop=theme |> Utils.spacing(1), ()),
      formElement: style(~marginBottom=theme |> Utils.spacing(2), ()),
    }
  )
];

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
  let classes = Styles.useStyles();
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
      fullWidth=true classes=[Root(classes.formElement)]>
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
      className={classes.submitButton}
      classes=[Root(classes.formElement)]>
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
     | _ => React.null
     }}
    <UserLink onClick=gotoLogin> {str("Back to login")} </UserLink>
  </form>;
};
