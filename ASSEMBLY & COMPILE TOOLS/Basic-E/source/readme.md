/manual.txt = original

>------!------!------!------!------!------!------!------!----------------------R
>TM 3
>BM 3
>HM 3
>FM 1
>PL 66
>SM 10
>RJ ON
>CP ON
>HE Stross/BASIC-E MANUAL/%
>LS 1
>FO____________________________________________________________________________
å








                            BASIC-E REFERENCE MANUAL







                            MICRO-COMPUTER LABORATORY

                            NAVAL POSTGRADUATE SCHOOL

                              MONTEREY, CALIFORNIA







                             GORDON E. EUBANKS, JR.



                                15 DECEMBER 1976























        TABLE OF CONTENTS

        I.        INTRODUCTION                                        3

        II.       DESCRIPTION OF THE BASIC-E LANGUAGE                 4

        III.      OPERATNG INSTRUCTIONS FOR BASIC-E                  63

        APPENDIX A - COMPILER ERROR MESSAGES                         68

        APPENDIX B - RUN TIME MONITOR ERROR MESSAGES                 69

        APPENDIX C - BASIC-E GRAMMAR                                 70

        APPENDIX D - COMPILER OPTIONS                                75

        BILIOGRAPHY                                                  76

>PA

        I. INTRODUCTION

        This manual describes the naval Postgraduate School BASIC language 
Š        (BASIC-E). BASIC-E is an extension of the BASIC language for the 
Š        8080 microprocessor. it consists of two subsystems, the compiler 
Š        and the run-time monitor. The compiler syntax checks a BASIC-E 
Š        source program producing an object file which may then be executed 
Š        by the run-time monitor.

        BASIC-E is intended to be used in the interactive mode with a CRT 
Š        or teletype terminal. It includes most features of the proposed 
Š        ANSI standard BASIC (1) as well as extensive string manipulation 
Š        and file input/output capabilities. BASIC-E requires a resident 
Š        disk operating system to handle input/output operations and disk 
Š        file management. The source program is assumed to reside on the 
Š        disk.

        The BASIC-E compiler consists of a table-driven parser which checks 
Š        statements for correct syntax and generates code for the BASIC-E 
Š        machine. The code is executed by the run-time monitor. The machine 
Š        is a zero address stack computer. Floating point arithmetic numbers 
Š        are represented in 32 bits with an 98 bit exponent, one bit sign 
Š        and twenty-four bits of fraction. This provides slightly more than 
Š        seven decimal digits of significance. Variable length strings and 
Š        dimensional arrays are both dynamically allocated.

        Section II of this manual describes the language elements. Section 
Š        III provides operating instructions for BASIC-E. The appendices 
Š        list compiler and run-time error messages, the formal grammar for 
Š        BASIC-E, and compiler toggles. References are included in the 
Š        bibliography.

>PA

        II. DESCRIPTION OF THE BASIC-E LANGUAGE

        Elements of BASIC-E are listed in alphabetical order in this 
Š        section of the manual. A synopsis of the element is shown, followed 
Š        by a description and examples of its use. The intent is to provide 
Š        a reference to the feature of this implementation of BASIC and not 
Š        to teach the BASIC language. References (2,3) provide a good 
Š        introduction to BASIC programming.

        A program consists of one or more properly formed BASIC-E 
Š        statements. An END statement, if present, terminates the program 
Š        and additional statements are ignored. The entire ASCII character 
Š        set is accepted, but all statements may be written using the common 
Š        64-character subset. Section III provides information on source 
Š        program format.

        In this section the synopsis presents the general form of the 
Š        element. Square brackets [] denote an optional feature while braces 
Š        () indicate that the enclosed section may be repeated zero or more 
Š        times. Terms enclosed in <> are either non-terminal elements of the 
Š        language, which are further defined in this section, or terminal 
Š        symbols. All special characters and capitalized words are terminal 
Š        symbols.

   
>PA
        ELEMENT:                                                      ABS

             ABS predefined function

        SYNOPSIS:

             ABS (<expression>)

        DESCRIPTION:

             The ABS function returns the absolute value of the 
Š        <expression>. The argument must evaluate to a floating point 
Š        number.

        EXAMPLES:

             ABS (X)
             ABS (X*Y)
>PA
        ELEMENT:                                                      ASC

             ASC predefined function

        SYNOPSIS:

             ASC (<expression>)

        DESCRIPTION:

        The ASC function returns the ASCII numeric value of the 
Š        first character of the <expression>. The argument must 
Š        evaluate to a string. If the length of the string is 0 
Š        (null string) an error will occur.

        EXAMPLES:

             ASC (A$)

             ASC ("X")

             ASC (RIGHT$(A$,7))
>PA
        ELEMENT:                                               ATN

             ATN predefined function

        SYNOPSIS:

             ATN (<expression>)

        DESCRIPTION:

        The ATN function returns the arctangent of the 
Š        <expression>. The argument must evaluate to a floating 
Š        point number.

        EXAMPLES:

             ATN (X)

             ATN (SQR(SIN(X)))

        PROGRAMMING NOTE:

        All other inverse trigonometric functions may be computed 
Š        from the arctangent using simple identities.

>PA
        ELEMENT:                                               CHR$

             CHR$ predefined function

        SYNOPSIS:

        CHR$ (<expression>)

        DESCRIPTION:

        The CHR$ function returns a character string of length 1 
Š        consisting of the character whose ASCII equivalent is the 
Š        <expression> converted to an integer modulo 128. The 
Š        argument must evaluate to a floating point number.

        EXAMPLES:

             CHR$ (A)

             CHR$ (12)

             CHR$ ((A+B/C)*SIN(X))

        PROGRAMMING NOTE:

        CHR$ can be used to send control characters such as a 
Š        linefeed to the output device. The following statement 
Š        would accomplish this:

                  PRINT CHR$(10)
>PA
        ELEMENT:                                               CLOSE

             CLOSE statement

        SYNOPSIS:

             [<line number>] CLOSE expression {,<expression>}

        DESCRIPTION:

        The CLOSE statement causes the file specified by each 
Š        <expression> to be closed. Before the file may be 
Š        referenced again it must be reopened using a FILE 
Š        statement.

        An error occurs if the specified file has not previously 
Š        appeared in a FILE statement.

        EXAMPLES:

             CLOSE 1

             150 CLOSE I,K, L*M-N

        PROGRAMMING NOTE:

        On normal completion of a program all open files are 
Š        closed. If the program terminates abnormally it is 
Š        possible that files created by the program will be lost.
>PA
        ELEMENT:                                          <constant>

             <constant>

        SYNOPSIS:

             [<sign>] <integer>. [<integer>] [E <sign><exp>]
             ["] <character string> ["]

        DESCRIPTION:

        A <constant> may be either a numeric constant or a string 
Š        constant. All numeric constants are stored as floating 
Š        point numbers. Strings may contain any ASCII character.

        Numeric constants may be either a signed or unsigned 
Š        integer, decimal number, or expressed in scientific 
Š        notation. Numbers up to 31 characters  in length are 
Š        accepted but the floating point representation of the 
Š        number maintains approximately seven significant digits 
Š        (1 part in 16,000,000). The largest magnitude that can be 
Š        represented is approximately 2.7 times ten to the minus 
Š        39th power.


        String constants may be up to 255 characters in length. 
Š        Strings entered from the console, in a data statement, or 
Š        read from a disk file may be either enclosed in quotation 
Š        marks or delimited by a comma. Strings used as constants 
Š        in the program must be enclosed in quotation marks.

        EXAMPLES:

             10

             -100.75639E-19

             "THIS IS THE ANSWER"

>PA
        ELEMENT:                                               COS

             COS predefined function

        SYNOPSIS:

             COS (<expression>)

        DESCRIPTION:

        COS is a function which returns the cosine of the 
Š        <expression>. The argument must evaluate to a floating 
Š        point number expressed in radians.

        A floating point overflow occurs if the absolute value of 
Š        the <expression> is greater than two raised to the 24th 
Š        power times pi radians.

        EXAMPLES:

             COS(B)

             COS(SQR(X-Y))

>PA
        ELEMENT:                                               COSH

             COSH predefined function

        SYNOPSIS:

             COSH (<expression>)

        DESCRIPTION:

             COSH is a function which returns the hyperbolic cosine of 
