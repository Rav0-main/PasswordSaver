<div>
<h1>Fast start</h1>
<p>Install the contents of the 'Compiled' directory and use it. To switch between input fields, use the Up arrow and the Down arrow:</p>
<img src=".\READMEdata\fastStart.png">
<p>Switching between screens is done by pressing the Left arrow key:</p>
<img src=".\READMEdata\switchScreens.png">
<p>For full instructions, read the 'Help'.</p>
</div>

<div>
<h1>How to compile?</h1>
<p>Compiling individual parts is automated by Python scripts, so a Python interpreter is required for building. The compiled project is located in the 'Compiled' directory</p>
<h2>1. Compile all libraries</h2>
<p>To compile each library: Log2Database, SHA256Generator, SimpleEncryptionOfString, use the Python script 'compilelib.py'</p>
<p>Compiling example:</p>
<p>The directory where the library is being created:</p>
<img src=".\READMEdata\howToCompileLibDirectory.png">
<p>Next, compile the script in the console:</p>
<img src=".\READMEdata\howToCompileLibConsole.png">
<p>In my case, the compiled libraries have the following names: Log2Database - log2database.dll, SHA256Generator - sha256generator.dll, and SimpleEncryptionOfString - strconverter.dll</p>
<p>Received libraries:</p>
<img src=".\READMEdata\howToCompileAllLibs.png">
<h2>2. Building a project</h2>
<p>Go to the 'Main' directory and run the Python script with the corresponding template: 'python compile.py PROGRAM_NAME.exe FIRST_LIB_DIR FIRST_LIB_NAME SECOND_LIB_DIR SECOND_LIB_NAME ...':</p>
<img src=".\READMEdata\howToCompileBuilding.png">
<p>If your names match mine and you have moved the libraries to the 'Main' directory, you can use the create.py Python script:</p>
<img src=".\READMEdata\howToCompileMyselfBuilding.png">
<p>After successful completion, a program with the name PROGRAM_NAME.exe (in my case, PasswordSaver.exe) should appear</p>
</div>

<div>
<h1>How to use?(For more - check 'Help' screen)</h1>
<h2>1. Start screens</h2>
<p>Initial screens such as 'Sign in', 'Register', and 'Help' perform their respective functions
</p>
<h2>2. Main screens</h2>
<p>After creating an account and logging in, you will see this screen:</p>
<img src=".\READMEdata\useMenuScreen.png">
<p>Each screen performs its corresponding functionality.</p>
<h3>3.0. Features of each screen</h3>
<p>Below are the features of each screen.</p>
<h3>3.1 'Create record' screen</h3>
<p>Appearance:</p>
<img src=".\READMEdata\useCreateRecordScreen.png">
<p>Required input fields: 'Record name', 'Login', and 'Password'. If the 'Description' field is not filled in, a null string is entered</p>
<h3>3.2 'Find record' screen</h3>
<p>Appearance:</p>
<img src=".\READMEdata\useFindRecordScreen.png">
<p>This field has an autocomplete feature: when the input is empty, you can select a record from all of them, and when the input is specific, only those whose beginning matches the input.</p>
<p>You can switch between hints using the Up and Down arrows.</p>
<p>To retrieve data, press 'Enter'. The output will contain information about the currently visible record</p>
<h3>3.3 'Output all records' screen</h3>
<p>Displays all entries in alphabetical order.</p>
<h3>3.4 'Change record' screen</h3>
<p>The first subscreen:</p>
<img src=".\READMEdata\useFindRecordScreen.png">
<p>It has the same introductory functionality as 'Find record' screen.</p>
<p>After successful input, it shows the second subscreen:</p>
<img src=".\READMEdata\useChangeRecordScreen.png">
<h3>3.5 'Delete record' screen</h3>
<p>Appearance:</p>
<img src=".\READMEdata\useFindRecordScreen.png">
<p>It has the same introductory functionality as 'Find record' screen.</p>
<p>Deletes the entered entry when entering.</p>
<h3>3.6 'Delete account' screen</h3>
<p>Appearance:</p>
<img src=".\READMEdata\useDeleteAccountScreen.png">
<p>Contains two fields: 'Delete' and 'Cancel'. Each performs a corresponding function.</p>
</div>

<div>
<p>Thanks for read!</p>
</div>