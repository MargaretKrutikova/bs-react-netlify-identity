external promiseErrorToJsObj: Js.Promise.error => Js.t('a) = "%identity";

let errorToMessage = (error: Js.Promise.error) =>
  (
    switch (promiseErrorToJsObj(error)##message->Js.Nullable.toOption) {
    | None => error
    | Some(message) => message
    }
  )
  ->Js.Json.stringifyAny
  ->Belt.Option.getWithDefault("");

let useReducerSafe = (reducer, initState) => {
  let (state, dispatch) = React.useReducer(reducer, initState);
  let mounted = React.useRef(false);

  React.useEffect0(() => {
    mounted->React.Ref.setCurrent(true);
    Some(() => mounted->React.Ref.setCurrent(false));
  });

  let dispatchSafe = (action: 'a) =>
    if (mounted->React.Ref.current) {
      dispatch(action);
    };
  (state, dispatchSafe);
};

let useStateSafe = initState => {
  let (state, setState) = React.useState(initState);
  let mounted = React.useRef(false);

  React.useEffect0(() => {
    mounted->React.Ref.setCurrent(true);
    Some(() => mounted->React.Ref.setCurrent(false));
  });

  let setStateSafe = updateState =>
    if (mounted->React.Ref.current) {
      setState(updateState);
    };
  (state, setStateSafe);
};
