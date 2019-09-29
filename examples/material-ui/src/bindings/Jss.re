[@bs.module "jss"] external create: 'a => 'b = "create";

[@bs.module "@material-ui/styles"]
external jssPreset: unit => 'a = "jssPreset";

module StylesProvider = {
  [@bs.module "@material-ui/styles"] [@react.component]
  external make: (~jss: 'a, ~children: React.element) => React.element =
    "StylesProvider";
};

module JssProvider = {
  let preset = jssPreset();

  // Define a custom insertion for injecting the JSS styles in the DOM
  let jssCustomPreset = [%bs.raw
    {|{  ...preset, insertionPoint: "jss-insertion-point" } |}
  ];
  let jss = create(jssCustomPreset);

  [@react.component]
  let make = (~children) => {
    <StylesProvider jss> children </StylesProvider>;
  };
};
