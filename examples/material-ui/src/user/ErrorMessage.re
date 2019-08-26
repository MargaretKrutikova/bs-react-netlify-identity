let style = ReactDOMRe.Style.make;

[%mui.withStyles
  "Styles"(theme =>
    {
      formElement: style(~marginBottom=theme |> Utils.spacing(2), ()),
      errorMsg: style(~textAlign="center", ~fontSize="16px", ()),
    }
  )
];

[@react.component]
let make = (~message) => {
  let classes = Styles.useStyles();

  <MaterialUi_FormControl
    fullWidth=true className={classes.formElement} error=true>
    <MaterialUi_FormHelperText className={classes.errorMsg}>
      {message |> Js.String.replace("invalid_grant:", "") |> React.string}
    </MaterialUi_FormHelperText>
  </MaterialUi_FormControl>;
};
