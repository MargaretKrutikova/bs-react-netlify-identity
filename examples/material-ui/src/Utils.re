/** material ui */
external styleToString: ReactDOMRe.Style.t => string = "%identity";

let px_of_float = x => x->int_of_float->string_of_int ++ "px";
let px_of_int = x => x->string_of_int ++ "px";

let spacing = (ind, theme) =>
  theme->MaterialUi.Theme.Theme.spacingGet(ind)->px_of_int;

let getInputValue = (e): string => ReactEvent.Form.target(e)##value;
