ReactDOMRe.renderToElementWithId(
  <ReactNetlifyIdentity.IdentityContextProvider
    url="https://bs-react-netlify-identity.netlify.com/">
    <ThemeProvider>
      <Jss.JssProvider> <Root /> </Jss.JssProvider>
    </ThemeProvider>
  </ReactNetlifyIdentity.IdentityContextProvider>,
  "root",
);
