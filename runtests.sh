#!/usr/bin/env bash
make clean
make zoo


./zoo --mode MST < spec-test.txt > specout.txt
diff <(sort spec-test-MST-out.txt) <(sort specout.txt)


./zoo --mode MST < sample-e.txt > eout.txt
diff <(head -n 1 sample-e-MST-out.txt) <(head -n 1 eout.txt)

./zoo --mode MST < sample-d.txt > dout.txt
diff <(head -n 1 sample-d-MST-out.txt) <(head -n 1 dout.txt)

./zoo --mode MST < sample-c.txt > cout.txt
diff <(head -n 1 sample-c-MST-out.txt) <(head -n 1 cout.txt)

./zoo --mode MST < sample-ab.txt > about.txt
diff <(head -n 1 sample-ab-MST-out.txt) <(head -n 1 about.txt)
diff <(tail -n +2 sample-ab-MST-out.txt | sort) <(tail -n +2 about.txt | sort) | head -n 20

./zoo --mode MST < test-2-MST.txt > out.txt
diff <(head -n 1 t2-correct.txt) <(head -n 1 out.txt)
diff <(tail -n +2 t2-correct.txt | sort) <(tail -n +2 out.txt | sort) | head -n 20


