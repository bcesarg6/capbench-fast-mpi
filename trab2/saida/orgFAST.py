import sys
file = sys.argv[1]

transfer = []
time = []

with open(file) as f:
    line = ''
    for i in xrange(10):
        line = f.readline()[:-1]
        transfer.append(float(line))

        line = f.readline()[:-1]
        time.append(float(line))

        f.readline()

transfer.sort()
time.sort()

del transfer[0]
del transfer[-1]

del time[0]
del time[-1]

print sum(transfer) / float(len(transfer))
print sum(time) / float(len(time))
