[@react.component]
let make = () => {
  let identity = ReactNetlifyIdentity.useIdentityContext();

  let (userMenuAnchor, setUserMenuAnchor) = React.useState(() => None);
  let (showIdentityDialog, setShowIdentityDialog) =
    React.useState(() => false);

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
                 onClick={e => {
                   let anchor = e->ReactEvent.Mouse.currentTarget;
                   setUserMenuAnchor(_ => Some(anchor));
                 }}>
                 {React.string("My Account")}
               </MaterialUi_Button>
               <UserMenu
                 anchorEl=userMenuAnchor
                 onClose={(_, _) => setUserMenuAnchor(_ => None)}
               />
             </>
           : <>
               <MaterialUi_Button color=`Inherit
                 onClick={_ => setShowIdentityDialog(_ => true)}>
                 {React.string("Log in")}
               </MaterialUi_Button>
               <IdentityDialog
                 open_=showIdentityDialog
                 onClose={() => setShowIdentityDialog(_ => false)}
               />
             </>}
      </MaterialUi_Toolbar>
    </MaterialUi_Container>
  </MaterialUi_AppBar>;
};
