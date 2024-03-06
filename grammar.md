# EngLang grammar

program           ::= statement_list

statement_list    ::= statement
                    | statement statement_list

statement         ::= if_statement
                    | for_each_statement
                    | function_declaration
                    | launch_statement
                    | display_statement

if_statement      ::= "if" expression "do" statement "else" statement "end if"

for_each_statement ::= "for each" identifier "in" "[" number_list "]" statement "end for"

function_declaration ::= "fn" identifier "use" identifier_list ":" statement_list "end fn"

launch_statement  ::= "launch" identifier "using" number_list

display_statement ::= "display" string

expression        ::= number
                    | identifier
                    | comparison

comparison        ::= identifier "=" identifier

number_list       ::= number
                    | number "," number_list

identifier_list   ::= identifier
                    | identifier "," identifier_list

