# Release Process

The releases in this repo follow the following process.

## Versioning

There are a few versioning schemes used to tag builds.

### Releases

For releases [calendar versioning](https://calver.org/) is used in the format `YYYY.0M.xx`.
In English this format means:

- `YYYY` the full year
- `0M` zero-padded month
- `xx` an incrementing number if multiple releases are made in one month, resets to 0 each new month

Examples:

- `2005.12.0` First release of December 2005
- `2030.09.3` Fourth release of September 2030
- `2025.02.100` In case we made one-hundred-and-one releases in February 2025

### Merge Requests

Merge requests use the versioning scheme `MRAA.BB.CC`.
In English this format means:

- `MRAA` static prefix `MR` followed by the merge request ID `AA`
- `BB` the build ID
- `CC` the build attempt counter

For more information see the
[GitHub context documentation](https://docs.github.com/en/actions/writing-workflows/choosing-what-your-workflow-does/accessing-contextual-information-about-workflow-runs#github-context). .

### Other builds

For other builds such as the main branch builds, the version is `git.HASH`, where `HASH` is replaced with the current
git commit hash.

## Creating Releases

To create a release, a maintainer has to create a lightweight git tag on the main branch following the calver release
versioning scheme.

The GitHub pipeline will automatically build the web-app and publish it to GitHub pages, as well as store it as an
artifact.

In the future an APK could also be created and signed.

Linux builds are left as an exercise for the reader.
