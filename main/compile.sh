assert_file() {
	if [ ! -e "$1/$2" ] ; then
		echo Can not compiled library: "$2"
		return 1
	fi
}

wait_to_press_enter() {
	read -p "Press <Enter> to continue..."
}

prog_name="PasswordSaver.exe"
icon="icon.o"
src_dir="./src/*.c"

log2database_name="log2database.dll"
log2database_dir="../log2database"

sha256generator_name="sha256generator.dll"
sha256generator_dir="../sha256generator"

strconverter_name="strconverter.dll"
strconverter_dir="../strconverter"

if ! assert_file $log2database_dir $log2database_name ; then
	wait_to_press_enter
	exit 1
fi

if ! assert_file $sha256generator_dir $sha256generator_name ; then
	wait_to_press_enter
	exit 1
fi

if ! assert_file $strconverter_dir $strconverter_name ; then
	wait_to_press_enter
	exit 1
fi

cp "$log2database_dir/$log2database_name" "./$log2database_name"
cp "$sha256generator_dir/$sha256generator_name" "./$sha256generator_name"
cp "$strconverter_dir/$strconverter_name" "./$strconverter_name"

gcc -I"$log2database_dir/include" -I"$sha256generator_dir/include" -I"$strconverter_dir/include" -I"./include" $src_dir $icon -o $prog_name -L. -llog2database -L. -lsha256generator -L. -lstrconverter -std=c11 -O2