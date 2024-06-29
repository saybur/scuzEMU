# scuzEMU Conversion Scripts

Very simple Bash scripts to handle file conversions between the Mac environment
and the expectations for git. You will need `tr` and `iconv` installed for
these to run correctly.

- `compare`: compares files in the _import_ and _export_ directories to verify
  file conversion scripts create equivalent files.
- `import`: copies relevant files from _import_ to _src_, converting from Mac
  Roman to UTF-8 and changing classic Mac line endings to Unix ones.
- `export`: copies relevant files from _src_ to _export_, converting from UTF-8
  to Mac Roman and changing Unix line endings to classic Mac ones.

Execute in the root of the repository.
