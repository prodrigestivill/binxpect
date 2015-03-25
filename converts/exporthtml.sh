#!/bin/bash
if [ $# -lt 2 ]; then
cat << EOF > /dev/stderr
 Usage: $0 [html] [chat out]
 Convert exported HTML conversation log to a plaintext format.
 Input HTML log are from HHD Software Serial Monitor free edition.
 Web-site: http://www.serial-port-monitor.com/free-serial-port-monitor-product-details.html

EOF
	exit 1
fi
HTML=$1
OUT=$2
TMPFILE=$(mktemp)

cd `dirname $0`
xsltproc --html export.xsl "$HTML" > "$TMPFILE"
awk 'BEGIN{print "TIMEOUT 10"; print "SPEED 57600"}{if (length($0)>50) $0 = substr($0, 0, 50); gsub(/\s+/, "", $0); if (length($0)>0) print}' "$TMPFILE" > "$OUT"
rm -f "$TMPFILE"
