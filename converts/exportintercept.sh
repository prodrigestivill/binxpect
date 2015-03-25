#!/bin/bash
if [ $# -lt 2 ]; then
cat << EOF > /dev/stderr
 Usage: $0 [interceptty] [binxpect out]
 Convert intercepted tty conversation log to a binxpect format.
 Web-site: http://www.suspectclass.com/sgifford/interceptty/

EOF
	exit 1
fi
INT=$1
OUT=$2

cd `dirname $0`
awk -f exportint.awk "$INT" > "$OUT"
