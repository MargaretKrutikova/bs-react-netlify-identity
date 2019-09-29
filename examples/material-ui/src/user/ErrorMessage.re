[@react.component]
let make = (~message) => {
  let theme = Mui_Theme.useTheme();

  <MaterialUi_FormControl
    fullWidth=true className={Styles.Form.formElement(theme)} error=true>
    <MaterialUi_FormHelperText className=Styles.Form.errorMessage>
      {message |> Js.String.replace("invalid_grant:", "") |> React.string}
    </MaterialUi_FormHelperText>
  </MaterialUi_FormControl>;
};
