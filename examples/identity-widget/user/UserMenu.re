let style = ReactDOMRe.Style.make;
[%mui.withStyles
  "Styles"(_ =>
    {
      userName: style(~fontWeight="500", ~width="100%", ()),
      userListItem: style(~flexWrap="wrap", ()),
    }
  )
];

[@react.component]
let make = (~anchorEl, ~onLogout, ~onClose) => {
  let identity = ReactNetlifyIdentity.useIdentityContext();
  let (status, setStatus) = UserUtils.useStateSafe(() => FormData.NotAsked);

  let userName =
    identity.user
    ->Belt.Option.flatMap(user => user.metaData)
    ->Belt.Option.map(data => data##full_name);

  let handleLogout = _ => {
    setStatus(_ => Submitting);
    identity.logoutUser()
    |> Js.Promise.then_(_ => {
         setStatus(_ => Success);
         onLogout() |> Js.Promise.resolve;
       })
    |> Js.Promise.catch(error =>
         setStatus(_ => Error(UserUtils.promiseErrorToJsObj(error)##message))
         |> Js.Promise.resolve
       )
    |> ignore;
  };

  let classes = Styles.useStyles();

  <MaterialUi_Menu
    anchorEl
    open_={Belt.Option.isSome(anchorEl)}
    onClose
    getContentAnchorEl=None
    anchorOrigin={MaterialUi_Menu.AnchorOrigin.make(
      ~horizontal=`Enum(`Center),
      ~vertical=`Enum(`Bottom),
    )}
    transformOrigin={MaterialUi_Menu.TransformOrigin.make(
      ~horizontal=`Enum(`Center),
      ~vertical=`Enum(`Top),
    )}>
    {switch (userName) {
     | None => React.null
     | Some(name) =>
       <MaterialUi_MenuItem className={classes.userListItem}>
         <MaterialUi_Typography component={`String("div")}>
           {React.string("Logged in as:")}
         </MaterialUi_Typography>
         <MaterialUi_Typography
           component={`String("div")} className={classes.userName}>
           {React.string(name)}
         </MaterialUi_Typography>
       </MaterialUi_MenuItem>
     }}
    <MaterialUi_Divider variant=`FullWidth />
    <MaterialUi_MenuItem onClick=handleLogout disabled={status === Submitting}>
      <MaterialUi_ListItemText primary={React.string("Log out")} />
    </MaterialUi_MenuItem>
  </MaterialUi_Menu>;
};
