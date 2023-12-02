#!/usr/bin/env bash
make clean
make zoo

./zoo --mode FASTTSP < spec-test.txt > specout.txt
diff <(sort spec-test-FASTTSP-out.txt) <(sort specout.txt)

./zoo --mode FASTTSP < sample-e.txt > eout.txt
diff <(head -n 1 sample-e-FASTTSP-out.txt) <(head -n 1 eout.txt)

./zoo --mode FASTTSP < sample-d.txt > dout.txt
diff <(head -n 1 sample-d-FASTTSP-out.txt) <(head -n 1 dout.txt)

./zoo --mode FASTTSP < sample-c.txt > cout.txt
diff <(head -n 1 sample-c-FASTTSP-out.txt) <(head -n 1 cout.txt)

./zoo --mode FASTTSP < sample-ab.txt > about.txt
diff <(head -n 1 sample-ab-FASTTSP-out.txt) <(head -n 1 about.txt)
diff <(tail -n +2 sample-ab-FASTTSP-out.txt | sort) <(tail -n +2 about.txt | sort) | head -n 20