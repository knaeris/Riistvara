#!/bin/bash

# Simple Bash script for code formatting in 1tbs.
# See http://astyle.sourceforge.net/astyle.html for syntax and defaults.

# See man systexits for exit codes descriptions

MINPARAMS=1
ORIG_SUFFIX=orig
EXIT_CODE=0

# Exit error if astyle not installed
if ! [ -x "$(command -v astyle --version)" ]; then
    echo 'Error: astyle not installed. Please install it via apt-get install astyle' >&2
    # SYSEXITS EX_UNAVAILABLE(69)
    EXIT_CODE=69
    exit $EXIT_CODE
fi

# Exit error if not enough arguments
if [ $# -lt "$MINPARAMS" ]
then
    echo "Error: This script needs C source files passed as arguments" >&2
    echo "USAGE: format-code.sh src/main.c src/somecode.c ..." >&2
    # SYSEXITS EX_USAGE(64)
    EXIT_CODE=64
    exit $EXIT_CODE
fi

#Print astyle version
echo "Using: " $(astyle --version)

# For all files provided as arguments
for FILE in "$@"
do
    RESULT="$(astyle    --style=1tbs \
                        --indent-col1-comments \
                        --break-blocks \
                        --pad-oper \
                        --pad-header \
                        --delete-empty-lines \
                        --add-brackets \
                        --convert-tabs \
                        --max-code-length=80 \
                        --break-after-logical \
                        --mode=c \
                        --suffix=.$ORIG_SUFFIX \
                        --lineend=linux \
                        $FILE)"

    # If file unchanged print unchanged result message
    if [[ "$RESULT" = Unchanged* ]] || [[ "$RESULT" = Muutumatu* ]]
    then
        echo "$RESULT"
    fi

    # If file formatted print result and renamed original file name
    if [[ "$RESULT" = Formatted* ]] || [[ "$RESULT" = Formaadis* ]]
    then
        echo "$RESULT"
        echo "Original code was preserved in file $FILE.$ORIG_SUFFIX"
        # Exit with SYSEXITS EX_DATAERR(65) if code formatting was changed. Needed for CI to fail tests when code formatting not correct
        EXIT_CODE=65
    fi
done

exit $EXIT_CODE
