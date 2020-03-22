external toDomElement: 'a => Dom.element = "%identity";

[@react.component]
let make = (~openDialog) => {
  let identity = UserIdentity.Context.useIdentityContext();

  let (userMenuAnchor, setUserMenuAnchor) = React.useState(() => None);
  let clearAnchor = () => setUserMenuAnchor(_ => None);

  <MaterialUi_AppBar color=`Primary position=`Static>
    <MaterialUi_Container>
      <MaterialUi_Toolbar disableGutters=true>
        <MaterialUi_Typography variant=`H4 color=`Inherit>
          {React.string("Netlify identity")}
        </MaterialUi_Typography>
        <div style={ReactDOMRe.Style.make(~flex="1 1 auto", ())} />
        {identity.isLoggedIn
           ? <>
               <MaterialUi_Button
                 color=`Inherit
                 onClick={e => {
                   let anchor = e->ReactEvent.Mouse.target->toDomElement;
                   setUserMenuAnchor(_ => Some(anchor));
                 }}>
                 {React.string("My Account")}
               </MaterialUi_Button>
               {switch (userMenuAnchor) {
                | Some(_) =>
                  <UserMenu
                    anchorEl=userMenuAnchor
                    onLogout=clearAnchor
                    onClose={(_, _) => clearAnchor()}
                  />
                | None => React.null
                }}
             </>
           : <MaterialUi_Button color=`Inherit onClick=openDialog>
               {React.string("Log in")}
             </MaterialUi_Button>}
      </MaterialUi_Toolbar>
    </MaterialUi_Container>
  </MaterialUi_AppBar>;
};
