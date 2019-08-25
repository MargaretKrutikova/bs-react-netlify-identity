/**
 * Extra data about the user that will saved during signup process,
 * e.g. user name, first name, last name, date of birth etc.
 * see Signup.re
 */

type userMetaData = {. "user_name": Js.Nullable.t(string)};
