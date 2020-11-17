#!/bin/sh
dist="dist"
output=$dist"/data.h"
includes="#include <Arduino.h>\n#include <PgmSpace.h>"
licence="licence.txt"
html=$dist"/index.html"
script=$dist"/app.js"
icon_hi=$dist"/images/icon_hi_res.png"
icon_lo=$dist"/images/icon_lo_res.png"

cat $licence > $output
echo $includes >> $output

echo "PROGMEM" >> $output
xxd -i $html >> $output

echo "PROGMEM" >> $output
xxd -i $script >> $output

echo "PROGMEM" >> $output
xxd -i $icon_lo >> $output

echo "PROGMEM" >> $output
xxd -i $icon_hi >> $output

# Replace unsigned by const
sed -i.bak 's/unsigned/const/g' $output

# Clean
find $dist \! -name '*.h' -maxdepth 1 -type f -delete
rm -rf $dist/images
