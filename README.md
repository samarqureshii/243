# ECE243: Computer Organization (Annie's Branch)
> Programming labs in Nios II Architecture. By Annie Wang and Samar Qureshi.

## How to use [CPUlator](https://cpulator.01xz.net/?sys=nios-de1soc&d_audio=48000)
- Write the code in the editor
    - Edits can be made in the `Editor` pane
- `File > Save` will save a copy of the text in your Downloads folder
- `Compile and Load` will assemble the program and load it into the memory of the simulated computer system
- `Disassembly` pane lets you see machine code and gives address in memory of each machine-code word 
- `Step Into` command allows you to execute your program one instruction at a time
- Breakpoints can be set at a specific memory address where you want the execution to pause 
    - Useful for debugging or examining the state of the program at certain points, such as before or after a loop or a function call
    - Use `Continue` command to run the program until it reaches the breakpoint 
- To set the register values back to 0, you can click on the register values and `Compile and Load` the program again
