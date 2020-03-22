# bs-react-netlify-identity

Reason bindings for [react-netlify-identity](https://github.com/sw-yx/react-netlify-identity) with `react` context and hooks.

[![NPM](https://img.shields.io/npm/v/bs-react-netlify-identity.svg)](https://www.npmjs.com/package/bs-react-netlify-identity)

Includes example of a fully implemented authentication solution using netlify identity (login, signup, forgot password, account recovery and login using external providers) wrapped in `material-ui` with hooks creating a beautiful design out of the box 😍

Check out demo at https://bs-react-netlify-identity.netlify.com/.

---

## Installation

```sh
npm i --save bs-react-netlify-identity
// or
yarn add bs-react-netlify-identity
```

Then update your bsconfig.json:

```diff
"bs-dependencies": [
  ...
+ "bs-react-netlify-identity",
]
```

## Setting up

You need to have an active Netlify site with Netlify identity turned on, instructions are in [this section readme](https://github.com/sw-yx/react-netlify-identity#usage) of `react-netlify-identity`.

Add the netlify identity provider to the top of the tree:

```reason
// url of the netlify site, should already be public, so no point in using .env
let url = "https://your-identity-instance.netlify.com/";

ReactDOMRe.renderToElementWithId(
  <ReactNetlifyIdentity.IdentityContextProvider url>
   ...
  </ReactNetlifyIdentity.IdentityContextProvider>,
  "root",
);
```

## Usage

There are two different hooks that can be used, depending on whether you need to save extra meta data about the user (besides email and password), like username, date of birth etc. and be able to access the data as a strongly typed reason record (as opposed to `Js.t({..})`).

The extra data about the user is saved in user metadata field by [`go-true`](https://github.com/netlify/gotrue-js) (used in `react-netlify-identity`) when calling `signupUser`. The simpler version of the hook will have this field typed to a js object `Js.t({..})` and is not intended to be used if you need to manipulate that metadata (compiler won't know anything about the type so it would be extremely unsafe).

If you want to manipulate the meta data object, you will want to define your own type for user's metadata, conversion functions (to and from JS) and configure the hook with those in order to get compiler help when accessing the metadata field.

### Simple case: signup, login, logout

Let's start with the simple version of the hook where you don't need to take account of the user metadata - `useIdentityContextSimple`. Here are some basic operations on the netlify identity object we can perform:

```reason
// somewhere in your component
let identity = ReactNetlifyIdentity.useIdentityContextSimple();

// check if user is logged in
let isLoggedIn = identity.isLoggedIn;

// login user
identity.loginUser(~email, ~password, ~remember, ())
    |> Js.Promise.then_(_ => ...);

// signup user with data as empty js object since there is no need for extra data
identity.signupUser(~email, ~password, ~data=Js.Obj.empty())
    |> Js.Promise.then_(_ => ...);

// logout
identity.logoutUser()
    |> Js.Promise.then_(_ => ...);
```

These operations will look exactly the same for the other version of the hook, where we need to have the metadata typed.

## Account recovery

To recover an account we first call `requestPasswordRecovery` with the user's email:

```reason
// somewhere in your component
let identity = ReactNetlifyIdentity.useIdentityContextSimple();

identityContext.requestPasswordRecovery(~email)
    |> Js.Promise.then_(_ => ...);
```

This will send an email with a link containing a recovery token to the email address, for more information and testing tips check `gotrue-js` docs on [account recovery](https://github.com/netlify/gotrue-js#request-password-recover-email).

The recovery token is appended as a parameter to the link and will be parsed into `identity.param`, that we can access in a root component when the application loads:

```reason
let identity = ReactNetlifyIdentity.useIdentityContextSimple();

// Handle recovery token.
switch (identity.param.token, identity.param.type_) {
  | (Some(_), NetlifyToken.Recovery) =>
    identity.recoverAccount(~remember=false, ())
    |> Js.Promise.(then_(/* show a dialog to reset password */))
    |> ignore
  | _ => ignore()
  };
```

If the token is not empty and is of type `Recovery`, we can call `identity.recoverAccount`, which will log in the user, for more see [gotrue-js recovery](https://github.com/netlify/gotrue-js#recover-a-user-account).

Check the full example at [Root.re](examples/material-ui/src/Root.re) and [RecoveryDialog.re](examples/material-ui/src/user/RecoveryDialog.re)

## Other token types

Token, that comes as a url parameter, can serve different purposes:

- account recovery,
- invitation to create an account,
- email change, etc.

To model these types, there is a module `NetlifyToken` that exposes the following variant:

```reason
type tokenType =
  | Invite
  | Recovery
  | EmailChange
  | Unknown;
```

Expanding on the previous step for account recovery, we can handle other token types in the root component:

```reason
let identity = ReactNetlifyIdentity.useIdentityContextSimple();

switch (identity.param.token, identity.param.type_) {
  | (Some(_), NetlifyToken.Recovery) => // recover
  | (Some(_), NetlifyToken.Invite) => // register user
  | (Some(_), NetlifyToken.EmailChange) // change email
  | _ => ignore()
  };
```

## Work with user metadata

There is a functor `ReactNetlifyIdentity.Make`, that needs to know the type of the metadata on the `reason` side (record), the type of the metadata on the JS side (a js object) and functions to convert between the two.

If you don't need to have different metadata representations (e.g. field "full_name" in the netlify identity db and field "fullName" in `reason`), you can use `[@bs.deriving {jsConverter: newType}]` that will generate a type for JS (`abs_myMetadata`) and conversion functions (`myMetadataFromJs` and `myMetadataToJs`), more on this in the docs on [converters](https://bucklescript.github.io/docs/en/generate-converters-accessors#more-safety).

Let's say we want to save user name as metadata. We need to define a file where we will create a hooks based on the custom metadata type, e.g. `UserIdentity.re`:

```reason
[@bs.deriving {jsConverter: newType}]
type userMetaData = {userName: string};

module Context =
  ReactNetlifyIdentity.Make({
    type userData = userMetaData;
    type userDataJs = abs_userMetaData;
    let convertFromJs = userMetaDataFromJs;
    let convertToJs = userMetaDataToJs;
  });
```

Now we will use

```reason
UserIdentity.Context.useIdentityContext()
```

instead of `ReactNetlifyIdentity.useIdentityContextSimple()` and we have access to all the same functionality, the only difference being `signupUser`, that accepts third argument of type `userMetaData`, and how we access the metadata object:

```reason
let identity = UserIdentity.Context.useIdentityContext();

// won't compile if passed wrong type in data
identity.signupUser(~email, ~password, ~data={userName: userName})
    |> Js.Promise.then_(_ => ...);

// access username
let userName =
  identity.user
  ->Belt.Option.flatMap(u => u.metaData)
  // won't compile if the property doesn't exist on the metadata type
  ->Belt.Option.map(m => m.userName);
```

## Work with roles

Netlify allows you to configure roles and assign them to your users either manually via the interface, or using a lambda function (see [how to set roles in Netlify Identity](https://www.netlify.com/blog/2019/02/21/the-role-of-roles-and-how-to-set-them-in-netlify-identity/)).

Roles are then stored in app metadata on the `user` object. Here is an example of how to access roles from a reason component:

```reason
let identity = UserIdentity.Context.useIdentityContext();

let roles =
  identity.user
  ->Belt.Option.flatMap(u => u.appMetaData)
  ->Belt.Option.flatMap(m => m.roles);

let isAdmin =
  switch (roles) {
  | Some(roles) =>
    roles->Belt.Array.keep(r => r == "admin")->Belt.Array.length > 0
  | _ => false
  };
```

## Work with external providers

An external provider is exposed as variant:

```reason
type provider =
  | Bitbucket
  | GitHub
  | GitLab
  | Google;
```

and can be used to implement login with a specific provider, e.g.:

```reason
let identity = ReactNetlifyIdentity.useIdentityContextSimple();
let handleClick = _ => identity.loginProvider(GitHub);
```

It is however more practical to get the enabled providers via the identity hook and iterate over them:

```reason
let identity = ReactNetlifyIdentity.useIdentityContextSimple();

identity.settings.providers
->Belt.Array.map(provider =>
    <MyProviderButton
      provider
      key={ReactNetlifyIdentity.providerToString(provider)}
    />
  )
|> React.array;
```

## Running locally

```sh
yarn
npm start
```

## License

MIT © [Margaret](https://github.com/MargaretKrutikova)
