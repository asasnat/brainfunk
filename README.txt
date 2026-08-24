    /---\                     *              --                /
   /    /                                   /                 /    /
  /----\  /---\  /-----\ ----/      /---\ --- /    /  /---\  /---/
 /     / /      |      /    |      /    / /  /    /  /    / /    \
/_____/ /        ----- \     \___ /    / /   \___/\ /    / /      \
--------------------------------------------------------------------
      An interpreter for the Brainfuck language, written in C.

--- Features ---

> You can set the size of the stack and the tape,
  respectively using the -s and -t command line options.
  You can expand them to make more complex programs work,
  or you can shrink them to reduce the load on weaker hardware.

> You can change how EOF in standard input will be handled,
  using the -e command line option.
  Supported paradigms are: zero, minus-one, and unchanged.

--- How to build ---

Just run make and it should work. Unless it doesn't, in which case you'll
have to troubleshoot some things, I guess.
