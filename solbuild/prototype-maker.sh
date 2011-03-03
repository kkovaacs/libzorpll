#!/bin/sh
find opt/ -exec chown root:bin \{\} \;
# strip the runtime libraries
find opt/zorp/lib/ -type f -name *.so* -exec strip \{\} \;
LIBPROTOTYPE="../prototype.lib"
echo "i pkginfo" > $LIBPROTOTYPE
echo "i admin" >> $LIBPROTOTYPE
echo "d none opt/zorp 0755 root bin" >> $LIBPROTOTYPE
# /usr/bin/grep is a f*ing LAME program !!!
pkgproto -c library opt/zorp/lib | grep -v "opt/zorp/lib/pkgconfig/zorplibll.pc" | grep -v "opt/zorp/lib/libzorpll.a" | grep -v "opt/zorp/lib/libzorpll.la" >> $LIBPROTOTYPE

DEVPROTOTYPE="../prototype.dev"
echo "i pkginfo" > $DEVPROTOTYPE
echo "i admin" >> $DEVPROTOTYPE
echo "d none opt/zorp 0755 root bin" >> $DEVPROTOTYPE
pkgproto -c headers opt/zorp/include >> $DEVPROTOTYPE
pkgproto -c headers opt/zorp/lib/pkgconfig/zorplibll.pc >> $DEVPROTOTYPE
pkgproto -c headers opt/zorp/lib/libzorpll.a >> $DEVPROTOTYPE
pkgproto -c headers opt/zorp/lib/libzorpll.la >> $DEVPROTOTYPE
