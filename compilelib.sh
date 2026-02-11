wait_to_press_enter(){
	if [ "$1" != "$TO_SKIP_WAITING" ] ; then
		read -p "Press <Enter> to continue..."
	fi
}

TO_SKIP_WAITING="-f"
include_dir="include/"
src_c_files="src/*.c"
src_o_files="*.o"

if [ "$#" == "1" ] ; then
	lib_name="$1"
else
	lib_name="$2"
fi

if gcc -I$include_dir -c -fPIC $src_c_files -std=c11 -O2; then
	echo Success compiling object files.

else
	echo Compiling errors...

	wait_to_press_enter "$1"
	exit 1
fi

if gcc -shared -o $lib_name $src_o_files ; then
	echo Success compiling library.

else
	echo Compiling errors...

	wait_to_press_enter "$1"
	exit 1
fi

if rm -f $src_o_files ; then
	echo Success removed object files.

else
	echo Removing errors...

	wait_to_press_enter "$1"
	exit 1
fi

wait_to_press_enter "$1"
exit 0