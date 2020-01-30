# Advanced programming I milestone 2
## Repository link
https://github.com/davidros11/ms2
## instructions
Compile with

```bash
g++ -std=c++14 *.cpp -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic -o
a.out -pthread
```

Then run with

```bash
./a.out [port number]
```

Default port is 5400.
send matrices with the appropriate format. Each line of the matrix should be:
[number],[number],...,[number]\n
after that, show the beginning and end points respectively, and then put 'end' in the end:
[x-coordinate],[y-coordinate]\n
[x-coordinate],[y-coordinate]\n
end
any spaces will be ignored.
After that, you'll get a result for the shortest path from the beginning point to the end
point of the matrix


