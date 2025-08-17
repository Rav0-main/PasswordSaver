#include "helpscreen.h"
#include "generalscreenfuncts.h"
#include "screencodes.h"
#include <stdio.h>

void runHelpScreen(int* currentScreen) {
	printf("This is a 'Password Saver' app\n");
	printf("It is needed to save account data\n");
	printf("\n");

	printf("1)Keyboard guide\n");
	printf("----------------------------------------------------------------\n");
	printf("- Characters are entered using the keyboard.\n");
	printf("\n");
	printf("- There are screens where you only need to\n");
	printf("  select an option instead of entering it\n");
	printf("  You can switch between options using\n");
	printf("  the up and down arrow buttons\n");
	printf("  To select an option, press the Enter key.\n");
	printf("\n");
	printf("- When entering data, pressing Enter on\n");
	printf("  the last input line completes the input.\n");
	printf("\n");
	printf("- When entering data in an autocomplete field,\n");
	printf("  you can switch between the autocomplete rows\n");
	printf("  using the up and down arrows.\n");
	printf("\n");
	printf("- Pressing Enter in the autocomplete field\n");
	printf("  will enter the current word being displayed.\n");
	printf("\n");
	printf("- Switching between screens is done using\n");
	printf("  the left arrow button.\n");
	printf("\n");
	printf("- Some input fields are protected.\n");
	printf("  To see them, press the Shift + ` keyboard shortcut.\n");
	printf("\n");

	printf("2)Manipulations with account data\n");
	printf("----------------------------------------------------------------\n");
	printf("- Data is stored in strict lexicographic order,\n");
	printf("  so information about the order of addition\n");
	printf("  may be lost.\n");
	printf("\n");

	printf("- All data is encrypted based on the entered\n");
	printf("  password, so the stronger the password,\n");
	printf("  the more difficult it is to extract the data.\n");
	printf("\n");

	printf("3)Rules of use\n");
	printf("----------------------------------------------------------------\n");
	printf("- When you manually delete a specific file,\n");
	printf("  you may lose data or cause the program\n");
	printf("  to malfunction.\n");
	printf("\n");

	printf("4)Launch from terminal\n");
	printf("----------------------------------------------------------------\n");
	printf("- You can log in to your account from the terminal\n");
	printf("  by typing 'passwordsaver _login_  _password_'\n");
	printf("\n");

	printf("5)Thanks\n");
	printf("----------------------------------------------------------------\n");
	printf("- Thanks for using!\n");
	printf("\n");
	printf("- Good luck in life!\n");
	printf("\n");
	
	*currentScreen = INPUT_SCREEN;
	showToPressEnter();
}