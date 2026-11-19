# Release Checklist

Step-by-step release procedure with the non-obvious details ("rat trails") collected
along the way. The short version for users lives in the README ("Automated Builds
and Releases"); this file is for the maintainer.

## 1. Version bump (one commit before the tag)

- `Sources/CMakeLists.txt`: `project(Shprot VERSION X.Y LANGUAGES CXX)` — the only
  place where the version is defined. Everything else (package file names, NSIS
  installer, About dialog, update checker) derives from it.
- `README.md`, `README.ru.md`, `README.zh.md`: the tray menu item example in the
  "New Version Available (X-1 → X)" line. See the "Version bumped" commit for the
  exact pattern.

## 2. Tag

    git tag vX.Y && git push origin vX.Y

- The workflow's `check-version` job fails immediately if the tag does not match
  the project version, before hours of builds.
- The release job runs only for `v*` tags; pushes to `main` produce artifacts only.

## 3. CI build (`.github/workflows/build.yml`)

- Builds the Windows installer (NSIS) and the DEB and RPM packages in parallel;
  unit tests run on all three platforms as part of the same workflow.
- Docker builder images are cached in GHCR keyed by the Dockerfile hash. Only the
  first run after a Dockerfile change is long (Qt build, ~20 minutes on GitHub
  runners).
- Known quirk: when two matrix jobs create *new* GHCR packages simultaneously
  (for example, after renaming an image), one push can fail with
  `error from registry: denied`. Just use "Re-run failed jobs" — without the
  concurrent push it succeeds.
- Artifacts (`package-windows`, `package-deb`, `package-rpm`) belong to the run,
  but the UI shows them per attempt: after "Re-run failed jobs" the artifacts of
  the jobs that did not re-run are visible only in attempt #1 (switch it in the
  "Latest #N" dropdown) or via
  `gh api repos/<owner>/<repo>/actions/runs/<id>/artifacts`.

## 4. Draft release

Once all builds succeed, the release job creates a **draft** release with:

- the three packages,
- `SHA256SUMS` (so users can run `sha256sum -c SHA256SUMS --ignore-missing`),
- build provenance attestations (so users can run
  `gh attestation verify <package file> -R andrew-markin/shprot`),
- auto-generated release notes.

Review the draft and publish it manually.

## 5. After publishing

- `.github/workflows/site.yml` runs on every published release, so the site
  download links point to the new release automatically. No site edits needed
  unless compatibility texts changed.
- The update checker in the app picks the release up from the GitHub API on its
  own (an hour after start, then every 12 hours). Nothing to do.

## 6. winget manifests

For each released version, add a folder
`Packaging/winget/manifests/a/AndrewMarkin/Shprot/<version>/` with the three
manifests, copying and updating the previous version's folder:

- `AndrewMarkin.Shprot.yaml` — `PackageVersion`.
- `AndrewMarkin.Shprot.locale.en-US.yaml` — `PackageVersion`,
  `ReleaseNotesUrl` (`.../releases/tag/vX.Y`).
- `AndrewMarkin.Shprot.installer.yaml` — `PackageVersion`, `ReleaseDate`,
  `InstallerUrl` (`.../releases/download/vX.Y/Shprot-X.Y.exe`),
  `InstallerSha256` (SHA-256 of the released `.exe`),
  `DisplayVersion`.

Instead of opening a new PR per version (which goes to the back of the validation
queue), push the new version's folder into the *open* PR of the previous version
and rename it. The branch is reused since 1.5 (in the fork, branch
`AndrewMarkin.Shprot-1.5-24a155b7-668c-45fc-8868-48dacd8f6f61`), so validation
restarts without losing the queue position.

- **Remove the previous version's folder in the same push.** The validator
  accepts exactly one version directory per PR; pushing `X.Y` while `X.(Y-1)`
  is still there fails with "The pull request contains more than one manifest"
  and emails validation errors. The folder stays in this repository as an
  archive — remove it only in the PR branch.
- `gh pr edit` may fail on an old `gh` (Projects classic deprecation); use
  `gh api -X PATCH repos/microsoft/winget-pkgs/pulls/<n> -f title=...` instead.

## Optional sanity checks before publishing the draft

- Install the RPM on RED OS 7.3 and the DEB on Ubuntu 22.04 in containers and
  start the app under Xvfb.
- Compare the artifact checksums with the local builds from `Release/`.
