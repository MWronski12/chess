1) Only plain ASCII characters are allowed.
2) Everything should be in english.
3) File names should be adequate to its content.
4) File names should be capitalized.
5) Source code files should have .cc extension.
6) Header files should have .h extension.
7) Header files containing both declaration and definition should have .hpp extension.
8) Every source file and header file should start with a block comment containing author data and dile description.
9) Every non-trivial class and method should be preceeded with a block comment.
10) Hard to understand code fragments should be avoided or at elast well commented if necessary.
11) Class, structs, enums and typedefs should be named using capitalized words (ex. MyClass, MyEnum...).
12) Function/method and class member names should be named using camel case naming convention (ex. myFunc, myMethod...).
13) Function/method parameters and local variables should be named using camel case naming convention (ex. myFuncParam, myLocalVar...).
14) Enum elements and names of constants should be written only with capital letters and every word should be separatad with underscore '_' character (ex. MY_CONSTANT, MU_ENUM_EL...).
15) Private class members should start with underscore '_' and use camel case naming convention (ex. _myPrivateMember).
16) Library headers should be included first and project headers after.
17) 'std' namespace should be included in source files, and shouldn't be included in header files.


Function and class block comment template:

/**
 * Brief description of the function.
 * 
 * @param param1 Description of the first parameter.
 * @param param2 Description of the second parameter.
 * @return Description of the return value.
 * @throw Description of the exception thrown (if any).
 * 
 * Detailed description of the function, including any assumptions made,
 * potential side effects, and any other relevant information.
 * 
 * Examples of usage:
 * <code>
 * int result = myFunction(arg1, arg2);
 * </code>
 */
