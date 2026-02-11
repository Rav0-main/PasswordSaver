TO_SKIP_WAITING="-f"

lib_name="sha256generator.dll"

if [ "$1" == "$TO_SKIP_WAITING" ] ; then
	../compilelib.sh -f $lib_name
else 
	../compilelib.sh $lib_name
fi