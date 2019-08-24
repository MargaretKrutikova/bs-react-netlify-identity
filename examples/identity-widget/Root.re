[@react.component]
let make = () => {
  <>
    <MaterialUi_CssBaseline />
    <AppBar />
    <MaterialUi_Container>
      <MaterialUi_Typography variant=`H1>
        {React.string("404 Found nothing")}
      </MaterialUi_Typography>
    </MaterialUi_Container>
  </>;
};
