yarn build:native
APPNAME=$1
rm -rf $APPNAME.app
./macapp.sh $APPNAME icon.png
DEST=$APPNAME.app/Contents/MacOS
cp ../../lib/bs/native/mainmacos.native $DEST/$APPNAME
cp -r ../assets $APPNAME.app/Contents/MacOS/
zip -r $APPNAME.zip $APPNAME.app
