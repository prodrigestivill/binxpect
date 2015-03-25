#!/bin/sh
if [ $# -lt 2 ]; then
cat << EOF > /dev/stderr
 Usage: $0 [chat in] [chat out]
 Invert chat conversion

EOF
	exit 1
fi
IN=$1
OUT=$2

sed -e 's/SEND/TMPCMDSND/g' -e 's/EXPECT/SEND/g' -e 's/TMPCMDSND/EXPECT/g' "$IN" > "$OUT"
