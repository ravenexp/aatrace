#!/bin/bash

P=$(dirname "$0")
S="$1"
FONT=${AAFONT:-${P}/font/font8x16.pgm}
SCALE="$2"

[[ -f $S ]] || {
    echo "Usage:"
    echo "    aaconvert image.pgm [diffscale]"
    exit
} >&2

[[ -f $FONT ]] || {
    echo "No font file: '$FONT'"
    echo "Check AAFONT= env var"
    exit
} >&2

BS=$(basename "$S" ".pgm")

D="${BS}.diff.pgm"
T="${BS}.txt"
O="${T}.pgm"

die() {
    echo "FAIL! Status: $?"
    exit
} >&2

echo "DIFF:   $S -> $D"
${P}/testdiff "$S" "$D" $SCALE || die
echo "FONT:   $FONT"
echo "CONV:   $D -> $T"
${P}/testconv "$D" "$FONT" "$T" || die
echo "RENDER: $T -> $O"
${P}/testrender "$T" "$FONT" "$O" || die
