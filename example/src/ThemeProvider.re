let theme =
  MaterialUi_Theme.create(
    MaterialUi_ThemeOptions.(
      make(
        ~palette=
          PaletteOptions.make(
            ~primary=Primary.make(~main="#613F75", ()),
            ~background=TypeBackground.make(~default="#F9F6F5", ()),
            (),
          ),
        (),
      )
    ),
  );

[@react.component]
let make = (~children) =>
  MaterialUi.(<MuiThemeProvider theme> children </MuiThemeProvider>);
