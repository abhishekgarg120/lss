lss
===

**Utility to find all the valid sequences in folder.**


Copy `lss.py` to `/usr/local` and then create a new executable file as `/usr/local/bin/lss` with the following content

```bash
#!/usr/bin/env bash
/usr/bin/python /usr/local/lss.py $@
```

Build `lss.cpp`

```bash
g++ -o lss -lboost_system -lboost_regex -lboost_filesystem --std=c++11 lss.cpp
```