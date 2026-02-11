wait_to_press_enter(){
	read -p "Press <Enter> to continue..."
}

test_py_file="tests.py"
lib_name="strconverter.dll"

cd ..

if ./compile.sh -f ; then
	python tests/$test_py_file
	wait_to_press_enter

else
	echo Compiling errors...
	wait_to_press_enter

	exit 1
fi