yarn build:native
rm -rf Pure.app
./macapp.sh Pure icon.png
DEST=Pure.app/Contents/MacOS
cp ../../lib/bs/native/mainmacos.native $DEST/Pure
# zip -r Gravitron.zip Gravitron.app
