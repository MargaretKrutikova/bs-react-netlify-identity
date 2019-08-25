let style = ReactDOMRe.Style.make;
let addUnsafe = ReactDOMRe.Style.unsafeAddProp;
let combine = ReactDOMRe.Style.combine;

[%mui.withStyles
  "Styles"(_ =>
    {
      link:
        style(
          ~textDecoration="underline",
          ~textTransform="none",
          ~padding="0",
          (),
        )
        ->addUnsafe(
            "&:hover",
            style(
              ~backgroundColor="transparent",
              ~textDecoration="underline",
              (),
            )
            ->Utils.styleToString,
          ),
    }
  )
];

[@react.component]
let make = (~onClick, ~children) => {
  let classes = Styles.useStyles();

  <MaterialUi_Button
    onClick
    color=`Primary
    fullWidth=true
    component={`String("button")}
    classes=[TextPrimary(classes.link)]>
    children
  </MaterialUi_Button>;
};
