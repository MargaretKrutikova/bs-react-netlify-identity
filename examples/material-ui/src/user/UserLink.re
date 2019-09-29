[@react.component]
let make = (~onClick, ~children) => {
  let css =
    Css.(
      style([
        textDecoration(`underline),
        textTransform(`none),
        padding(px(0)),
        hover([background(`transparent), textDecoration(`underline)]),
      ])
    );

  <MaterialUi_Button
    onClick
    color=`Primary
    fullWidth=true
    component={`String("button")}
    classes=[TextPrimary(css)]>
    children
  </MaterialUi_Button>;
};
