/**
 * Extra data about the user that will be saved during signup process,
 * e.g. user name, first name, last name, date of birth etc.
 * see Signup.re
 */
[@bs.deriving {jsConverter: newType}]
type userMetaData = {userName: string};

module Context =
  ReactNetlifyIdentity.Make({
    type userData = userMetaData;
    type userDataJs = abs_userMetaData;
    let convertFromJs = userMetaDataFromJs;
    let convertToJs = userMetaDataToJs;
  });