Š        the <expression>. The argument must evaluate to a 
Š        floating point number.

        EXAMPLES:

             COSH(X)

             COSH(X^2+Y^2)
>PA

        ELEMENT:                                               DATA

             DATA statement

        SYNOPSIS:

             [<line number>] DATA constant {,<constant>}

        DESCRIPTION:

        DATA statements define string and floating point 
Š        constants which are assigned to variables using a READ 
Š        statement. Any number of DATA statements may occur in a 
Š        program. The constants are stored consecutively in a data 
Š        area as they appear in the program and are not syntax 
Š        checked by the compiler. Strings maybe enclosed in 
Š        quotation marks or optionally delimited by commas.

        EXAMPLES:

             10 DATA 10.0,11.72,100
             DATA "XYZ",11.,THIS IS A STRING
>PA
   
        ELEMENT:                                               DEF

             DEF statement

        SYNOPSIS:

             [<line number>] DEF <function name> (<dummy argument 
                                 list>) = <expression>

        DESCRIPTION:

        The DEF statement specifies a user defined function which 
Š        returns a value of the same type as the<function name>. 
Š        One or more <expressions> are passed to the function and 
Š        used in evaluating the expression. The passed values may 
Š        be in string or floating point form but must match the 
Š        type of the corresponding dummy argument. Recursive calls 
Š        are not permitted.

        The <expression> in the define statement may reference 
Š        <variables> other than the dummy arguments, in which case 
Š        the current value of the <variable> is used in evaluating 
Š        the <expression>. The type of the function must match the 
Š        type of the <expression>.

        EXAMPLES:

             10 DEF FNA(X,Y) = X + Y - A
             DEF FNB$(A$,B$) = A$ + B$ + C$
             DEF FN.COMPUTE (A,B) = A + B - FNA(A,B) + D
>PA
        ELEMENT:                                               DIM

             DIM statement

        SYNOPSIS:

             [line number] DIM <identifier> (<subscript list>) 
                            {,<identifier> (<subscript list>)}

        DESCRIPTION:

        The dimension statement dynamically allocates space for 
Š        floating point or string arrays. String array elements 
Š        may be of any length up to 255 bytes and change in length 
Š        dynamically as they assume different values. Initially, 
Š        all floating point arrays are set to zero and all string 
Š        arrays are null strings. An array must be dimensioned 
Š        explicity; no default options are provided. Arrays are 
Š        stored in row major order.

        Expressions in subscript lists are evaluated as floating 
Š        point numbers and rounded to the earnest integer when 
Š        determining the size of the array. All subscripts have an 
Š        implied lower bound of 0.

        When array elements are reference a check is make to 
Š        ensure the element resides in the reference array.

        EXAMPLES:

             DIM A(10,20), B(10)

             DIM B$(2,5,10),C(I + 7.3,N),D(I)

             DIM X(A(I),M,N)

        PROGRAMMING NOTE:

        A <DIM statement> is an executable statement, and each 
Š        execution will allocate a new array.

>PA
        ELEMENT:                                               END

             END statement

        SYNOPSIS:

             [line number] END

        DESCRIPTION:

        An END statement indicates the end of the source program. 
Š        It is optional and, if present, it terminates reading of 
Š        the source program. If any statements follow the END 
Š        statement they are ignored.

        EXAMPLES:

             10 END
             END
>PA

        ELEMENT:                                          <expression>

             <expression>

        DESCRIPTION:

        Expressions consist of algebra in combinations of 
Š        variables, constants, and operators. The heirarchy of 
Š        operators is:

             1) ()
             2) ^
             3) *,/
             4) +,-, concat (+), unary +, unary -
             5) relational ops <,<=,>, =,=,<> LT, LE, GT, GE, EQ, NE
             6) NOT
             7) AND
             8) OR, XOR

        Relational operators result in a 0 if false and -1 if 
Š        true, NOT, and AND, and OR are performed on 32 bit two's 
Š        complement binary representation of the integer portion 
Š        of the variable. The result is then converted to a 
Š        floating point number. String variables may be operated 
Š        on by relational operators and concatenation only. Mixed 
Š        string and numeric operations are not permitted.

        EXAMPLES:

             X + Y

             A$ + B$

             (A <= B) OR (C$> D$) / (A - B AND D)
>PA
        ELEMENT:                                               FILE

             FILE statement

        SYNOPSIS:

             [<line number>] FILE <variable>] FILE <variable> [(<expression>)] 
Š                            {,<variable>[(<expression>)]}

        DESCRIPTION:

        A file statement opens files used by the program. The 
Š        order of the names determines the numbers used to 
Š        reference the files in READ and PRINT statements. The 
Š        value assigned to the first simple variable is file 1, 
Š        the second is file 2, and so forth. There may be any 
Š        number of FILE statement in a program, but there is a 
Š        limit to the number of files which may be opened at one 
Š        time. Currently this limit is set at 6 files.


        The optional <expression> designates the logical record 
Š        length of the file. If no length is specified, the file 
Š        is written as a continuous string of fields with carriage 
Š        return linefeed characters separating each record. If the 
Š        record length is present, a carriage return linefeed will 
Š        be appended to each record. The <variable> must not be 
Š        subscripted and it must be of type string.

        EXAMPLES:

             FILE INPUT$, OUTPUT$

             FILE TABLE.INC$, TAX.INC$(160), PAY.AMT.DAY$(N*3-J)

        PROGRAMMING NOTE:

        The run-time monitor will always assign the lowest 
Š        available (not previously assigned) number to the file 
Š        being opened. this if files are closed and others opened 
Š        it is possible that number assignment may vary with 
Š        program flow.

>PA
        ELEMENT:                                               FOR

             FOR statement

        SYNOPSIS:

             [<line number>] FOR <index> = <expression> TO 
                                 <expression> [STEP <expression>]

        DESCRIPTION:

        Execution of all statements between the FOR statement and 
Š        its corresponding NEXT statement is repeated until the 
Š        indexing variable, which is incremented by the STEP 
Š        <expression> after each iteration, reaches the exit 
Š        criteria. If the step is positive, the loop exit criteria 
Š        is that the index exceeds the value of the TO 
Š        <expression>. If the step is negative, the index must be 
Š        less than the TO <expression> for the exit criteria to be 
Š        met.

        The <index> must be an unsubscripted variable and is 
Š        initially set to the value of the first <expression>. 
Š        Both the TO and STEP expressions are evaluated on each 
Š        loop, and all variables associated with the FOR statement 
Š        may change within the loop. If the STEP clause is 
Š        omitted, a default value of 1 is assumed. a FOR loop is 
Š        always executed at least once. A step of 0 may be used to 
Š        loop indefinitely.

        EXAMPLES:

             FOR I = 1 TO 10 STEP 3

             FOR INDEX = J*K-L TO 10*SIN(X)

             FOR I = 1 TO 2 STEP 0

        PROGRAMMING NOTE:

        If a step of 1 is desired the step clause should be 
Š        omitted The execution will be substantially faster since 
Š        less run time checks must be made.
>PA
        ELEMENT:                                               FRE

             FRE predefined function

        SYNOPSIS:

             FRE

        DESCRIPTION:

        The FRE function returns the number of bytes of unused 
Š        space in the free storage area.

        EXAMPLE:

             FRE
>PA
        FUNCTION:                                    <function name>

             <function name>

        SYNOPSIS:

             FN<identifier>

        DESCRIPTION:

             Any <identifier> starting with FN refers to a user 
Š        defined function. The <function name> must appear in a 
Š        DEF statement prior to being used in an <expression>. 
Š        There may not be any spaces between the FN and the 
Š        <identifier>.

        EXAMPLES:

             FNA

             FN.BIGGER.$
>PA

        ELEMENT:                                               GOSUB

             GOSUB statement

        SYNOPSIS:

             [<line number>] GOSUB <line number>
             [<line number>] GO SUB<line number>

        DESCRIPTION:

        The address of the next sequential instruction is saved 
Š        on the run-time stack, and control is transferred to the 
Š        subroutine labelled with the <line number> following the 
Š        GOSUB or GO SUB.

        EXAMPLES:

             10 GOSUB 300

             GO SUB 100

        PROGRAMMING NOTE:

        The max depth of GOSUB calls allowed is controlled by the 
