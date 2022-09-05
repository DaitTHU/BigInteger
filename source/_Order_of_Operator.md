# Order of Operator in C++

|                              operator                              | binding order |
|:------------------------------------------------------------------:|:-------------:|
|                `()`, `[]`, <u>`->`</u>, `++`, `--`                 |      -->      |
|       `+`, `-`, `!`, `~`, `type*`, `type&`, <u>`sizeof`</u>        |      <--      |
|                           `*`, `/`, `%`                            |      -->      |
|                              `+`, `-`                              |      -->      |
|                             `<<`, `>>`                             |      -->      |
|                        `<`, `<=`, `>`, `>=`                        |      -->      |
|                             `==`, `!=`                             |      -->      |
|                                `&`                                 |      -->      |
|                                `^`                                 |      -->      |
|                                `\|`                                |      -->      |
|                                `&&`                                |      -->      |
|                               `\|\|`                               |      -->      |
|                            <u>`? :`</u>                            |      <--      |
| `=`, `+=`, `-=`, `*=`, `/=`, `%=`, `>>=`, `<<=`, `&=`, `^=`, `\|=` |      <--      |
(unoverloadingable operators are <u>underlined</u>.)

while `.`, `.*`, `->*`, `::` and `#` are unoverloadingable.