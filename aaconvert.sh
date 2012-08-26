#!/bin/bash

DIFFSUFF=""
CONVSUFF=""

DIFFOPTS=""
CONVOPTS=""

usage() {
    echo "Usage:"
    echo "    aaconvert.sh [OPTIONS] image.pgm"
    echo "OPTIONS:"
    echo "  [-l <scale>] [-d <kernel>]"
    echo "  [-m <id>] [-W <sad>,<asd>]"
    echo "  [-s <id>] [-r (<px>|<hpx,vpx>)] [-c <1/0>]"
    exit
}

while getopts "l:d:m:W:s:r:c:" OPT
do
    case $OPT in
	l|d)
	    DIFFOPTS="$DIFFOPTS -$OPT $OPTARG"
	    DIFFSUFF="${DIFFSUFF}${OPT}${OPTARG}."
	    ;;
	m|W|s|c|r)
	    CONVOPTS="$CONVOPTS -$OPT $OPTARG"
	    CONVSUFF="${CONVSUFF}${OPT}${OPTARG}."
	    ;;
	*)
	    usage
	    ;;
    esac
done

P=$(dirname "$0")

shift $(($OPTIND - 1))

S="$1"
[[ -n "$S" ]] || usage

D="${S%.pgm}.diff.${DIFFSUFF}pgm"
T="${S%.pgm}.ascii.${DIFFSUFF}${CONVSUFF}txt"
O="${T%.txt}.pgm"

FONT=${AAFONT:-${P}/font/font8x16.pgm}
[[ -f $FONT ]] || {
    echo "Missing font file: '$FONT'"
    echo "Check AAFONT environment variable"
    exit
} >&2

die() {
    echo "FAIL! Status: $?"
    exit
} >&2

echo "Using font '$FONT'"
echo "Running DIFF $DIFFOPTS '$S' -> '$D' ..."
${P}/testdiff $DIFFOPTS "$S" "$D" || die
echo "Running CONV $CONVOPTS '$D' -> '$T' ..."
${P}/testconv $CONVOPTS "$D" "$FONT" "$T" || die
echo "Running RENDER '$T' -> '$O' ..."
${P}/testrender "$T" "$FONT" "$O" || die
echo "Done"
