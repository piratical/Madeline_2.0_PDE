#!/usr/bin/env bash

error() {
  echo "$@" 1>&2
}

error_and_exit() {
  error "$@"; exit 1;
}

has_any_files_of_ext() {
  local ext="$1"
  [[ $( ls testdata/input/*.${ext} 2>/dev/null | wc -l ) -ne 0 ]]
}

set -x

[[ -d tmp/output ]] || mkdir -p tmp/output

cmake . && make

for ext in data xml; do
  if has_any_files_of_ext ${ext}; then
    for input_file in testdata/input/*.${ext}; do
      short_name="$( basename ${input_file} .${ext} )"

      src/madeline2  --outputprefix tmp/output/${short_name} ${input_file} || exit 1
      if !  cmp tmp/output/${short_name}.xml testdata/travis_ci_expected/${short_name}.xml; then
        error_and_exit "tmp/output/${short_name}.xml does not match testdata/travis_ci_expected/${short_name}.xml"
      fi
      echo; echo
    done
  fi
done

set +x

echo "<html><body><ul>" > tmp/output/index.html

for f in $( ls tmp/output | grep -v 'index\.html' ); do
  echo "<li><a href='$f'>$f</a></li>" >> tmp/output/index.html
done

echo "</ul></body></html>" >> tmp/output/index.html
