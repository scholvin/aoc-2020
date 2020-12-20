BEGIN {
    print "%{";
    print "extern int yylex();";
    print "int yyerror(const char* s);";
    print "%}";
    print "%start _0";
    print "%%";
}
{
    if (NF == 0)
    {
        # blank line - everything after this isn't important
        exit;
    }
    if (NF == 2)
    {
        # rule of the form R: "c" or R: N
        if (substr($2, 1, 1) == "\"")
        {
            # needs to be _R: 'c'
            printf("_%s '%s' ;\n", $1, substr($2, 2, 1));
        }
        else
        {
            printf("_%s _%s ;\n", $1, $2);
        }
    }
    else
    {
        for (i=1; i<=NF; i++)
        {
            if ($i == "|")
                printf(" | ");
            else
                printf("_%s ", $i);
        }
        print(" ;");
        
    }
}
END {
    print "%%";
    print "int yyerror(const char *s) { return -1; }";
}
