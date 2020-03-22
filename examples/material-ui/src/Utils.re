external toJson: 'a => Js.Json.t = "%identity";

/** material ui */
let spacingPx = (ind, theme) =>
  theme->MaterialUi.Theme.Theme.spacingGet(ind);

let getInputValue = (e): string => ReactEvent.Form.target(e)##value;

let getBreakpoint = (breakpoint, theme) => {
  let muiBreakpoint =
    switch (breakpoint) {
    | `XS => "xs"->MaterialUi.ThemeHelpers.stringToBreakpoint
    | `SM => "sm"->MaterialUi.ThemeHelpers.stringToBreakpoint
    | `MD => "md"->MaterialUi.ThemeHelpers.stringToBreakpoint
    | `LG => "lg"->MaterialUi.ThemeHelpers.stringToBreakpoint
    | `XL => "xl"->MaterialUi.ThemeHelpers.stringToBreakpoint
    | `Int(x) => x->MaterialUi.ThemeHelpers.intToBreakpoint
    };

  theme
  ->MaterialUi.ThemeHelpers.Theme.breakpointsGet
  ->MaterialUi.ThemeHelpers.Breakpoints.upGet
  ->MaterialUi.ThemeHelpers.jsonToBreakpointFunc(muiBreakpoint)
  |> Js.String.replace("@media", "");
};
