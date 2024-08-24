```
g++ -o output LUTConstruct_cont.cpp
b.exe N1 N2 P1 P2 T
```

This runs LUT construction for DB sizes 2^N1 to 2^N2 and for Page sizes 2^P1 to 2^P2, each experiment running for T test cases.
output is stored in csv file. re-running b.exe will overwrite previous experiment results
