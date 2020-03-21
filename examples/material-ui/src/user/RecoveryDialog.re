open FormData;
let str = ReasonReact.string;

[@react.component]
let make = (~open_, ~onClose) => {
  let theme = Mui_Theme.useTheme();

  let (state, dispatch) = UserUtils.useReducerSafe(reducer, initState);
  let {password, status} = state;

  let identity = UserIdentity.Context.useIdentityContext();

  let handleSetPassword = _ => {
    dispatch(SubmitRequest);

    identity.updateUser({"password": password} |> Utils.toJson)
    |> Js.Promise.then_(_ => dispatch(SubmitSuccess) |> Js.Promise.resolve)
    |> Js.Promise.catch(error => {
         Js.log(error);
         dispatch(SubmitError(UserUtils.errorToMessage(error)))
         |> Js.Promise.resolve;
       })
    |> ignore;
  };

  <MaterialUi_Dialog
    open_
    onClose={(_, _) => onClose()}
    scroll=`Body
    classes=[PaperScrollBody(Styles.Dialog.root(theme))]>
    <MaterialUi_DialogContent className=Styles.Dialog.dialogContent>
      <MaterialUi_DialogTitle className=Styles.Dialog.title>
        {str("Reset your password")}
      </MaterialUi_DialogTitle>
      <form
        onSubmit={e => {
          ReactEvent.Form.preventDefault(e);
          handleSetPassword();
        }}>
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
            {str("Reset password")}
          </MaterialUi_Button>
        </MaterialUi_FormControl>
        {switch (status) {
         | Error(message) => <ErrorMessage message />
         | Success => str("Password updated. Please log in.")
         | _ => React.null
         }}
      </form>
    </MaterialUi_DialogContent>
  </MaterialUi_Dialog>;
};
