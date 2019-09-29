open Css;

module Form = {
  let submitButton = theme =>
    style([marginTop(px(theme |> Utils.spacingPx(1)))]);

  let formElement = theme =>
    style([marginBottom(px(theme |> Utils.spacingPx(2)))]);

  let errorMessage = style([textAlign(`center), fontSize(px(16))]);

  let divider = style([flexGrow(1.0), flexShrink(1.0), flexBasis(`auto)]);

  let providerSection = theme =>
    style([
      marginTop(px(theme |> Utils.spacingPx(3))),
      marginBottom(px(theme |> Utils.spacingPx(3))),
      display(`flex),
      alignItems(`center),
    ]);
};

module Dialog = {
  let root = theme =>
    style([
      width(vw(90.0)),
      margin2(~v=vh(10.0), ~h=`auto),
      maxWidth(px(400)) |> important,
      padding(px(theme |> Utils.spacingPx(2))),
      paddingTop(px(theme |> Utils.spacingPx(4))),
      media(Utils.getBreakpoint(`MD, theme), [width(vw(50.0))]),
    ]);

  let dialogContent = style([firstChild([paddingTop(px(0))])]);

  let tab = theme => style([minHeight(px(Utils.spacingPx(1, theme) * 3))]);

  let title = style([paddingLeft(px(0))]);

  let tabRoot = theme =>
    style([marginBottom(px(theme |> Utils.spacingPx(2)))]);
};
