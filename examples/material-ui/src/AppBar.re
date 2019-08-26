[@react.component]
let make = (~showIdentityDialog, ~openDialog, ~closeDialog) => {
  let identity = UserIdentity.Context.useIdentityContext();

  let (userMenuAnchor, setUserMenuAnchor) = React.useState(() => None);

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
                   let anchor = e->ReactEvent.Mouse.currentTarget;
                   setUserMenuAnchor(_ => Some(anchor));
                 }}>
                 {React.string("My Account")}
               </MaterialUi_Button>
               <UserMenu
                 anchorEl=userMenuAnchor
                 onLogout={_ => ignore()}
                 onClose={(_, _) => setUserMenuAnchor(_ => None)}
               />
             </>
           : <>
               <MaterialUi_Button color=`Inherit onClick=openDialog>
                 {React.string("Log in")}
               </MaterialUi_Button>
               <IdentityDialog
                 open_=showIdentityDialog
                 onLogin={_ => ignore()}
                 onClose=closeDialog
               />
             </>}
      </MaterialUi_Toolbar>
    </MaterialUi_Container>
  </MaterialUi_AppBar>;
};
