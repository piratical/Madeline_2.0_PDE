#!/usr/bin/env bash

#-----------------------------------------------------------------------
# Functions
#-----------------------------------------------------------------------

error() {
  echo "$@" 1>&2
  exit 1
}

is_empty() {
  [[ -z "$@" ]]
}

is_set() {
  [[ -n "$@" ]]
}

parse_options() {
  myopts=$( getopt --options lyfh --longoptions list,yes,force,help -- "$@" ) || exit 1

  eval set -- "$myopts"

  while true; do
    case $1 in
      -l|--list) do_list; shift;;
      -y|--yes) opt_force_install="yes"; shift;;
      -h|--help) usage ;;
      --) shift; break;;
      *) exit 1;;
    esac
    shift
  done
}

#-----------------------------------------------------------------------
# Testing DSL
#-----------------------------------------------------------------------

INPUT() {
  INPUT="$1"
}

OUTPUT() {
  OUTPUT="$1"
}

COLORS() {
  if is_empty "$COLORS"; then
    COLORS="$1"
  else
    COLORS="$COLORS;$1"
  fi
}

OPTIONS() {
  if is_empty "$OPTIONS"; then
    OPTIONS="$@"
  else
    OPTIONS="$OPTIONS "$@""
  fi
}

LABELS() {
  if is_empty "$LABELS"; then
    LABELS="$@"
  else
    LABELS="$LABELS $@"
  fi
}

RUN() {
  is_empty "$INPUT" && error "At minimum the method 'INPUT' must be specified for every test"
  is_empty "$OUTPUT" && OUTPUT="$INPUT"

  if is_set "$COLORS"; then
    OPTIONS="$OPTIONS --custom-icon-colors \"$COLORS\""
  fi

  if is_set "$LABELS"; then
    OPTIONS="$OPTIONS --Labels \"$LABELS\""
  fi

  OPTIONS=""$@" $OPTIONS"

  printf -- "${MADELINE} $OPTIONS --outputprefix ${OUTPUT_DIR}/${OUTPUT} ${INPUT_DIR}/${INPUT}.data" \
    > $OUTPUT_DIR/$OUTPUT-cli.txt

  bash -x $OUTPUT_DIR/$OUTPUT-cli.txt 2>&1 | tee ${OUTPUT_DIR}/${OUTPUT}-log.txt

  echo "<hr/>" >> $OUTPUT_DIR/index.html
  echo "<h2>$OUTPUT</h2>" >> $OUTPUT_DIR/index.html
  echo "<h3>Command Line</h3>" >> $OUTPUT_DIR/index.html

  echo "<pre>$( cat $OUTPUT_DIR/$OUTPUT-cli.txt | \
                sed "s|$MYBASE/||g" | fmt -w 132 \
                | perl -pe 's,\n, \\\n,g;' | sed '$s,\\$,,' )</pre>" >> $OUTPUT_DIR/index.html

  echo "<h3>Log</h3>" >> $OUTPUT_DIR/index.html
  echo "<a href='${OUTPUT}-log.txt'>log</a>" >> $OUTPUT_DIR/index.html
  echo "<h3>Generated Image</h3>" >> $OUTPUT_DIR/index.html
  echo "<img src='${OUTPUT}.svg' alt='${OUTPUT}.svg image' />" >> $OUTPUT_DIR/index.html
  echo "<p/><p/>" >> $OUTPUT_DIR/index.html

  unset -v INPUT
  unset -v OUTPUT
  unset -v OPTIONS
  unset -v COLORS
  unset -v LABELS
}

OUTPUT_DIR=$MYBASE/ci/tmp
INPUT_DIR=$MYBASE/ci/testdata
MADELINE=$MYBASE/src/madeline2

if [[ ! -x $MADELINE ]]; then
  cmake . || exit 1
  make || exit 1
fi

[[ -d $OUTPUT_DIR ]] || mkdir -p $OUTPUT_DIR

> $OUTPUT_DIR/index.html

echo "<head><style>img { max-width: 100%; height: auto; width: auto\9; /* ie8 */ }</style></head>" >> $OUTPUT_DIR/index.html

echo "<h1>Madeline PDE CI Artifacts $(date --utc +%FT%T%z )</h1>" >> $OUTPUT_DIR/index.html



# If input file is remote, specify the file
# name starting with 'http://' or 'https://'.
# To retrieve the data from a mysql database use
# 'mysql://[host:port/]username:passwd@database:table'
#
#         --Labels Specify labels to be displayed on the Pedigree using a single string containing space-delimited labels
#               -L
#             --bw Print pedigrees in black and white
#               -b
#      --collapsed “Collapse” multiple individuals into groups (requires “Collapsed” data column)
#               -k
#          --color Print pedigrees in color
#               -c
# --custom-icon-colors Specify a comma- and semicolon-delimited list of custom icon shading color codes in HTML/CSS hex format.
#               -C
#          --debug Print run-time progress messages
#               -d
#       --embedded Produce an XML file that can be embedded in another XML document
#               -e
#           --font Font to be used for the display of Pedigree labels
#               -f
#      --font-size Font size to be used for the display of Pedigree labels
#               -z
#           --help Print this help and exit
#               -h
#         --labels Path to the file that has a list of labels to be displayed on the Pedigree
#               -l
#   --noiconlabels Prevent the printing of affection status labels on the icons
#               -n
# --nolabeltruncation Prevent truncation of labels on a drawing when they are too wide.
#               -N
# --outputdatatable Output the pedigree input data file (column order preserved) as a tab-delimited file with name 'FAMILY_IDinput.txt'
#               -I
#      --outputext Specify output file name extension
#               -x
# --outputpedtable Output the pedigree core/non-core fields as a tab-delimited file with name 'FAMILY_IDinput.txt'
#               -i
#   --outputprefix Specify output file name prefix
#               -o
# --quadrantshading Use the quadrant shading method to indicate categorical levels on icons
#               -q
#           --sort Field based on which siblings are sorted
#               -s
#        --version Print version and exit
#               -v
#
