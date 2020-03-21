type status =
  | NotAsked
  | Submitting
  | Error(string)
  | Success;

type state = {
  email: string,
  password: string,
  userName: string,
  status,
};

let initState = {email: "", password: "", userName: "", status: NotAsked};
type action =
  | SetEmail(string)
  | SetPassword(string)
  | SetUserName(string)
  | SubmitRequest
  | SubmitError(string)
  | SubmitSuccess;

let reducer = (state, action) => {
  switch (action) {
  | SetEmail(email) => {...state, email}
  | SetPassword(password) => {...state, password}
  | SetUserName(userName) => {...state, userName}
  | SubmitRequest => {...state, status: Submitting}
  | SubmitError(msg) => {...state, status: Error(msg)}
  | SubmitSuccess => {...state, status: Success}
  };
};