Š        size of the run-time stack which is currently set at 12.
>PA

        ELEMENT:                                               GOTO

             GOTO statement

        SYNOPSIS:

             [<line number>] GOTO <line number>
             [<line number>] GO TO<line number>

        DESCRIPTION:

        Execution continues at the statement labelled with the 
Š        <line number> following the GOTO or GO TO.

        EXAMPLES:

             100 GOTO 50

             GO TO 10

>PA

        ELEMENT:                                          <identifier>

             <identifier>

        SYNOPSIS:

             <letter> {<letter> or <number> or.} ($)

        DESCRIPTION:

        An identifier begins with an alphabetic character 
Š        followed by any number of alphanumeric characters, or 
Š        periods. Only the first 31 characters are considered 
Š        unique. If the last character is a dollar sign the 
Š        associated variable is of type string. otherwise it is of 
Š        type floating point.

        EXAMPLES:

             A

             B$

             XYZ.ABC

             PAY.RECORD.FILE.NUMBER.76

        PROGRAMMING NOTE:

        All lowercase letters appearing in an <identifier> are 
Š        converted to uppercase unless compiler toggle D is set to 
Š        off.
>PA
        ELEMENT:                                               IF

             IF statement

        SYNOPSIS:

             [<line number>] IF <expression> THEN <line number>
             [<line number>] IF <expression> THEN <statement list>
             [<line number>] IF <expression> THEN <statement list> 
                                             ELSE <statement list>

        DESCRIPTION:

        If the value of the <expression> is not 0 the statements 
Š        which make up the <statement list> are executed. 
Š        Otherwise the <statement list> following the ELSE is 
Š        executed, if present, or the next sequential statement is 
Š        executed.

        In the first form of the statement if the <expression> is 
Š        not equal to 0, an unconditional branch to the label 
Š        occurs.

        EXAMPLES:

             IF A$ B$ THEN X=Y*Z

             IF (A$<B$) AND (C OR D) THEN 300

             IF B THEN X=3.0 : GOTO 200

             IF J AND K THEN GOTO 11 ELSE GOTO 12
>PA

        ELEMENT:                                               IF END

             IF END statement

        SYNOPSIS:

             [<line number>] IF END #<expression> THEN <line number>

        DESCRIPTION:

        If during a read to the file specified by the 
Š        <expression>, an end of file is detected control is 
Š        transferred to the statement labelled with the line 
Š        number following the THEN.

        EXAMPLES:

             IF END # 1 THEN 100
             10 IF END # FILE.NUMBER - INDEX THEN 700


        PROGRAMMING NOTE:

        On transfer to the line number followingthe THEN the 
Š        stack is restored to the state prior to the execution of 
Š        the READ statement which caused the end of file 
Š        condition.
>PA
        ELEMENT:                                               INP

             INP predefined function

        SYNOPSIS:

             INP (<expression>)

        DESCRIPTION:

        The INP function performs an input operation on the 8080 
Š        machine port represented by the value of the <expression> 
Š        modulo 256 returning the resulting value. The argument 
Š        must evaluate to a floating point number.

        EXAMPLES:

             INP (2)

             INP (CURRENT.INPUT.PORT)
