type t =
  | Bitbucket
  | GitHub
  | GitLab
  | Google;

let toString = str =>
  switch (str) {
  | Bitbucket => "bitbucket"
  | GitHub => "github"
  | GitLab => "gitlab"
  | Google => "google"
  };