>PA
   
        ELEMENT:                                               INPUT

             INPUT statement

        SYNOPSIS:

             [<line number>] INPUT [<prompt string>;]
             <variable> (,<variable>

        DESCRIPTION:

        The <prompt string>, if present, is printed on the 
Š        console. A line of input data is read from the console 
Š        and assigned to the variables as they appear in the 
Š        variable list. The data items are separated by commas 
Š        and/or blanks and terminated by a carriage return. 
Š        Strings may be enclosed in quotation marks. If a string 
Š        is not enclosed by quotes, the first comma terminates the 
Š        string. If more data is requested than was entered, or if 
Š        insufficient data items is entered, a warning is printed 
Š        on the console and the entire line must be reentered.

        EXAMPLES:

             10 INPUT A,B

             INPUT "SIZE OF ARRAY?"; N
             
             INPUT "VALUES?"; A(I),B(I),C(A(I))

        PROGRAMMING NOTE:

        Trailing blanks in the <prompt string> are ignored. One 
Š        blank is always supplied by the system.
>PA
        ELEMENT:                                               INT

             INT predefined function

        SYNOPSIS:

             INT (expression)

        DESCRIPTION:

        The INT function returns the largest integer less than or 
Š        equal to the value of the <expression>. The argument must 
Š        evaluate to a floating point point number.

        EXAMPLES:

             INT (AMOUNT / 100)

             INT (3^8/X*8*SIN(Y))
>PA
        ELEMENT:                                               LEFT$

             LEFT$ predefined function

        SYNOPSIS:

             LEFT$ (<expression>,<expression>)

        DESCRIPTION:

        The LEFT$ function returns the n leftmost characters of 
Š        the first <expression>, where n is equal to the integer 
Š        portion of the second <expression>. An error occurs if n 
Š        is negative. If n is greater than the length of the first 
Š        <expression> then the entire expression is returned. The 
Š        first argument must evaluate to a string and the second 
Š        to a floating point number.

        EXAMPLES:

             LEFT$ (A$,3)

             LEFT$(CS*DS,I-J)
>PA
        ELEMENT:                                               LEN

             LEN predefined function

        SYNOPSIS:

             LEN (<expression>)

        DESCRIPTION:

        The LEN function returns the length of the string 
Š        <expression> passed as an argument. Zero is returned if 
Š        the argument is the null string.

        EXAMPLES:

             LEN (A$)

             LEN(CS + B$)

             LEN (LASTNAME$ + "," + FIRSTNAME$)
>PA
        ELEMENT:                                               LET

             LET statement

        SYNOPSIS:

             [<line number>] [LET] <variable> = <expression>

        DESCRIPTION:

        The <expression> is evaluated and assigned to the 
Š        <variable> appearing on the left side of the equal sign. 
Š        The type of the <expression>, either floating point or 
Š        string, must match the type of the <variable>.

        EXAMPLES:

             100 LET A = B + C
             
             X(3,A) = 7.32 * Y = X(2,3)

             73 W = (A<B) OR (C$>D$)

             AMOUNT$ = DOLLARS$ + "." + CENTS$
>PA
        ELEMENT:                                          <line number>

             <line number>

        SYNOPSIS:

             <digit> {<digit>}

        DESCRIPTION:

        <line numbers> are optional on all statementsand are 
Š        ignored by the compiler except when they appear in a 
Š        GOTO, GOSUB, or ON statement. In these cases, the <line 
Š        number> must appear as the label of one and only one 
Š        <statement> in the program.

        <line numbers> may contain any number of digits but only 
Š        the first 31 are considered significant by the compiler.

        EXAMPLES:

             100

             4635276353
>PA
        ELEMENT:                                               LOG

             LOG predefined function

        SYNOPSIS:

             LOG (<expression>)

        DESCRIPTION:

        The LOG function returns the natural logarithm of the 
Š        absolute value of the <expression>. The argument must 
Š        evaluate to a non-zero floating point number.

        EXAMPLES:

             LOG (X)

             LOG((A + B)/D)

             LOG10 = LOG(X)/LOG(10)
>PA

        ELEMENT:                                               MID$

             MID$ predefined function

        SYNOPSIS:

             MID$ (<expression>,<expression>,<expression>)

        DESCRIPTION:

        The MID$ function returns a string consisting of the n 
Š        characters of the first <expression> starting at the mth 
Š        character. The value of m is equal to the integer portion 
Š        of the second <expression> while n is the integer portion 
Š        of the third <expression>.

        The first argument must evaluate to a string, and the 
Š        second and third arguments must be floating point 
Š        numbers. If m is greater than the length of the first 
Š        <expression> a null string is returned. If n is greater 
Š        than the number of character are returned. An error 
Š        occurs if m or n is negative.

        EXAMPLES:

             MID$(A$,I,J)

             MID$(BS+C$,START,LENGTH)
>PA
        ELEMENT:                                               ON

             ON statement

        SYNOPSIS:

             (1) [<line number>] ON <expression> GOTO 
                                 <line number> (, <line number>)

             (2) [<line number>] ON <expression> GO TO 
                                 <line number> (, <line number>)

             (3) [<line number>] ON <expression> GOSUB 
                                 <line number> (, <line number>)

             (4) [<line number>] ON <expression> GO SUB 
                                 <line number> (, <line number>)

        DESCRIPTION:

        The <expression>, rounded to the nearest integer value, 
Š        is used to select the <line number at which execution 
Š        will continue. If the <expression> evaluates to 1 the 
Š        first <line number> is selected and so forth. In the case 
Š        of an ON... GOSUB statement the address of the next 
Š        instruction becomes the return address.

        An error occurs if the <expression> after rounding is 
Š        less than one or greater than the number of <line 
Š        numbers> in the list.

        EXAMPLES:

             10 ON I GOTO 10, 20, 30, 40

             ON J*K-M GO SUB 10, 1, 1, 10
>PA
        ELEMENT:                                               NEXT

             NEXT statement

        SYNOPSIS:

             [<line number>] NEXT [<identifier> {<identifier>}]

        DESCRIPTION:

        A NEXT statement denotes the end of the closest unmatched 
Š        FOR statement. If the optional <identifier> is present if 
Š        must match the index variable of the FOR statement being 
Š        terminated. The list of <identifiers> allows matching 
Š        multiple FOR statements. The <line number> of a NEXT 
Š        statement may appear in an ON or GOTO statement, which 
Š        case execution of the FOR loop continues with the loop 
Š        variables assuming their current values.

        EXAMPLES:

             10 NEXT

             NEXT I
           
             NEXT I, J, K

>PA
        ELEMENT:                                               OUT

             OUT statement

        SYNOPSIS:

             [<line number>] OUT <expression>, <expression>

        DESCRIPTION:

        The low-order eight bits of the integer portion of the 
Š        second <expression> is sent to the 8080 machine output 
Š        port selected by the integer portion of the first 
Š        expression modulo 256. Both arguments must evaluate to 
Š        floating point numbers.

        EXAMPLES:

             100 OUT 3,10

             OUT PORT.NUM, NEXT.CHAR
>PA

        ELEMENT:                                               POS

             POS predefined function

        SYNOPSIS:

             POS

        DESCRIPTION:

             The POS function returns the current position of the 
Š             output line buffer pointer. This value will range from 1 
Š             to the print buffer size.

        EXAMPLE:

             PRINT TAB(POS + 3);X
>PA
        ELEMENT:                                               PRINT

             PRINT statement

        SYNOPSIS:

             (1) [<line number>] PRINT #<expression>,<expression>; 
Š                                 <expression> {,<expression>}

             (2) [<line number>] PRINT #<expression>; <expression> 
Š                                 {,<expression>}

             (3) [<line number>] PRINT <expression> <delim> 
                                 {<expression> <delim>}


        DESCRIPTION:

        A PRINT statement sends the value of the expressions in 
Š        the expression list to either a disk file (type(1) and 
Š        (2) or the console (type (3)). A type (1) PRINT statement 
Š        sends a random record specified by the second 
Š        <expression> to the disk file specified by the first 
Š        <expression>. An error occurs if there is insufficient 
Š        space in the record for all values.

        A type (2) PRINT statement outputs the next sequential 
Š        record to the file specified by the <expression> 
Š        following the #.

        A type (3) PRINT statement outputs the value of each 
Š        <expression> to the console. A space is appended to all 
Š        numeric values and if the numeric item exceeds the right 
Š        margin then the print buffer is dumped before the item is 
Š        printed. The <delim> between the <expressions> may be 
Š        either a comma or a semicolon. The comma causes automatic 
Š        spacing to the next tab position (14,28,42,56). If the 
Š        current print position is greater than 56 then the print 
Š        buffer is printed and the print position is set to zero. 
Š        A semicolon indicates no spacing between the printed 
Š        values. if the last (expression> is not followed by a 
Š        <delim> the print buffer is dumped and the print position 
Š        set equal to zero. The buffer is automatically printed 
Š        anytime the print position exceeds 71.

        EXAMPLES:

             100 PRINT #1;A,B,A$+"*"
             PRINT # FILE, WHERE; A/B,D,"END"
             PRINT A, B, "THE ANSWER IS";x
>PA
        ELEMENT:                                          RANDOMIZE

             RANDOMIZE statement

        SYNOPSIS:

             [<line number>] RANDOMIZE

        DESCRIPTION:

        A RANDOMIZE statement initializes the random number 
Š        generator.

        EXAMPLES:

             10 RANDOMIZE
             RANDOMIZE

>PA

        ELEMENT:                                               READ

             READ statement

        SYNOPSIS:

             (1) [<line number>] READ <expression>, <expression>;
                                 <variable> {,<variable>}

             (2) [<line number>] READ # <expression>;
                                 <variable> {,<variable>}

             (3) [<line number>] READ #<variable> {,<variable>}

        DESCRIPTION:

        A READ statement assigns values to variables in the 
Š        variable list from either a file (type (2) and (3)) or 
Š        from a DATA statement (type (1)). Type (2) reads a random 
Š        record specified by the second expression from the disk 
Š        file specified by the first expression and assigns the 
Š        fields in the record to the variables in the variable 
Š        list. Fields may be floating point or string constants 
Š        and are delimited by a blank or comma. Strings may 
Š        optionally be enclosed in quotes. An error occurs if 
Š        there are more variables than fields in the record.

        The type (3) READ statement reads the next sequential 
Š        record from the file specified by the expression and 
Š        assigns the fields to variables as described above.

        A type (2) READ statement assigns values from DATA 
Š        statements to the variables in the list. DATA statements 
Š        are processed sequentially as they appear in the program. 
Š        An attempt to read past the end of the last data 
Š        statement produces an error.

        EXAMPLES:

             100 READ A,B,C$

             200 READ # 1,I; PAY.REG,PAY.OT,HOURS.REG,HOURS.OT
            
             READ # FILE.NO; NAMES,ADDRESS$,PHONE$,ZIP
>PA
        ELEMENT:                                               REM

             REM statement


        SYNOPSIS:

             [<line number>] REM [<remark>]
             [<line number>] REMARK [<remark>]

        DESCRIPTION:

        A REM statement is ignored by the compiler and 
Š        compilation continues with the statement following the 
Š        next carriage return. The REM statement may be used to 
Š        document a program. REM statements do not affect the size 
Š        of programs that may be compiled or executed. An 
Š        unlabelled REM statement may follow any statement on the 
Š        same line. And the <line number> may occur in a GOTO 
Š        GOSUB, or ON statement.

        EXAMPLES:

             10 REM THIS IS A REMARK

             REMARK THIS IS ALSO A REMARK
          
             LET X = 0 REM INITIAL VALUE OF X
>PA
        ELEMENT:                                  reserved word list

             reserved word list

        SYNOPSIS:

             <letter> {<letter>} [$]

        DESCRIPTION:

        The following words are reserved by BASIC-E and may not 
Š        be used as <identifiers>:

        ABS            AND            ASC          ATN         CHR$ 
        CLOSE          COS            COSH         DATA        DEF
        DIM            ELSE           END          EQ          EXP
        FILE           FOR            FRE          GE          GO
        GOSUB          GOTO           GT           IF          INP
        INPUT          INT            LE           LEFT$       LEN
        LET            LOG            LT           MID$        NE
        NEXT           NOT            ON           OR          OUT
        POS            PRINT          RANDOMIZE    READ        REM
        RESTORE        RETURN         RIGHT$       RND         SGN
        SIN            SINH           SQR          STEP        STOP
        STR$           SUB            TAB          TAN         THEN
        TO             VAL

        Reserved words must be preceeded and followed by either a 
Š        special character or a space Spaces may not be embedded 
Š        within reserved words. Unless compiler toggle D is set, 
Š        lower-case letters are converted to uppercase prior to 
Š        checking to see if an <identifier> is a reserved word.
>PA
        ELEMENT:                                               RESTORE

             RESTORE statement

        SYNOPSIS:

             [<line number>] RESTORE

        DESCRIPTION:

        A RESTORE statement repositions the pointer into the data 
Š        area so that the next value read with a READ statement 
Š        will be the first item in the first DATA statement. The 
Š        effect of a RESTORE statement is to allow rereading the 
Š        DATA statements.

        EXAMPLES:

             RESTORE
             
             10 RESTORE
>PA
        ELEMENT:                                               RETURN

             RETURN statement

        SYNOPSIS:

             [<line number>] RETURN

        DESCRIPTION:

        Control is returned from a subroutine to the calling 
Š        routine. The return address is maintained on the top of 
Š        the run-time monitor stack. No check is made to insure 
Š        that the RETURN follows a GOSUB statement.

        EXAMPLES:

        130 RETURN
            RETURN
>PA

        ELEMENT:                                               RIGHT$

             RIGHT$ predefined function

        SYNOPSIS:

             RIGHT$ (<expression>,<expression>

        DESCRIPTION:

        The RIGHT$ function returns the n rightmost characters of 
Š        the first <expression>. The value of n is equal to the 
Š        integer portion of the second <expression>. If n is 
Š        negative an error occurs; if n is greater than the length 
Š        of the first <expression> then the entire <expression> is 
Š        returned. The first argument must produce a string and 
Š        the second must produce a floating point number

        EXAMPLES:

             RIGHT$(X$,1)
             RIGHT$(NAME$,LNG.LAST)

>PA
        ELEMENT:                                               RND

             RND predefined function

        SYNOPSIS:

             RND

        DESCRIPTION:

        The RND function generates a uniformly distributed random 
Š        number between 0 and 1.

        EXAMPLE:

             RND
>PA
        ELEMENT:                                               SGN

             SGN predefined function

        SYNOPSIS:

             SGN (<expression>)

        DESCRIPTION:

        The SGN function returns 1 if the value of the 
Š        <expression> is greater than 0, -1 if the value is less 
Š        than 0 and 0 if the value of the <expression> is 0. The 
Š        argument must evaluate to a floating point number.

        EXAMPLES:

             SGN(X)

             SGN(Z - B + C)
>PA
        ELEMENT:                                               SIN

             SIN predefined function

        SYNOPSIS:

             SIN (<expression>)

        DESCRIPTION:

        SIN is a predefined function which returns the sine of 
Š        the <expression> The argument must evaluate to a floating 
Š        point number in radians.

        A floating point overflow occurs if the absolute value of 
Š        the <expression> is greater than two raised to the 24th 
Š        power times pi.

        EXAMPLES:

             X = SIN(Y)

             SIN(A - B/C)
>PA
        ELEMENT:                                               SINH

             SINH predefined function

        SYNOPSIS:

             SINH (<expression>)

        DESCRIPTION:

        SINH is a function which returns the hyperbolic sine of 
Š        the <expression>. The argument must evaluate to a 
Š        floating pointer number.

        EXAMPLES:

             SINH(Y)

             SINH(B<C)
>PA
        ELEMENT:                                   special characters

             special characters

        DESCRIPTION:

             The following special characters are used by BASIC-E:

                  ^                   circumflex

                  (                   open parenthesis

                  )                   closed parenthesis

                  *                   asterisk

                  +                   plus

                  -                   minus

                  /                   slant

                  :                   colon

                  ;                   semicolon

                  <                   less-than

                  >                   greater-than

                  =                   equal

                  #                   number-sign

                  ,                   comma

                  CR                  carriage return

                  \                   backslant

        Any special character in the ASCII character set may 
Š        appear in a string. Special characters other than those 
Š        listed above, if they appear outside a string will 
Š        generate an IC error.
>PA
        ELEMENT:                                          <statement>

             <statement>

        SYNOPSIS:

             [<line number>] <statement list><cr>

             [<line number>] IF statement <cr>

             [<line number>] DIM statement <cr>
             
             [<line number>] DEF statement <cr>

             [<line number>] END statement <cr>

        DESCRIPTION:

        All BASIC-E statements are terminated by a carriage 
Š        return (<cr>).
>PA
        ELEMENT:                                   <statement list>

             <statement list>

        SYNOPSIS:

             <simple statement> (: <simple statement>)

             where a <simple statement> is one of the following:

                  FOR statement
                  NEXT statement
                  FILE statement
                  CLOSE statement
                  GOSUB statement
                  GOTO statement
                  INPUT statement
                  LET statement
                  ON statement
                  PRINT statement
                  READ statement
                  RESTORE statement
                  RETURN statement
                  RANDOMIZE statement
                  OUT statement
                  STOP statement
                  <empty> statement

        DESCRIPTION:

             A <statement list> allows more than one <statement> to 
Š             occur on a single line.

        EXAMPLES:

             LET I = 0 : LET J = 0 : LET K - 0

             X = Y+Z/W : RETURN

             :::::: PRINT "THIS IS OK TOO"

>PA
        ELEMENT:                                               STR$

             STR$ predefined function

        SYNOPSIS:

             STR$ (<expression>)

        DESCRIPTION:

             This STR$ function returns the ASCII string which 
Š        represents the value of the <expression>. The argument 
Š        must evaluate to a floating point number.

        EXAMPLES:

             STR$(X)

             STR$(3.141617)
>PA
        ELEMENT:                                   <subscript list>

             <subscript list>

        SYNOPSIS:

             <expression> {<expression>}

        DESCRIPTION:

        A <subscript list> may be used as part of a <DIM 
Š        statement> to specify the number of dimensions and extent 
Š        of each dimension of the array being declared or as part 
Š        of a <subscripted variable> to indicate which element of 
Š        an array is being referenced.

        There may be any number of expressions but each must 
Š        evaluate to a floating point number. A <subscript list> 
Š        as part of a DIM statement may not contain a reference to 
Š        the array being dimensioned.

        EXAMPLES:

             X(10,20,20)

             Y$(1,J)

             CUST(AMT(I),PRICE(I))
>PA
        ELEMENT:                                               SQR

             SQR  (<expression>)

        DESCRIPTION:

        SQR returns the square root of the absolute value of the 
Š        <expression>. The argument must evaluate to a floating 
Š        point number.

        EXAMPLES:

             SQR (Y)

             SQR(X^2 + Y^2)
>PA
        ELEMENT:                                               TAB

             TAB predefined function

        SYNOPSIS:

             TAB (<expression>)

        DESCRIPTION:

        The TAB function positions the output buffer pointer to 
Š        the position specified by the integer value of the 
Š        <expression> rounded to the nearest integer modulo 73. If 
Š        the value of the rounded expression is less than or equal 
Š        to the current print position, the print buffer is dumped 
Š        and the buffer pointer is set as described above.

             The TAB function may occur only in PRINT statements.

        EXAMPLES:

             TAB(10)

             TAB(I + 1)
>PA
        ELEMENT:                                               STOP

             STOP statement

        SYNOPSIS:

             [<line number>] STOP

        DESCRIPTION:

        Upon encountering a <STOP statement> program execution 
Š        terminates an all open files are closed. The print buffer 
Š        is emptied and control returns to the host system. Any 
Š        number of STOP statements may appear in a program.

        A STOP statement is appended to all programs by the 
Š        compiler.

        EXAMPLES:

             10 STOP

             STOP
>PA
        ELEMENT:                                               TAB

             TAB predefined function

        SYNOPSIS:

             TAN (<expression>)

        DESCRIPTION:

        TAN is a function which returns the tangent of the 
Š        expression. The argument must be in radians.

        An error occurs if the <expression> is a multiple of pi/2 
Š        radians.

        EXAMPLES:

             10 TAN(A)

             TAN(X - 3*COS(Y))
>PA
        ELEMENT:                                               VAL

             VAL predefined function

        SYNOPSIS:

             VAL (<expression>)

        DESCRIPTION:

        The VAL function converts the number in ASCII passed as a 
Š        parameter into a floating point number. The <expression> 
Š        must evaluate to a string.

        Conversion continues until a character is encountered 
Š        that is not part of a valid number or until the end of 
Š        the string is encountered.

        EXAMPLES:

             VAL(A$)

             VAL("3.789" + "E-07" + "THIS IS IGNORED")
>PA
        ELEMENT:                                          <variable>

             <variable>

        SYNOPSIS:

             <identifier> [ (<subscript list>) ]

        DESCRIPTION:

        A <variable> in BASIC-E may either represent a floating 
Š        pint number or a string depending on the type of the 
Š        <identifier>. Subscripted variables must appear in a DIM 
Š        statement before being used as a <variable>.

        EXAMPLES:

             X

             Y$(3,10)

             ABS.AMT(X(I),Y(I),S(I-1))
>PA

                   III. OPERATING INSTRUCTIONS FOR BASIC-E

        The BASIC-E programs are written to operate with the CP/M 
Š        Floppy Disk Operating System (4). Operation with a different 
Š        system will require modification to the input/output routines 
Š        in the compiler and run-time monitor. Execution of the program 
Š        using BASIC-E consists of three steps. First the source 
Š        program must be created on disk. Next the program is compiled 
Š        by executing the BASIC-E compiler with the name of the source 
Š        program provided as a parameter. Finally the intermediate 
Š        (INT) file created by the compiler may be interpreted by 
Š        executing the run-time monitor, again using the source program 
Š        name as a parameter.

        Creation of the source program will normally be accomplished 
Š        using CP/M's text editor, and must have a file type BAS. The 
Š        BASIC-E statements are free from with the restriction that 
Š        when a statement is not completed on a single line, a 
Š        continuation character (\) must be that last character on the 
Š        line. Spaces may precede statements and any number of spaces 
Š        may appear wherever one space is permitted. Line numbers need 
Š        only be used on statements to which control is passed. The 
Š        line numbers do not have to be in ascending order. Using 
Š        identifiers longer than two characters and indenting 
Š        statements to enhance readability does not affect the size of 
Š        the object file created by the compiler.

        The first statement of a source program may be used to specify 
Š        certain compiler options. If present, this statement must 
Š        begin with a dollar sign($) in column one and be followed by 
Š        the letter or letters indicating the options which are 
Š        desired.The letters may be separated by any number of blanks. 
Š        Invalid letters or characters are ignores. Appendix D lists 
Š        valid compiler options, and their initial settings. Toggle A 
Š        is used for compiler debugging. Toggle B supresses listing of 
Š        the source program except for statements with errors. Toggle C 
Š        compiles the program but does not create a INT file. Normally 
Š        the BASIC-E compiler converts all letters appearing in 
Š        identifiers or reserved words to uppercase. If Toggle D is set 
Š        this conversion is not performed. Letters appearing in strings 
Š        are never converted to uppercase. Toggle E causes code to be 
Š        generated by the compiler so that, upon detection of a run-
Š        time error, the source statements line which was being 
Š        executed at the time the error occurred is listed along with 
Š        the error message.


                 The BASIC-E compiler is invoked as follows:

                             BASIC <program name>

        The compiler begins execution by opening the source file 
Š        specified as a parameter and compiles each BASIC-E statement 
Š        producing an object file in the BASIC-E machine language with 
Š        the same name as the source program but of type "INT". The 
Š        source program may be listed on the output device with any 
Š        error messages following each line of the program. Appendix A 
Š        lists the compiler error messages. Errors SO, TO and VO 
Š        indicate storage used by the compiler has been exceeded. These 
Š        errors require recompilation of the BASIC-E compiler with more 
Š        space allocated to the particular vector involved. Figures 1 
Š        and 2 show sample program listings. If no errors occur during 
Š        compilation, the object file may be executed by the run-time 
Š        monitor by typing the command:

                              RUN <program name>

        The run-time monitor consists of two programs. The first 
Š        program initializes the floating point package and then reads 
Š        the intermediate language file specified as a parameter As the 
Š        INT file is read, the floating point constants, BASIC-E 
Š        machine code, and the data area are built. At this time branch 
Š        addresses and references to floating point constanst and the 
Š        program reference table (PRT) are relocated to reflect actual 
Š        machine addresses. Control is then passed to the interpreter 
Š        for execution of the BASIC-E machine language. The program 
Š        which builds the machine resides in the free storage area and 
Š        is overwritten as space is allocated by the interpreter. 
Š        Appendix B lists run-time error messages. Figure 3 outlines 
Š        the structure of the BASIC-E machine.

        The BASIC-E machine is divided into a static and varying area. 
Š        The static section consists of the BASIC-E Run Time Monitor, 
Š        constants, machine code and data statements. The varying area, 
Š        which includes all the remaining memory, stores program 
Š        variables, the stack and the free storage area. The stack is a 
Š        circular queue and therefore it is not possible to overflow 
Š        the stack. However, if the stack wraps around onto itself the 
Š        results could be meaningless. The size of the stack is 
Š        initially set at 12 but may be changed by recompiling the 
Š        interpreter. The free storage area is used to dynamically 
Š        allocate arrays and strings. Storage is freed as soon as it is 
Š        no long required by the program.

        All requests for input from the console are prompted with 
Š        question mark. If more information is entered than was 
Š        requested, or if insufficient information is entered, a 
Š        warning occurs and the entire line of data must be entered 
Š        again. A program may be terminated by typing a control-Z 
Š        followed by a carriage return in response to an input request.

        Disk files may be read, written or updated by the BASIC-E 
Š        programmer using both sequential and random access. there are two 
Š        basic types of files. Those with no declared record size, which 
Š        are referred to as unblocked, and those with user-specified 
Š        record size. The latter are blocked files. Blocked files may be 
Š        processed either sequentially or randomly, while unblocked files 
Š        must be accessed sequentially. All data in files is stored as 
Š        ASCII characters. Either a comma or carriage return denotes the 
Š        end of field. Blanks are ignored between fields.

        In unblocked files there is no concept of a record as such. Each 
Š        reference to the file either reads from or writes to the next 
Š        field. At the end of each <write statement> a carriage return and 
Š        a line feed are written to the file. This provides compatibility 
Š        with the operating system utilities such as TYPE and also allows 
Š        files to be created by the text editor.

        Blocked files consist of a series of fixed length records. The 
Š        user specifies the logical record length with the <file 
Š        statement>. An error occurs if a line feed is encountered during 
Š        a read from a blocked file or if the current record being built 
Š        exceeds the block size during a write. At the end of a write 
Š        statement any remaining area in the record is filled with blanks, 
Š        and then a carriage return and linefeed are added.
>PA

        REMARK PROGRAM TO COMPUTE THE FIRST N
        REMARK FIBONACCI NUMBERS

        PRINT "THIS PROGRAM COMPUTES THE FIRST N"
        PRINT "FIBONACCI NUMBERS"
        PRINT "AN INPUT OF 0 TERMINATES THE PROGRAM"

        FOR I = 1 TO 1 STEP 0 REMARK DO THIS FOREVER
        INPUT "ENTER THE VALUE OF N"; N
        IF N = 0 THEN \
           PRINT "PROGRAM TERMINATES":\
           STOP

        IF N 0 THEN \
           PRINT "N MUST BE POSITIVE."; :\
           PRINT "PLEASE REENTER"\
        ELSE \
           GOSUB 300 REMARK CALCULATE AND PRINT RESULTS
        NEXT 1

        300 REMARK SUBROUTINE TO CALCULATE FIB NUMBERS
        F1 = 1 : F2 = 1 REMARK INITIAL VALUES
        NUM = F1

        REMARK HANDLE FIRST TWO NUMBERS (IF REQ) AS REMARK SPECIAL CASES

        FOR J = 1 TO 2 
            GOSUB 400
            IF N = 0 THEN \
            RETURN
        NEXT J

        REMARK HANDLE REMAINING NUMBERS

        FOR J = 1 TO 1 STEP 0
            NUM = F1 + F2
            GOSUB 400
            F2 = F1
            F1 = NUM
            IF N - 0 THEN \
            RETURN
        NEXT J
        RETURN

        400   REMARK PRINT NEXT NUMBER AND
              REMARK DECREMENT N
              PRINT NUM, REMARK 5 TO A LINE
              N = N - 1
              RETURN
        END
>PA
        REMARK    PROGRAM BUILDS A FILE OF MAILING LABELS
        REMARK    FROM A FILE CONTAINING 100 BYTE RECORDS
        REMARK    WHICH CONTAIN NAME AND ADDRESS INFORMATION

             INPUT "NAME OF COURSE FILE";SOURCE.FILE$
             INPUT "NAME OF LABEL FILE";LABEL.FILE$
             IF END # 1 THEN 100
             FILE SOURCE.FILE$(100), LABEL.FILE
        REMARK LABEL.FILE IS NOT BLOCKED

             FOR INDEX = 1 TO 1 STEP 0 REM UNTIL END OF FILE
                 READ # 1; FIRST$, LAST$, STREET$, CITY$, STATE$, ZIP

                 REMARK LINES ARE TRUNCATED AT 60 CHARACTERS

        LINE1$ = LEFT$(FIRST$ + " " + LAST$,60)
        LINE2$ = LINE3$ + " " + SIR$(ZIP)

        REMARK INSURE ZIP NOT TRUNCATED

        LINE3$ = LEFT$(CITY$ + " , " + STATE$54)
        LINE3$ = LINE3$ + " " + SIR$(ZIP)

        PRINT # 2; LINE1$
        PRINT # 2; LINE2$
        PRINT # 2; LINE3$
        NEXT INDEX

        100 PRINT "JOB COMPLETE"
            STOP

        END
>PA
                     APPENDIX A - COMPILER ERROR MESSAGES

        CE   Could not close file.
        DE   Disk error
        DF   Could not create INT file; disk or directory is full.
        DL   Duplicate labels or synchronization error.
        DP   Identifier in DIM statement previously defined.
        FC   Identifier in FILE statement previously defined.
        FD   Predefined function name previously defined.
        FI   FOR loop index is not a simple floating point variable.
        FN   Incorrect number of parameters in function reference.
        FP   Invalid parameter type in function reference.
        FU   Function is undefined.
        IC   Invalid character in BASIC statement.
        IE   Expression in IF statement is not of type floating point.
        IS   Subscripted variable not previously dimensioned.
        IU   Array name used as simple variable.
        MF   Expression is of type string where only floating point is 
Š             allowed.
        MM   Expression contains string and floating point variables in 
Š             mixed mode expression.
        NI   Identifier following NEXT does not match FOR statement index.
        NP   No applicable production exists.
        NS   No BAS file found
        NU   NEXT statement without corresponding FOR statement.
        SN   Incorrect number of subscripts.
        SO   Compiler stack overflow
        TO   Symbol table overflow.
        UL   Undefined label.
        VO   VARC overflow. 
>PA
                 APPENDIX B - RUN-TIME MONITOR ERROR MESSAGES

        AC   Null string passed as parameter to ASC function.
        CE   Error closing a file.
        DR   Disk read error (reading unwritten data in random access).
        DW   Error writing to a file.
        DZ   Division by zero.
        EF   EOF on disk file; no action specified
        ER   Exceeded record size on block file.
        II   Invalid record number in random access.
        FU   Accessing an unopened file.
        ME   Error attempting to create a file.
        MF   File identifier too large or zero.
        NE   Attempt to raise a number to a negative power.
        NI   No INT file found in directory.
        OD   Attempt to read past end of data area.
        OE   Error attempting to open a file.
        OI   Index in ON statement out of bounds.
        RE   Attempt to read past end of record on blocked file.
        RU   Unblocked file used with random access.d
        SB   Array subscript out of bounds.
        SL   String length exceeds 255.
        SS   Second parameter of MID$ is negative.
        TZ   Attempt to evaluate tangent of pi over two.  
>PA
                         APPENDIX C - BASIC-E GRAMMAR

        1 <program> ::= <line number> <statement> <cr>
        2 <line number> ::= <number>
        4 <statement> ::= <statement list>
        5               : <if statement>
        6               : <end statement>
        7               : <dimension statement>
        8               : <define statement>
        9 <statement list> ::= < simple statement>
        10                   : <statement list>:
        10                   : <simple statement>
        11 <simple statement>  ::= <simple statement>
        12                       : <assignment>
        13                       : <for statement>
        14                       : <next statement>
        15                       : <file statement>
        16                       : <close statement>
        17                       : <read statement>
        18                       : <print statement>
        19                       : <goto statement>
        20                       : <gosub statement>
        21                       : <input statement>
        22                       : <stop statement>
        23                       : <return statement>
        24                       : <on statement>
        25                       : <restore statement>
        26                       : <randomize statement>
        27                       : <out statement>
        28                       :

        29 <let statement> ::= let <assignment>
        30 <assignment> ::= <assign head> <expression>
        31 <assign head> ::= <variable> =
        32 <expression> ::= <logical factor>
        33                : <expression> <or> <logical factor>

        34 <or> ::= or
        35        : xor

        36 <logical factor> ::= <logical secondary>
        37                    : <logical factor> and
        37                      <logical secondary>
        38 <logical secondary> ::= <logical primary>
                                 : not <logical primary>
        40 <logical primary> ::= <arithmetic expression>
                               : <arithmetic expression>
                                 <relation>
                                 <arithmetic expression>
        42 <arithmetic expression> ::= <term>
        43                           : <arithmetic expression> +
        43                             <term>

        44                           : <arithmetic expression> -
        44                             <term>
        45                           : + <term>
        46                           : - <term>

        47 <term> ::= <primary>
        48          : <term> * <primary>
        49          : <term> / <primary>

        50 <primary> ::= <element>
        51             : <primary> ** <element>

        52 <element> ::= <variable>
        53             : <constant>
        54             : <function call>
        55             : ( <expression> )

        56 <variable>  ::= <identifier>
        57               : <subscript head> <expression>)

        58 <subscript head>  ::= <identifier> (
        59                     : <subscript head> < expression>,

        60 <function call>   ::= <function heading> <expression>)
        61                     : <function name>
        62 <function heading> ::= <function name> (
        63                      : <function heading> <expression>
        63

        64 <function name>  ::= <user defined name>
        65                    : <predefined name>

        66 <constant>  ::= <number>
        67               : <string>

        68 <relation>  ::=  =
        69               :  >=
        70               :  ge
        71               :  <=
        72               :  le
        73               :  >
        74               :  <
        75               :  <>
        76               :  ne
        77 <for statement> ::= <for head> to <expression>
        77                     <step clause>

        78 <for head>  ::= <for> <assignment>
        79 <for>  ::= for

        80 <step clause>  ::=  step <expression>
        81                  :


        82 <if statement>   ::= <if group>
        83                    : <if else group> <statement list>
        84                    : if end # <expression> then <number>

        85 <if group>  ::= <if head> <statement list>
        86               : <if head> <number>

        87 <if else group>  ::= <if head> <statement list> else

        88 <if head>  ::= if <expression> then

        89 <define statement>  ::= <ud function name>
        89                         <dummy arg list> = <expression>

        90 <ud function name>  ::= def <userdefined name>

        91 <dummy arg list>    ::= <dummy arg head> <identifier>
        92                       :

        93 <dummy arg head>    ::= (
        94                       : <dummy arg head> <identifier>

        95 <file statement>    ::= <file head> <file declaration>

        96 <file head>   ::= file
        97                 : <file head> <file declaration>

        98 <file declaration>   ::= <identifier> <file rec size>

        99 <file rec size>  ::= (<expression>)
        100                   :

        101 <dimension statement>  ::= dim
        101                            <dimension variable list>

        102 <dimension variable list>   ::= <dimension variable>
        103                               :
        103                                 <dimension variable list>
        103                                 , <dimension variable>

        104 <dimension variable>  ::= <dim var head> <expression>)

        105 <dim var head>  ::= <identifier> (
        106                   : <dim var head> <expression>,
        107 <close statement>   ::= close <close list>

        108 <close list>   ::= <expression>
        109                  : <close list>, <expression>

        110 <read statement>  ::= read <file option> <read list>
        111                     : read <read list>

        112 <input statement>  ::= input <prompt option>
        112                        <read list>
        113 <prompt option>  ::= <constant> ;
        114                    :

        115 <read list>   ::= <variable>
        116                 : <read list> , <variable>
        117                 :

        118 <print statement>  ::= print <print list> <print end>
        119                      : print <file option> <file list>

        120 <print list>  ::= <expression>
        121                 : <print list> < print delim>
        121                   <expression>
        122                 :

        123 <file list>     ::= <expression
        124                   : <file list>, <expression>

        125 <print end>    ::= <print delim>
        126                  :

        127 <file option>  ::=  # <expression>
        128                  :  # <expression> , <expression>;

        129 <print delim>   ::=  ;
        130                   :

        131 <goto statement>  ::= <goto><number>
        132                     :
        132 <on statement>   ::= <on goto> < label list>
        133                    : <on gosub> <label list>

        134 <on goto>    ::= on <expression> <goto>

        135 <on gosub>  ::= on <expression> <gosub>

        136 <label list> ::= <number>
        137                : <label list> , <number>

        138 <gosub statement>  ::= <gosub> <number>

        139 <goto>  ::= goto
        140           : go to

        141 <gosub> ::= gosub
Š        142           : go sub

        143 <next statement>  ::= <next head> <identifier>
        144                     : next

        145 <next head>  ::= next
        146                : <next head> <identifier> ,

        147 <out statement>  ::= out <expression> , <expression>

        148 <return statement>  ::= return

        149 <stop statement>  ::= stop

        150 <end statement>   ::= end

        151 <restore statement>  ::= restore

        152 <randomize statement)  ::= randomize
>PA
        APPENDIX D - COMPILER OPTIONS

        OPTION                   DESCRIPTION                   DEFAULT

        A         List productions (for compiler debugging).   OFF

        B         List only source statements with errors.     OFF

        C         Do not crete INT file; syntax check only.    OFF

        D         Convert lowercase to uppercase.              ON

        E         Generate line number code                    OFF
>PA
                                 BIBLIOGRAPHY

        1. Draft Proposed American nation Standard Programming 
Š        Language Minimal BASIC. X3J2/76-01 76-01-01. Technical 
Š        Committee X3J2-BASIC American National Standards Committee X3 
Š        - Computers and Information Processing.

        2. Worth, Thomas. BASIC For Everyone. Englewood Cliffs: 
Š        Prentice Hall, Inc., 1976.

        3. Albrecht, Robert L., Fenkel, Le Roy and Brown, Jerry. 
Š        BASIC. New York: John Wiley & Sons, Inc., 1973.

        4. Kldall, G.A. An Introduction to CP/M Features and 
Š        Facilities. Digital Research, 1976.
>PA

                           INTRODUCTION TO BASIC-E

        BASIC-E  is a compiled, scientifically-oriented, fast-
Š        executing BASIC language processor. BASIC-E was written by 
Š        Gordon E. Eubanks, Jr. at the Naval Postgraduate School, 
Š        Monterey, California, and is in the public domain. 

        The BASIC-E language features include sequential and random 
Š        diskette file capabilities, a wide range of predefined 
Š        functions, user-defined functions, and strings up to 255 
Š        characters long. Arrays may have any number of subscripts and 
Š        may be string or numeric. In a string array, each element can 
Š        be a string of up to 255 characters. Numbers in BASIC-E are 
Š        represented internally in binary floating point and 
Š        approximately 7 significant decimal digits are carried. BASIC-
Š        E currently has no provision for formatted printing, and no 
Š        provision for calling machine language subroutines.

                           BASIC-E LANGUAGE SUMMARY

        Numbers

        Range 2.7E-39 to 3.6E38
        Approximately 7 significant decimal digits are represented

        Strings

        0 to 255 ASCII characters Storage for strings is automatically 
Š        allocated as required during program execution. In a string 
Š        array (subscripted string variable) each element is a separate 
Š        string of characters.

        Identifiers

        Variable names up to 31 characters. First character must be a 
Š        letter; remainder must be letter, digit, or period.

        Numeric

        (, ),,*,/,+,-

        Operators

        <, <=, >, >=, =, <>      (relational)
        LT,LE,GT,GE,EQ,NE    (same as preceding line)
        NOT,AND,OR

        String

        +                    (concatenation)

        Operators

        Relational operators (as above)

        Statement

        LET

        Types

        IF...THEN line number
        IF...THEN any statement
        IF...THEN...ELSE...
        FOR
        NEXT
        GOTO
        GOSUB
        RETURN
        ON...GOTO list of line numbers
        ON...GOSUB list of line numbers
        RANDOMIZE
        PRINT
        READ
        OUT port, data
        DATA
        RESTORE
        REM
        END

        DIM Statements

        DIM name (dimensions), name (dimensions),...

        Any number of subscripts are allowed. All arrays must be 
Š        DIMensioned and the DIM statement should appear in the program 
Š        before the array is referenced. Examples:

        DIM N(100)
        DIM BIT(2,4,3,3,2,2,8), DATASTRINGS$ (10,5,23)

        User Defined

        DEF FNname (arguments) = expression

        Functions

        Examples:

        DEF FNA (X+Y)=X+Y-A
        DEF FNXY.COMBINE$(A$+B$)=A$+", "+B$

        Sequential

        FILE Variable$, variable$...

        File Operations

        Opens one or more files. Each string variable's value is taken as 
Š        the name of a file to open; the first file opened is #1, the 
Š        second, #2, etc. Up to six files can be open at one time.

        PRINT #filenumber'; list

        Outputs a list of variables or expressions to the indicated file. 
Š        "filenumber" is a numeric expression; note the # before 
Š        filenumber and the ; after it.

        READ #filenumber; list

        Reads a list of variables from indicated open file.

        CLOSE filenumber, filenumber
        Closes one or more files.

        IF END #filenumber THEN line number
        Specifies line number to GOTO if and when a subsequent INPUT 
Š        attempts to ready beyond the end of the indicated file.

        Random Access

        FILE variable$ (recordlen), variable$ (recordlen)...

        File Operations

        Opens files for random access. Recordlen is an expression 
Š        specifying the length of each record in the file, that is, how 
Š        many bytes are to be transmitted with each PRINT or INPUT 
Š        statement.

        PRINT #filenumber, recordnumber; list
        READ #filenumber, recordnumber; list

        These access the specified record of a file. Note that there is a 
Š        comma before the record number expression and a semicolon after 
Š        it.

        Predefined

        X represents any numeric expression

        Numeric Functions

        ABS(X)
        ATN(X)
        COS(X)
        COSH(X)
        FRE
        INP(X)
        INT(X)
        LOG(X)
        POS(X)

        hyperbolic cosine
        number of bytes of unused storage
        input from port X
        carriage position

        Predefined Functions

        RND
        SGN(X)
        SIN(X)
        SINH(X)
        SQR(X)
        TAB(X)
        EXP(X)

        A$ represents any string expression

        String Functions

        ASC(A$)
        CHR$(X)
        LEFT$(A$)
        LEN(A$)
        MID$(A$, start,length)
        RIGHT$(A$,length)
        STR$(X)
        VAL(A$)

        BASIC-E Operation

        To use BASIC-E you must have at least 20K of RAM (many 
Š        applications will require more).

        BASIC-E source programs reside on diskette files of type BAS. 
Š        Usually they are input from the terminal with the CP/M editor 
Š        (ED). Like any other file, a BASIC-E program may be printed out 
Š        with the CP/M type or list command, copied to another file with 
Š        PIP, updated with ED, etc.

        Before it can be run, a BASIC-E program must be compiled. This is 
Š        done with the CP/M command:

        EBASIC name

        where "name" is the name of the file containing the program. The 
Š        compilation operation lists the program, types error message if 
Š        any are found in the program, and produces a new file with the 
Š        same name but of type INT.

        The compiled program can then be executed with the command:

        ERUN name

        The program can be used as many times as desired by reentering 
Š        the ERUN command.

        To modify the program, EDit the source file (name.BAS), 
Š        recompile it, then reERUN.

        Known Bugs

        1. PRINT sometimes makes an error of one in the seventh 
Š        significant digit; thus, for example, 93 may print out as 
Š        92.99999. This error occurs on printout and does not effect 
Š        arithmetic done in the program.

        Notes

        1. The RANDOMIZE statement is effective only if an INPUT 
Š        statement has been executed earlier in the program. The 
Š        variable time it takes the user to respond at the terminal is 
Š        used to randomize the random number generator.

        2. A CLOSE statement sometimes produces an apparently 
Š        erroneous error message from the compiler. Usually the CLOSE 
Š        statement can be omitted as all files are automatically closed 
Š        at the end of program execution.

        3. In some cases, when a file is written with a BASIC-E 
Š        program, garbage is left in the file after the last record. 
Š        This garbage shows up when the file is later TYPEd, LISTed, or 
Š        EDited. If you have trouble with this program, try putting an 
Š        ASCII control-Z character:

             PRINT #n; CHR$(26)
             where n is the file number.
>PA
(same as preceding line)
        NOT,AND,OR

        String

